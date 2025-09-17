#ifndef TASKS_H
#define TASKS_H
#include "IncludeFiles.h"
class Tasks 
{
	private:
		std::vector<Equation> equation;
		std::vector<Equation> linked;
		ParamS param_s;

	public:
		Tasks() = default;
		~Tasks() = default;
		void add_equation(Equation value);
		void add_linked(Equation value);
		void set_param_s(ParamS value);
		double calc_equation(int idx, double arg, matrix<double> params);
		double calc_linked(int idx, double arg, matrix<double> params);
		double calc_param_s(matrix<double> x, matrix<double> u0, matrix<double> optim_u);
};
#endif
