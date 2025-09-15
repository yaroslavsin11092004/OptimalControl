#ifndef OPTIMAL_CONTROL_H
#define OPTIMAL_CONTROL_H
#include "TasksApiRpc.h"
#include "AdamApiRpc.h"
class OptimalControl 
{
	private:
		std::shared_ptr<TasksApiRpc> tasks_api;
		std::shared_ptr<AdamApiRpc> adam_api;
		
		double col_of_matrix_norm(auto acceptor, size_t dim);
		double matrix_metrics(matrix<double>& val1, matrix<double>& val2);
	public:
		OptimalControl(std::string& conf_file);
		~OptimalControl() = default;

		std::pair<matrix<double>, matrix<double>> successive_approximation(std::vector<double> x0, double t0, double t1,double t_step, double delta, std::vector<double> u0);
		void adam_params(double learning_rate, std::vector<double> u_left, std::vector<double> u_right, int epochs);
};
#endif
