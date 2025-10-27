#ifndef CORE_SERVICE_H
#define CORE_SERVICE_H
#include "IncludeFiles.h"
#include <absl/strings/str_cat.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/grpcpp.h>
#include "OptContrAlg.h"
#include "core.grpc.pb.h"
#include "core.pb.h"
class CoreService final {
	private:
		std::unique_ptr<grpc::Server> server;
		std::unique_ptr<grpc::ServerCompletionQueue> cq;
		std::shared_ptr<OptimalControl> api;
		std::atomic<bool> shutdown{false};
		int thread_num;
		std::vector<std::thread> worker_thread;
		core_api::CoreOptimalControlService::AsyncService service;

		void handle_rpcs();

		class CallData {
			public:
				enum CallStatus { CREATE, PROCESS, FINISH };
				CallData(core_api::CoreOptimalControlService::AsyncService* service_, grpc::ServerCompletionQueue* cq_, std::shared_ptr<OptimalControl> api_):
					service(service_), cq(cq_), api(api_), status(CREATE) {}
				virtual void proceed(bool) = 0;
				virtual ~CallData() = default;
			protected:
				core_api::CoreOptimalControlService::AsyncService* service;
				grpc::ServerCompletionQueue* cq;
				CallStatus status;
				grpc::ServerContext context;
				std::shared_ptr<OptimalControl> api;
		};
		class OptimizeParamsCallData : public CallData {
			public:
				OptimizeParamsCallData(core_api::CoreOptimalControlService::AsyncService*, grpc::ServerCompletionQueue*, std::shared_ptr<OptimalControl>);
				void proceed(bool) override;
			private:
				core_api::OptimizeParamsRequest request;
				core_api::OptimizeParamsResponse response;
				grpc::ServerAsyncResponseWriter<core_api::OptimizeParamsResponse> writer;
		};
		class OptimalCallData : public CallData {
			public:
				OptimalCallData(core_api::CoreOptimalControlService::AsyncService*, grpc::ServerCompletionQueue*, std::shared_ptr<OptimalControl>);
				void proceed(bool) override;
			private:
				core_api::OptimalRequest request;
				core_api::OptimalResponse response;
				grpc::ServerAsyncResponseWriter<core_api::OptimalResponse> writer;
		};
	public:
		CoreService(std::string&);
		~CoreService();
		void run();
		void close();
};
#endif
