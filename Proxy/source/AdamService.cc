#include "AdamService.h"
#include "adam_api.grpc.pb.h"
AdamService::SetParamsCallData::SetParamsCallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api): AdamService::CallData(service, cq, api), writer(&context)
{
	proceed(true);
}
void AdamService::SetParamsCallData::proceed(bool ok)
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
				service->RequestSetGlobalParams(&context, &request, &writer, cq,cq,this);
				break;
			}
			case PROCESS:
			{
				std::vector<double> left_edge(request.left_edge().begin(), request.left_edge().end());
				std::vector<double> right_edge(request.right_edge().begin(), request.right_edge().end());
				api->set_global_params(request.learning_rate(), std::move(left_edge), std::move(right_edge), request.epochs());
				status = FINISH;
				writer.Finish(response, grpc::Status::OK, this);
				break;
			}
			case FINISH:
			{
				new SetParamsCallData(service,cq,api);
				delete this;
				break;
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception in SetParamsCallData: " << e.what() << std::endl;
		status = FINISH;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new SetParamsCallData(service, cq, api);
		delete this;
	}
}
AdamService::HamiltonCallData::HamiltonCallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api) : AdamService::CallData(service, cq, api), writer(&context) { proceed(true); }
void AdamService::HamiltonCallData::proceed(bool ok) {
	if (!ok) {
		delete this;
		return;
	}
	try {
		switch(status) {
			case CREATE: {
										 status = PROCESS;
										 service->RequestHamilton(&context, &request,&writer, cq,cq,this);
										 break;
									 }
			case PROCESS: {
											status = FINISH;
											std::string func(request.hamilton().begin(), request.hamilton().end());
											api->set_hamilton(std::move(func), request.dimension());
											writer.Finish(response, grpc::Status::OK, this);
											break;
										}
			case FINISH: {
										 new HamiltonCallData(service, cq, api);
										 delete this;
										 break;
									 }
		}
	}
	catch(const std::exception& e) {
		status = FINISH;
		std::cerr << "Set hamilton call failed with error::" << e.what() << std::endl;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new HamiltonCallData(service, cq, api);
		delete this;
	}
}
AdamService::OptimizeCallData::OptimizeCallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi>api): AdamService::CallData(service, cq,api), writer(&context)
{
	proceed(true);
}
void AdamService::OptimizeCallData::proceed(bool ok)
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
				service->RequestOptimize(&context, &request, &writer, cq,cq,this);
				break;
			}
			case PROCESS:
			{
				std::vector<double> params(request.params().begin(), request.params().end());
				auto res = api->adam(std::move(params));
				for (auto& i : res) response.add_result(i);
				writer.Finish(response, grpc::Status::OK, this);
				status = FINISH;
				break;
			}
			case FINISH:
			{
				new OptimizeCallData(service, cq, api);
				delete this;
				break;
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception in OptimizeCallData: " << e.what() << std::endl;
		status = FINISH;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new OptimizeCallData(service, cq, api);
		delete this;
	}
}
void AdamService::handle_rpcs()
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
void AdamService::run()
{
	for (int i = 0; i < num_thread; i++)
	{
		new SetParamsCallData(&service, cq.get(), api);
		new OptimizeCallData(&service, cq.get(), api);
		new HamiltonCallData(&service, cq.get(), api);
	}
	for (int i = 0; i < num_thread; i++)
		worker_thread.emplace_back(std::thread([this]{handle_rpcs();}));
}
void AdamService::close()
{
	if (!shutdown)
	{
		shutdown = true;
		server->Shutdown();
		cq->Shutdown();
		void* tag;
		bool ok;
		while(cq->Next(&tag, &ok))
		{
			auto* call = static_cast<CallData*>(tag);
			delete call;
		}
		for (auto& th : worker_thread)
    {
    	if (th.joinable()) {
      	th.join();
      }
    }
    worker_thread.clear();
	}
}
AdamService::AdamService(std::string& conf_path)
{
	try 
	{
		std::ifstream file(conf_path);
		if (file.is_open())
		{
			std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
			file.close();
			json conf_json = json::parse(buffer);
			api = std::make_shared<AdamApi>(conf_path);
			std::string address = absl::StrCat(conf_json["ProxyServer"]["host"].get<std::string>(), ":", conf_json["ProxyServer"]["port"].get<std::string>());
			num_thread = conf_json["ProxyServer"]["worker_threads"].get<int>();
			grpc::ServerBuilder builder;
			builder.AddChannelArgument(GRPC_ARG_USE_LOCAL_SUBCHANNEL_POOL, 0);
			cq = builder.AddCompletionQueue();
			builder.RegisterService(&service);
			builder.AddListeningPort(address, grpc::InsecureServerCredentials());
			server = builder.BuildAndStart();
			std::cout << "Server listening on " << address << std::endl;
		}
		else 
		{
			file.close();
			throw std::runtime_error("Error of open config file!");
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
AdamService::~AdamService()
{
	close();
}
