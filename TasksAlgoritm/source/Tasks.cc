#include "Tasks.h"
Tasks::Tasks() {
	f_builder = std::make_unique<ExpressionBuilder<double, int, double, matrix<double>&, matrix<double>&, matrix<double>&>>();
	fc_builder = std::make_unique<ExpressionBuilder<double, int, matrix<double>&, matrix<double>&>>();
	eq_builder = std::make_unique<ExpressionBuilder<double, double, matrix<double>>>();
	auto oper_plus = FunctionWrapper<double(double, double)>{[](auto x, auto y) { return x + y; }};
	auto oper_minus = FunctionWrapper<double(double, double)>{[](auto x, auto y) { return x - y; }};
	auto oper_mult = FunctionWrapper<double(double, double)>{[](auto x, auto y) { return x * y; }};
	auto oper_divide = FunctionWrapper<double(double, double)>{[](auto x, auto y) { return x / y; }};
	auto oper_pow = FunctionWrapper<double(double, double)>{[](auto x, auto y) { return std::pow(x,y); }};
	auto func_sin = FunctionWrapper<double(double)>{[](auto x) { return std::sin(x); }};
	auto func_cos = FunctionWrapper<double(double)>{[](auto x) { return std::cos(x); }};
	auto unary_minus = FunctionWrapper<double(double)> {[](auto x) { return -x; }};
	auto is_free_member = [](char c) { return std::isdigit(c); };
	auto to_free_member = [](const char* c) { return std::stod(c); };
	f_builder->set_is_free_member(is_free_member);
	f_builder->set_to_free_member(to_free_member);
	f_builder->add_first_prior_bin_oper('+', oper_plus);
	f_builder->add_first_prior_bin_oper('-', oper_minus);
	f_builder->add_second_prior_bin_oper('*', oper_mult);
	f_builder->add_second_prior_bin_oper('/', oper_divide);
	f_builder->add_third_prior_bin_oper('^', oper_pow);
	f_builder->add_unary_oper('-', unary_minus);
	f_builder->add_function('s',"sin",func_sin);
	f_builder->add_function('c', "cos", func_cos);
	fc_builder->set_is_free_member(is_free_member);
	fc_builder->set_to_free_member(to_free_member);
	fc_builder->add_first_prior_bin_oper('+', oper_plus);
	fc_builder->add_first_prior_bin_oper('-', oper_minus);
	fc_builder->add_second_prior_bin_oper('*', oper_mult);
	fc_builder->add_second_prior_bin_oper('/', oper_divide);
	fc_builder->add_third_prior_bin_oper('^', oper_pow);
	fc_builder->add_unary_oper('-', unary_minus);
	fc_builder->add_function('s', "sin", func_sin);
	fc_builder->add_function('c', "cos", func_cos);
	eq_builder->set_is_free_member(is_free_member);
	eq_builder->set_to_free_member(to_free_member);
	eq_builder->add_first_prior_bin_oper('+', oper_plus);
	eq_builder->add_first_prior_bin_oper('-', oper_minus);
	eq_builder->add_second_prior_bin_oper('*', oper_mult);
	eq_builder->add_second_prior_bin_oper('/', oper_divide);
	eq_builder->add_third_prior_bin_oper('^', oper_pow);
	eq_builder->add_function('s', "sin", func_sin);
	eq_builder->add_function('c', "cos", func_cos);
}
void Tasks::make_builders_table(int dim) {
	fc_builder->clear_variables();
	eq_builder->clear_variables();
	f_builder->clear_variables();
	for (int i = 0; i < dim; i++) {
		std::string xkey = std::move(absl::StrCat("x", i + 1));
		std::string ukey = std::move(absl::StrCat("u", i + 1));
		std::string psikey = std::move(absl::StrCat("psi", i + 1));
		eq_builder->add_variable(xkey, FunctionWrapper<double(double, matrix<double>)>{[idx = i](auto, auto params) { return params(idx,0); }});
		eq_builder->add_variable(ukey, FunctionWrapper<double(double, matrix<double>)>{[idx = i](auto, auto params) { return params(idx,1); }});
		eq_builder->add_variable(psikey, FunctionWrapper<double(double, matrix<double>)>{[idx= i](auto, auto params) { return params(idx,2); }});
		fc_builder->add_variable(xkey, FunctionWrapper<double(int, matrix<double>&, matrix<double>&)>{[idx = i](auto index, auto x, auto) { return x(index, idx); }});
		fc_builder->add_variable(ukey, FunctionWrapper<double(int, matrix<double>&, matrix<double>&)>{[idx = i](auto index, auto, auto u) { return u(index, idx); }});
		f_builder->add_variable(xkey, FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
		[idx = i](auto index, auto, auto x, auto, auto) { return x(index, idx); }});
		f_builder->add_variable(ukey, FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
		[idx = i](auto index, auto s, auto, auto u, auto optim_u) { return s * (optim_u(index, idx) - u(index, idx)) + u(index, idx); }});
	}
	eq_builder->add_variable("t", FunctionWrapper<double(double, matrix<double>)>{[](auto t, auto) { return t; }});
	fc_builder->add_variable("t", FunctionWrapper<double(int, matrix<double>&, matrix<double>&)>{[](auto index, auto x, auto) { return x(index, 0); }});
	f_builder->add_variable("t", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](auto index, auto, auto x, auto, auto) { return x(index, 0); }});
}
void Tasks::add_equations(std::vector<std::string> input) {
	for (auto& eq : input) {
		auto val = eq_builder->build(eq);
		equation.push_back(std::move(val));
	}
}
void Tasks::add_linked(std::vector<std::string> input) {
	for (auto& eq : input) {
		auto val = eq_builder->build(eq);
		linked.push_back(std::move(val));
	}
}
void Tasks::set_param_s(std::string& input) {
	auto func = f_builder->build(input);
	auto functional = [func = func](matrix<double> x, matrix<double> u, matrix<double> optim_u) -> double {
		auto minimize_func = [func = func](double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) -> double {
			matrix<double> grid(x.size_row(), 2);
			grid.set_column(0, x.make_column_acceptor(0));
			for (int i = 0; i < x.size_row(); i++)
				grid(i,1) = func(i,s,x,u,optim_u);
			return SimpsonIntegral(grid);
		};
		GoldenRatio optimizer(std::move(minimize_func), std::move(x), std::move(u), std::move(optim_u));
		return optimizer.minimum(0,1);
	};
	param_s = std::move(functional);
}
void Tasks::add_functional(std::string& input) {
	auto func = fc_builder->build(input);
	auto value = [func = func](matrix<double> x, matrix<double> u) -> double {
		matrix<double> grid(x.size_row(), 2);
		grid.set_column(0, x.make_column_acceptor(0));
		for (int i = 0; i < x.size_row(); i++)
			grid(i,1) = func(i,x,u);
		return SimpsonIntegral(grid);
	};
	functional = std::move(value);
}
double Tasks::calc_equation(int idx, double arg, matrix<double> param) {
	return equation[idx](arg, std::move(param));
}
double Tasks::calc_linked(int idx, double arg, matrix<double> param) {
	return linked[idx](arg, std::move(param));
}
double Tasks::calc_param_s(matrix<double> x, matrix<double> u0, matrix<double> optim_u) {
	return param_s(std::move(x), std::move(u0), std::move(optim_u));
}
double Tasks::calc_functional(matrix<double> x, matrix<double> u) {
	return functional(std::move(x), std::move(u));
}
void Tasks::clear_task() {
	equation.clear();
	linked.clear();
}
