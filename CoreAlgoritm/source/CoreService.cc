#include "CoreService.h"
#include "core.grpc.pb.h"
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
CoreService::OptimizeParamsCallData::OptimizeParamsCallData(core_api::CoreOptimalControlService::AsyncService* service_, grpc::ServerCompletionQueue* cq_, std::shared_ptr<OptimalControl> api_):
	CoreService::CallData(service_, cq_, api_),  writer(&context) {
	proceed(true);
}
void CoreService::OptimizeParamsCallData::proceed(bool ok) {
	if (!ok) {
		delete this;
		return;
	}
	try {
		switch(status) {
			case CREATE: {
										 status = PROCESS;
										 service->RequestSetOptimizerParams(&context, &request, &writer, cq,cq,this);
										 break;
									 }
			case PROCESS: {
											status = FINISH;
											std::vector<double> left_edge(request.left_edge().begin(), request.left_edge().end());
											std::vector<double> right_edge(request.right_edge().begin(), request.right_edge().end());
											int epochs = request.epochs();
											double learning_rate = request.learning_rate();
											api->adam_params(learning_rate, std::move(left_edge), std::move(right_edge), epochs);
											writer.Finish(response, grpc::Status::OK, this);
											std::cout << "Call SetOptimizeParamsRequest successful!" << std::endl;
											break;
										}
			case FINISH: {
									 		new OptimizeParamsCallData(service, cq, api);
											delete this;
											break;
									 }
		}
	} catch(const std::exception& e) {
		status = FINISH;
		std::cout << "Error of SetOptimizeParamsRequest::" << e.what() << std::endl;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new OptimizeParamsCallData(service, cq, api);
		delete this;
	}
}
CoreService::OptimalCallData::OptimalCallData(core_api::CoreOptimalControlService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<OptimalControl> api):
	CoreService::CallData(service, cq, api), writer(&context) {
		proceed(true);
}
void CoreService::OptimalCallData::proceed(bool ok) {
	if (!ok) {
		delete this;
		return;
	}
	try {
		switch(status) {
			case CREATE: {
										 status = PROCESS;
										 service->RequestCalcOptimalTask(&context, &request, &writer, cq,cq,this);
										 break;
									 }
			case PROCESS: {
											status = FINISH;
											std::vector<std::string> equations(request.equations().begin(), request.equations().end());
											std::vector<std::string> linked(request.linked().begin(), request.linked().end());
											std::string functional(request.functional());
											std::string hamilton(request.hamilton());
											api->set_hamilton(std::move(hamilton), equations.size());
											api->task(std::move(equations), std::move(linked), std::move(functional));
											std::vector<double> x0(request.x0().begin(), request.x0().end());
											std::vector<double> u0(request.u0().begin(), request.u0().end());
											auto result = api->successive_approximation(std::move(x0), request.t0(), request.t1(), request.tstep(), request.delta(), std::move(u0));
											double func_value = api->functional_value(result.first, result.second);
											response.set_functional_value(func_value);
											for (auto& val : *result.first.store_handle()) response.add_optim_path(val);
											for (auto& val : *result.second.store_handle()) response.add_optim_control(val);
											response.set_optim_control_size_row(result.second.size_row());
											response.set_optim_control_size_col(result.second.size_col());
											response.set_optim_path_size_row(result.first.size_row());
											response.set_optim_path_size_col(result.first.size_col());
											writer.Finish(response, grpc::Status::OK, this);
											std::cout << "Call OptimalTaskRequest successful!" << std::endl;
											break;
										}
			case FINISH: {
										 new OptimalCallData(service, cq, api);
										 delete this;
										 break;
									 }
		}
	} catch(const std::exception& e) {
		status = FINISH;
		std::cout << "Error of OptimalRequest::" << e.what() << std::endl;
		writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		new OptimalCallData(service, cq, api);
		delete this;
	}
}
void CoreService::handle_rpcs() {
	void* tag;
	bool ok;
	while(!shutdown) {
		if (!cq || !cq->Next(&tag, &ok))
			break;
		auto req = static_cast<CallData*>(tag);
		req->proceed(ok);
	}
}
CoreService::CoreService(std::string& conf_file) {
	try {
		std::ifstream file(conf_file);
		if (file.is_open())  {
			std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
			file.close();
			json conf_data = json::parse(std::move(buffer));
			thread_num = conf_data["CoreServer"]["worker_threads"].get<int>();
			std::string addr = std::move(absl::StrCat(conf_data["CoreServer"]["host"].get<std::string>(), ":", conf_data["CoreServer"]["port"].get<std::string>()));
			grpc::ServerBuilder builder;
			cq = builder.AddCompletionQueue();
			api = std::make_shared<OptimalControl>(conf_file);
			builder.AddChannelArgument(GRPC_ARG_HTTP2_MAX_PINGS_WITHOUT_DATA, 0);
			builder.AddChannelArgument(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);
			builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
			builder.RegisterService(&service);
			builder.AddChannelArgument(GRPC_ARG_USE_LOCAL_SUBCHANNEL_POOL, 0);
			server = builder.BuildAndStart();
			std::cout << "Server listening on " << addr << std::endl;
		} else {
			file.close();
			throw std::runtime_error("Error of open config file!");
		}
	} catch(const std::exception& e) {
		throw std::runtime_error(std::move(absl::StrCat("Internal Error::", e.what())));
	}
}
void CoreService::run() {
	for (int i = 0; i < thread_num; i++) {
		new OptimizeParamsCallData(&service, cq.get(), api);
		new OptimalCallData(&service, cq.get(), api);
	}
	for (int i = 0; i < thread_num; i++)
		worker_thread.emplace_back(std::thread([this]{ handle_rpcs();}));
}
void CoreService::close() {
	shutdown = true;
	server->Shutdown();
	cq->Shutdown();
	void* tag;
	bool ok;
	while(cq->Next(&tag, &ok)) {
		auto req = static_cast<CallData*>(tag);
		delete req;
	}
	for (int i = 0; i < thread_num; i++) {
		if (worker_thread[i].joinable())
			worker_thread[i].join();
	}
	worker_thread.clear();
}
CoreService::~CoreService() {
	close();
}
