#ifndef OPTIMAL_CONTROL_H
#define OPTIMAL_CONTROL_H
#include "TasksApiRpc.h"
#include "AdamApiRpc.h"
class OptimalControl 
{
	private:
		std::shared_ptr<TasksApiRpc> tasks_api;
		std::shared_ptr<AdamApiRpc> adam_api;
		
		double matrix_metrics(matrix<double>& val1, matrix<double>& val2);
	public:
		OptimalControl(std::string& conf_file);
		~OptimalControl() = default;

		std::pair<matrix<double>, matrix<double>> successive_approximation(std::vector<double> x0, double t0, double t1,double t_step, double delta, std::vector<double> u0);
		void adam_params(double learning_rate, std::vector<double> u_left, std::vector<double> u_right, int epochs);
		void task(std::vector<std::string> equations, std::vector<std::string> linked, std::string functional);
		double functional_value(matrix<double> x, matrix<double> u);
		void set_hamilton(std::string hamilton, int dim);
};
#endif
