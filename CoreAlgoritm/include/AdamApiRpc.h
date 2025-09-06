#ifndef ADAM_API_RPC_H
#define ADAM_API_RPC_H
#include "IncludeFiles.h"
#include <absl/strings/str_cat.h>
#include "adam_api.grpc.pb.h"
#include <grpcpp/grpcpp.h>
class AdamApiRpc : public std::enable_shared_from_this<AdamApiRpc>
{
	protected:
		struct AsyncCallStateParams
		{
			grpc::ClientContext context;
			adam_api::GlobalParamsRequest request;
			adam_api::EmptyResponse response;
			grpc::Status status;
			std::promise<void> promise;
			std::unique_ptr<grpc::ClientAsyncResponseReader<adam_api::EmptyResponse>> reader;
		};
		struct AsyncCallStateAdam
		{
			grpc::ClientContext context;
			adam_api::OptimizeRequest request;
			adam_api::OptimizeResponse response;
			grpc::Status status;
			std::promise<std::vector<double>> promise;
			std::unique_ptr<grpc::ClientAsyncResponseReader<adam_api::OptimizeResponse>> reader;
		};
	private:
		std::unique_ptr<adam_api::AdamApiService::Stub> stub;
		net::thread_pool pool{4};
		std::optional<net::executor_work_guard<net::io_context::executor_type>> guard;
		std::shared_ptr<net::io_context> ioc;
		std::thread cq_thread;
		std::shared_ptr<grpc::CompletionQueue> cq;
		std::atomic<bool> shutdown{false};
		
		void run_completion_queue();

		net::awaitable<void> set_global_param_async(double learning_rate, double epsilon, int epochs);
		net::awaitable<std::vector<double>> adam_async(std::vector<double>& u_left, std::vector<double>& u_right, std::vector<double> params);
	public:
		AdamApiRpc(std::string& conf_file);
		~AdamApiRpc();
		void set_global_params(double learning_rate, double epsilon, int epochs);
		std::vector<double> adam(std::vector<double>& u_left, std::vector<double>& u_right, std::vector<double> params);

};
#endif
