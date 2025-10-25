#include "Tasks.h"
Tasks::Tasks() {
	fc_builder = std::make_unique<FuncCalcBuilder>();
	eq_builder = std::make_unique<EquationBuilder>();
	f_builder = std::make_unique<FunctionalBuilder>();
}
void Tasks::make_builders_table(int dim) {
	fc_builder->make_store_table(dim);
	eq_builder->make_store_table(dim);
	f_builder->make_store_table(dim);
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
