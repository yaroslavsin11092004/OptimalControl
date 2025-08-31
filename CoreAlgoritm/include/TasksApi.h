#ifndef TASKS_H
#define TASKS_H
#include "IncludeFiles.h"
class TasksApi : public std::enable_shared_from_this<TasksApi>
{
	private:
		std::string host;
		std::string port;
		std::optional<net::executor_work_guard<net::io_context::executor_type>> guard;
		net::thread_pool pool{4};
		std::shared_ptr<net::io_context> ioc;

		net::awaitable<double> call_equation(int id, double arg, matrix<double> params);
		net::awaitable<double> call_linked(int id, double arg, matrix<double> params);
		net::awaitable<double> call_param_s(matrix<double>& x, matrix<double>& u, matrix<double>& optim_u);
	public:
		TasksApi(std::string& conf_file);
		~TasksApi();
		
		std::shared_ptr<std::vector<Equation>> generate_equations(int dim);
		std::shared_ptr<std::vector<Equation>> generate_linked(int dim);
		double param_s(matrix<double>& x, matrix<double>& u, matrix<double>& optim_u);
};
#endif
