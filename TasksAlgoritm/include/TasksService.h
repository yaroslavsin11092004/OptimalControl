#ifndef TASKS_SERVICE_H
#define TASKS_SERVICE_H
#include "Tasks.h"
class TasksService final 
{
	private:
		class CallData 
		{
			public:
				enum CallStatus { CREATE, PROCESS, FINISH };
				CallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks> api):
					service(service), cq(cq), api(api), status(CREATE) {}
				virtual void proceed(bool ok) = 0;
				virtual ~CallData() = default;
			protected:
				grpc::ServerCompletionQueue* cq;
				grpc::ServerContext context;
				std::shared_ptr<Tasks> api;
				tasks_api::TasksApiService::AsyncService* service;
				CallStatus status;
		};
		class EquationCallData : public CallData 
		{
			public:
				EquationCallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks>api);
				void proceed(bool ok) override;
			private:
				tasks_api::CallEquationRequest request;
				tasks_api::CallEquationResponse response;
				grpc::ServerAsyncResponseWriter<tasks_api::CallEquationResponse> writer;
		};
		class LinkedCallData : public CallData 
		{
			public:
				LinkedCallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks> api);
				void proceed(bool ok) override;
			private:
				tasks_api::CallLinkedRequest request;
				tasks_api::CallLinkedResponse response;
				grpc::ServerAsyncResponseWriter<tasks_api::CallLinkedResponse> writer;
		};
		class ParamSCallData : public CallData 
		{
			public:
				ParamSCallData(tasks_api::TasksApiService::AsyncService* service, grpc::ServerCompletionQueue* cq, std::shared_ptr<Tasks> api);
				void proceed(bool ok) override;
			private:
				tasks_api::CallParamSRequest request;
				tasks_api::CallParamSResponse response;
				grpc::ServerAsyncResponseWriter<tasks_api::CallParamSResponse> writer;
		};
		void handle_rpcs();
		std::unique_ptr<grpc::Server> server;
		std::unique_ptr<grpc::ServerCompletionQueue> cq;
		std::shared_ptr<Tasks> api;
		std::atomic<bool> shutdown{false};
		int thread_num;
		std::vector<std::thread> worker_thread;
		tasks_api::TasksApiService::AsyncService service;
	public:
		TasksService(std::string& conf_file);
		~TasksService();
		void run();
		void close();
		void add_equation(Equation value);
		void add_linked(Equation value);
		void set_param_s(ParamS value);
};
#endif
