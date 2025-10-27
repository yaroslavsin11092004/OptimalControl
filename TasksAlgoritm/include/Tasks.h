#ifndef TASKS_H
#define TASKS_H
#include "IncludeFiles.h"
#include "GoldenRatio.h"
#include <MathPlot/function_builder.h>
class Tasks 
{
	private:
		std::vector<Equation> equation;
		std::vector<Equation> linked;
		ParamS param_s;
		std::function<double(matrix<double>, matrix<double>)> functional;
		std::unique_ptr<ExpressionBuilder<double, int, double, matrix<double>&, matrix<double>&, matrix<double>&>> f_builder;
		std::unique_ptr<ExpressionBuilder<double, double, matrix<double>>> eq_builder;
		std::unique_ptr<ExpressionBuilder<double, int, matrix<double>&, matrix<double>&>> fc_builder;
	public:
		Tasks();
		~Tasks() = default;
		void make_builders_table(int dim);
		void add_equations(std::vector<std::string> input);
		void add_linked(std::vector<std::string> input);
		void set_param_s(std::string& input);
		void add_functional(std::string& input);
		double calc_equation(int idx, double arg, matrix<double> params);
		double calc_linked(int idx, double arg, matrix<double> params);
		double calc_param_s(matrix<double> x, matrix<double> u0, matrix<double> optim_u);
		double calc_functional(matrix<double> x, matrix<double> u);
		void clear_task();
};
#endif
