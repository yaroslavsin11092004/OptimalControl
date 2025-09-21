#ifndef GOLDEN_RATIO_H
#define GOLDEN_RATIO_H
#include "IncludeFiles.h"
using IntFunctional = std::function<double(double, matrix<double>&, matrix<double>&, matrix<double>&)>;
class GoldenRatio 
{
	private:
		matrix<double> x;
		matrix<double> u;
		matrix<double> optim_u;
		IntFunctional function;
		int far_point(std::vector<double>& points, int min_idx);
		int min_idx(std::vector<double>& points);
	public:
		GoldenRatio(IntFunctional value, matrix<double> x, matrix<double> u, matrix<double> optim_u):
			function(value), x(std::move(x)), u(std::move(u)), optim_u(std::move(optim_u)){}
		double minimum(double left, double right);
};
#endif
