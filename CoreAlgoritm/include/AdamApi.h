#ifndef ADAM_H
#define ADAM_H
#include "IncludeFiles.h"
class AdamApi
{
	private:
		std::shared_ptr<net::io_context> ioc;
		std::string host;
		int port;
		net::thread_pool pool{4};
		void call_set_global_params(double learning_rate, double epsilon, int epochs);
		std::vector<double> call_adam(std::vector<double> args, std::vector<double> params);
	public:
		AdamApi(std::reference_wrapper<std::string> config_file);
		void set_global_params(double learning_rate, double epsilon, int epochs);
		std::vector<double> adam(std::vector<double> args, std::vector<double> params);
};
#endif
