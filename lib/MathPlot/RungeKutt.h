#ifndef RUNGEKUTT_H
#define RUNGEKUTT_H
#include "interpolation.h"
#include "matrix.h"
#include <optional>
#include "function_wrapper.h"
#include <memory>
struct rk_params
{
	std::shared_ptr<std::vector<FunctionWrapper<double(double,matrix<double>)>>> equations = nullptr;
	std::shared_ptr<std::vector<interpol_param>> param_func = nullptr;
	std::vector<std::pair<size_t,size_t>> param_func_indeces;
	double t0 = 0.0;
	double t1 = 0.0;
	std::shared_ptr<std::vector<double>> u0 = nullptr;
	double step = 0.0;
	bool dir = true;
	size_t dim_var = 1;
	size_t index_var = 0;
	rk_params() = default;
	~rk_params() = default;
};
using RkRef = std::optional<std::reference_wrapper<rk_params>>;
matrix<double> runge_kutt(RkRef args);
#endif
