#ifndef ADAM_SERVICE_H
#define ADAM_SERVICE_H
#include "IncludeFiles.h"
#include "AdamApi.h"
class AdamService final 
{
	private:
		class CallData 
		{
			public:
				enum CallStatus  {CREATE, PROCESS, FINISH};
				CallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api):
					service(service), cq(cq), api(api),status(CREATE) {}
				virtual void proceed(bool ok) = 0;
				virtual ~CallData() = default;
			protected:
				adam_api::AdamApiService::AsyncService* service;
				grpc::ServerCompletionQueue* cq;
				grpc::ServerContext context;
				std::shared_ptr<AdamApi> api;
				CallStatus status;
		};
		class SetParamsCallData : public CallData 
		{
			public:
				SetParamsCallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api);
				void proceed(bool ok) override;
			private:
				adam_api::GlobalParamsRequest request;
				adam_api::EmptyResponse response;
				grpc::ServerAsyncResponseWriter<adam_api::EmptyResponse> writer;
		};
		class OptimizeCallData : public CallData 
		{
			public:
				OptimizeCallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api);
				void proceed(bool ok) override;
			private:
				adam_api::OptimizeRequest request;
				adam_api::OptimizeResponse response;
				grpc::ServerAsyncResponseWriter<adam_api::OptimizeResponse> writer;
		};
		class HamiltonCallData : public CallData {
			private:
				adam_api::HamiltonRequest request;
				adam_api::EmptyResponse response;
				grpc::ServerAsyncResponseWriter<adam_api::EmptyResponse> writer;
			public:
				HamiltonCallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api);
				void proceed(bool ok) override;
		};
		void handle_rpcs();
		std::unique_ptr<grpc::Server> server;
		adam_api::AdamApiService::AsyncService service;
		int num_thread;
		std::vector<std::thread> worker_thread;
		std::atomic<bool> shutdown{false};
		std::unique_ptr<grpc::ServerCompletionQueue> cq;
		std::shared_ptr<AdamApi> api;
	public:
		AdamService(std::string& conf_path);
		~AdamService();
		void run();
		void close();
};
#endif
