#ifndef OPTIMAL_CONTROL_H
#define OPTIMAL_CONTROL_H
#include "TasksApiRpc.h"
#include "AdamApiRpc.h"
#include <MathPlot/DiffEvolution.h>
#include <absl/functional/any_invocable.h>
#include <absl/types/span.h>
class OptimalControl 
{
	private:
		std::shared_ptr<TasksApiRpc> tasks_api;
		std::shared_ptr<AdamApiRpc> adam_api;
		std::vector<double> uLeft;
		std::vector<double> uRight;
		std::vector<double> X0;
		double T0;
		double T1;
		double Tstep;
		double Delta;
		
		double matrix_metrics(matrix<double>& val1, matrix<double>& val2);
		double optimize_func(absl::Span<const double> x);
	public:
		struct Answer {
			matrix<double> optim_u;
			matrix<double> optim_x;
			double functiona_value;
		};
		OptimalControl(std::string& conf_file);
		~OptimalControl() = default;

		std::pair<matrix<double>, matrix<double>> successive_approximation(std::vector<double> x0, double t0, double t1,double t_step, double delta, std::vector<double> u0);
		void adam_params(double learning_rate, std::vector<double> u_left, std::vector<double> u_right, int epochs);
		void task(std::vector<std::string> equations, std::vector<std::string> linked, std::string functional);
		double functional_value(matrix<double> x, matrix<double> u);
		void set_hamilton(std::string hamilton, int dim);
		Answer evolution_approximation(std::vector<double> x0, double t0, double t1, double t_step, double delta);
};
#endif
