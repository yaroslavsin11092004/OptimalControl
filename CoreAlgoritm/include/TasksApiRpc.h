#ifndef TASKS_API_RPC_H
#define TASKS_API_RPC_H
#include "IncludeFiles.h"
#include "tasks_api.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <absl/strings/str_cat.h>
class TasksApiRpc : public std::enable_shared_from_this<TasksApiRpc>
{
	protected:
		struct AsyncCallStateEquation 
		{
			grpc::ClientContext context;
			tasks_api::CallEquationRequest request;
			tasks_api::CallEquationResponse response;
			grpc::Status status;
			std::promise<double> promise;
			std::unique_ptr<grpc::ClientAsyncResponseReader<tasks_api::CallEquationResponse>> reader;
		};
		struct AsyncCallStateLinked
		{
			grpc::ClientContext context;
			tasks_api::CallLinkedRequest request;
			tasks_api::CallLinkedResponse response;
			grpc::Status status;
			std::promise<double> promise;
			std::unique_ptr<grpc::ClientAsyncResponseReader<tasks_api::CallLinkedResponse>> reader;
		};
		struct AsyncCallStateParamS
		{
			grpc::ClientContext context;
			tasks_api::CallParamSRequest request;
			tasks_api::CallParamSResponse response;
			grpc::Status status;
			std::promise<double> promise;
			std::unique_ptr<grpc::ClientAsyncResponseReader<tasks_api::CallParamSResponse>> reader;
		};
	private:
		std::unique_ptr<tasks_api::TasksApiService::Stub> stub;
		std::unique_ptr<net::thread_pool> pool;
		std::shared_ptr<net::io_context> ioc;
		std::optional<net::executor_work_guard<net::io_context::executor_type>> guard;
		std::thread cq_thread;
		std::shared_ptr<grpc::CompletionQueue> cq;
		std::atomic<bool> shutdown{false};

		void run_completion_queue();

		net::awaitable<double> call_equation_async(int id, double arg, matrix<double> params);
		net::awaitable<double> call_linked_async(int id, double arg, matrix<double> params);
		net::awaitable<double> call_param_s_async(matrix<double>& x, matrix<double>& u, matrix<double>& optim_u);
	public:
		TasksApiRpc(std::string& conf_file);
		~TasksApiRpc();
		std::shared_ptr<std::vector<Equation>> generate_equations(int dim);
		std::shared_ptr<std::vector<Equation>> generate_linked(int dim);
		double param_s(matrix<double>& x, matrix<double>& u, matrix<double>& optim_u);
};
#endif
