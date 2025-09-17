#include "TasksService.h"
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
TasksService::EquationCallData::EquationCallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks> api):
	TasksService::CallData(service, cq, api), writer(&context)
{
	proceed(true);
}
void TasksService::EquationCallData::proceed(bool ok)
{
	if (!ok)
	{
		delete this;
		return;
	}
	try 
	{
		switch(status)
		{
			case CREATE:
			{
				status = PROCESS;
				service->RequestCallEquation(&context, &request, &writer,cq,cq,this);
				break;
			}
			case PROCESS:
			{
				status = FINISH;
				std::vector<double> params_store(request.params().begin(), request.params().end());
				matrix<double> params(request.size_row(), request.size_col());
				*params.store_handle() = std::move(params_store);
				double result = api->calc_equation(request.id(), request.arg(), std::move(params));
				response.set_result(result);
				writer.Finish(response, grpc::Status::OK, this);
				std::cout << "Call Equation Request successful!" << std::endl;
				break;
			}
			case FINISH:
			{
				new EquationCallData(service,cq, api);
				delete this;
				break;
			}
		}
	}
	catch(const std::exception& e)
	{
		status = FINISH;
		std::cerr << "Error in EquationCallData: " << e.what() << std::endl;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new EquationCallData(service, cq,api);
		delete this;
	}
}
TasksService::LinkedCallData::LinkedCallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks> api):
	TasksService::CallData(service, cq, api), writer(&context)
{
	proceed(true);
}
void TasksService::LinkedCallData::proceed(bool ok)
{
	if (!ok)
	{
		delete this;
		return;
	}
	try 
	{
		switch(status)
		{
			case CREATE:
			{
				status = PROCESS;
				service->RequestCallLinked(&context, &request, &writer, cq, cq, this);
				break;
			}
			case PROCESS:
			{
				status = FINISH;
				std::vector<double> params_store(request.params().begin(), request.params().end());
				matrix<double> params(request.size_row(),request.size_col());
				*params.store_handle() = std::move(params_store);
				double result = api->calc_linked(request.id(), request.arg(), std::move(params));
				response.set_result(result);
				writer.Finish(response, grpc::Status::OK, this);
				std::cout << "Call Linked Request successful!" << std::endl;
				break;
			}
			case FINISH:
			{
				new LinkedCallData(service, cq, api);
				delete this;
				break;
			}
		}
	}
	catch(const std::exception& e)
	{
		status = FINISH;
		std::cerr << "Error in LinkedCallData: " << e.what() << std::endl;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new LinkedCallData(service, cq, api);
		delete this;
	}
}
TasksService::ParamSCallData::ParamSCallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks> api):
	TasksService::CallData(service, cq, api), writer(&context)
{
	proceed(true);
}
void TasksService::ParamSCallData::proceed(bool ok)
{
	if (!ok)
	{
		delete this;
		return;
	}
	try 
	{
		switch(status)
		{
			case CREATE:
			{
				status = PROCESS;
				service->RequestCallParamS(&context, &request, &writer, cq,cq,this);
				break;
			}
			case PROCESS:
			{
				status = FINISH;
				std::vector<double> x_store(request.x().begin(), request.x().end());
				std::vector<double> u_store(request.u().begin(), request.u().end());
				std::vector<double> optim_u_store(request.optim_u().begin(), request.optim_u().end());
				matrix<double> x(request.x_size_row(), request.x_size_col());
				*x.store_handle() = std::move(x_store);
				matrix<double> u(request.u_size_row(), request.u_size_col());
				*u.store_handle() = std::move(u_store);
				matrix<double> optim_u(request.optim_u_size_row(), request.optim_u_size_col());
				*optim_u.store_handle() = std::move(optim_u_store);
				double result = api->calc_param_s(std::move(x), std::move(u), std::move(optim_u));
				response.set_result(result);
				writer.Finish(response, grpc::Status::OK, this);
				std::cout << "Call Param S Request successful!" << std::endl;
				break;
			}
			case FINISH:
			{
				new ParamSCallData(service, cq, api);
				delete this;
				break;
			}
		}
	}
	catch(const std::exception& e)
	{
		status = FINISH;
		std::cerr << "Error in ParamSCallData: " << e.what() << std::endl;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new ParamSCallData(service,cq,api);
		delete this;
	}
}
void TasksService::handle_rpcs()
{
	void* tag;
	bool ok;
	while(!shutdown)
	{
		if (!cq || !cq->Next(&tag, &ok))
			break;
		auto* call_data = static_cast<CallData*>(tag);
		call_data->proceed(ok);
	}
}
void TasksService::run()
{
	for (int i = 0; i < thread_num; i++)
	{
		new EquationCallData(&service, cq.get(), api);
		new LinkedCallData(&service, cq.get(), api);
		new ParamSCallData(&service, cq.get(), api);
	}
	for (int i = 0; i < thread_num; i++)
		worker_thread.emplace_back(std::thread([this]{handle_rpcs();}));
}
void TasksService::close()
{
	shutdown = true;
	server->Shutdown();
	cq->Shutdown();
	void* tag;
	bool ok;
	while(cq->Next(&tag, &ok))
	{
		auto* call_data = static_cast<CallData*>(tag);
		delete call_data;
	}
	for (auto& th : worker_thread)
	{
		if (th.joinable())
			th.join();
	}
	worker_thread.clear();
}
TasksService::~TasksService() 
{
	close();
}
TasksService::TasksService(std::string& conf_file)
{
	try 
	{
		std::ifstream file(conf_file);
		if (file.is_open())
		{
			std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
			file.close();
			json conf_json = json::parse(buffer);
			thread_num = conf_json["TasksServer"]["worker_threads"].get<int>();
			std::string addr = absl::StrCat(conf_json["TasksServer"]["host"].get<std::string>(), ":", conf_json["TasksServer"]["port"].get<std::string>());
			grpc::ServerBuilder builder;
			cq = builder.AddCompletionQueue();
			api = std::make_shared<Tasks>();
			builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
			builder.AddChannelArgument(GRPC_ARG_USE_LOCAL_SUBCHANNEL_POOL, 0);
			builder.RegisterService(&service);
			server = builder.BuildAndStart();
			std::cout << "Server listening on " << addr << std::endl;
		}
		else 
		{
			file.close();
			throw std::runtime_error("Error of open config file!");
		}
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
void TasksService::add_equation(Equation value)
{
	api->add_equation(std::move(value));
}
void TasksService::add_linked(Equation value)
{
	api->add_linked(std::move(value));
}
void TasksService::set_param_s(ParamS value)
{
	api->set_param_s(std::move(value));
}
