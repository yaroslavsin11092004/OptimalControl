#include "AdamService.h"

AdamService::AdamService(const std::string& address, Hamilton func):
	adam(std::make_unique<Adam>())
{
	adam->set_hamilton(func);
	grpc::ServerBuilder builder;
	builder.AddChannelArgument(GRPC_ARG_USE_LOCAL_SUBCHANNEL_POOL, 0);
	builder.AddListeningPort(address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	cq = builder.AddCompletionQueue();
	server = builder.BuildAndStart();
	std::cout << "Server listening on " << address << std::endl;
}
AdamService::~AdamService()
{
	close();
}
void AdamService::run()
{
	for (int i = 0; i < 4; i++)
	{
		new CallData(&service, cq.get(), adam.get(),false);
		new CallData(&service, cq.get(), adam.get(), true);
	}
	for (int i = 0; i < 4; i++)
		worker_thread.emplace_back([this]{handle_rpcs();});
}
void AdamService::close()
{
	if (!shutdown)
	{
		shutdown = true;
		server->Shutdown();
		cq->Shutdown();
		for (auto& th : worker_thread)
		{
			if (th.joinable())
				th.join();
		}
		 void* tag;
      bool ok;
      while (cq->Next(&tag, &ok)) {
      auto* call_data = static_cast<CallData*>(tag);
    	delete call_data;
    }
	}
}
void AdamService::handle_rpcs()
{
	void* tag;
	bool ok;
	while(cq->Next(&tag, &ok))
	{
		if (shutdown) break;
		auto* call_data = static_cast<CallData*>(tag);
		call_data->proceed(ok);
	}
}
AdamService::CallData::CallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, Adam* adam, bool is_optimize):
	service(service), cq(cq), adam(adam), set_params_writer(&context), optimize_writer(&context), status(CREATE), is_optimize_call(is_optimize)
{
	proceed(true);
}
void AdamService::CallData::proceed(bool ok)
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
				service->RequestSetGlobalParams(&context, &set_params_request, &set_params_writer, cq, cq, this);
				service->RequestOptimize(&context, &optimize_request,&optimize_writer, cq,cq,this);
				break;
			}
			case PROCESS:
			{
				if (context.IsCancelled())
				{
					status = FINISH;
					if (is_optimize_call)
						optimize_writer.FinishWithError(grpc::Status::CANCELLED, this);
					else 
						set_params_writer.FinishWithError(grpc::Status::CANCELLED, this);
					break;
				}
				if (is_optimize_call)
				{
					std::vector<double> left_edge(optimize_request.left_edge().begin(), optimize_request.left_edge().end());
					std::vector<double> right_edge(optimize_request.right_edge().begin(), optimize_request.right_edge().end());
					std::vector<double> params(optimize_request.params().begin(), optimize_request.params().end());
					matrix<double> edges(left_edge.size(),2);
					edges.set_column(0,std::move(left_edge));
					edges.set_column(1, std::move(right_edge));
					adam->set_edges(std::move(edges));
					adam->set_params(std::move(params));
					auto result = adam->optimize();
					for (auto& val : result) optimize_response.add_result(val);
					status = FINISH;
					optimize_writer.Finish(optimize_response, grpc::Status::OK, this);
				}
				else 
				{
					adam->set_epochs(set_params_request.epochs());
					adam->set_learning_rate(set_params_request.learning_rate());
					status = FINISH;
					set_params_writer.Finish(set_params_response, grpc::Status::OK, this);
				}
				break;
			}
			case FINISH:
			{
				if (!is_optimize_call) {
        	new CallData(service, cq, adam, false);
        } else {
        	new CallData(service, cq, adam, true);
        }
				delete this;
				break;
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception in CallData::proceed: " << e.what() << std::endl;
		status = FINISH;
		if (is_optimize_call)
			optimize_writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		else 
			set_params_writer.FinishWithError(grpc::Status(grpc::INTERNAL, e.what()), this);
		if (!is_optimize_call) {
    	new CallData(service, cq, adam, false);
		} 
		else {
    	new CallData(service, cq, adam, true);
    }
		delete this;
	}
}
