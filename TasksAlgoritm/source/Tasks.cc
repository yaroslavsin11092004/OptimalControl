#include "Tasks.h"
void Tasks::add_equation(Equation value)
{
	equation.push_back(std::move(value));
}
void Tasks::add_linked(Equation value)
{
	linked.push_back(std::move(value));
}
void Tasks::set_param_s(ParamS value)
{
	param_s = std::move(value);
}
double Tasks::calc_equation(int idx, double arg, matrix<double> param)
{
	return equation[idx](arg, std::move(param));
}
double Tasks::calc_linked(int idx, double arg, matrix<double> param)
{
	return linked[idx](arg, std::move(param));
}
double Tasks::calc_param_s(matrix<double> x, matrix<double> u0, matrix<double> optim_u)
{
	return param_s(std::move(x), std::move(u0), std::move(optim_u));
}
