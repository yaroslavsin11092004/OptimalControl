#ifndef ADAM_SERVICE_H
#define ADAM_SERVICE_H
#include "IncludeFiles.h"
#include "Adam.h"
class AdamService final 
{
	private:
		class CallData 
		{
			private:
				enum CallStatus { CREATE, PROCESS, FINISH };
				adam_api::AdamApiService::AsyncService* service;
				grpc::ServerCompletionQueue* cq;
				grpc::ServerContext context;
				Adam* adam;

				adam_api::GlobalParamsRequest set_params_request;
				adam_api::EmptyResponse set_params_response;
				grpc::ServerAsyncResponseWriter<adam_api::EmptyResponse> set_params_writer;

				adam_api::OptimizeRequest optimize_request;
				adam_api::OptimizeResponse optimize_response;
				grpc::ServerAsyncResponseWriter<adam_api::OptimizeResponse> optimize_writer;

				CallStatus status;
				bool is_optimize_call;
			public:
				CallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, Adam* adam, bool is_optimize);
				void proceed(bool ok);
		};
		void handle_rpcs();
		std::unique_ptr<grpc::Server> server;
		adam_api::AdamApiService::AsyncService service;
		std::unique_ptr<grpc::ServerCompletionQueue> cq;
		std::unique_ptr<Adam> adam;
		std::vector<std::thread> worker_thread;
		std::atomic<bool> shutdown{false};
	public:
		AdamService(const std::string& address, Hamilton func);
		~AdamService();
		void run();
		void close();
};
#endif
