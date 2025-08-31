#ifndef ADAM_H
#define ADAM_H
#include "IncludeFiles.h"
class AdamApi : public std::enable_shared_from_this<AdamApi>
{
	private:
		std::shared_ptr<net::io_context> ioc;
		std::string host;
		std::string port;
		net::thread_pool pool{4};
		std::optional<net::executor_work_guard<net::io_context::executor_type>> guard;
		net::awaitable<void> call_set_global_params(double learning_rate, double epsilon, int epochs);
		net::awaitable<std::vector<double>> call_adam(std::vector<double>& left_edge, std::vector<double>& right_edge, std::vector<double> params);
	public:
		AdamApi(std::string& config_file);
		~AdamApi();
		void set_global_params(double learning_rate, double epsilon, int epochs);
		std::vector<double> adam(std::vector<double>& left_edge, std::vector<double>& right_edge, std::vector<double> params);
};
#endif
