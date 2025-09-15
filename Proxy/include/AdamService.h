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
				enum CallType {SET_PARAMS, OPTIMIZE};
				CallData(adam_api::AdamApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<AdamApi> api, CallType type):
					service(service), cq(cq), api(api),status(CREATE),call_type(type){}
				virtual void proceed(bool ok) = 0;
				virtual ~CallData() = default;
				CallType get_type() const { return call_type; }
			protected:
				adam_api::AdamApiService::AsyncService* service;
				grpc::ServerCompletionQueue* cq;
				grpc::ServerContext context;
				std::shared_ptr<AdamApi> api;
				CallStatus status;
				CallType call_type;
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
