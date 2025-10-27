#include "Adam.h"
Adam::Adam() {
	builder = std::make_unique<ExpressionBuilder<torch::Tensor, std::vector<torch::Tensor>&, std::vector<torch::Tensor>&>>();
	builder->set_to_free_member([](const char* c) { return torch::tensor(std::stod(c)); });
	builder->set_is_free_member([](const char c) { return std::isdigit(c); });
	builder->add_first_prior_bin_oper('+', FunctionWrapper<torch::Tensor(torch::Tensor, torch::Tensor)>{[](auto x, auto y) { return x + y; }});
	builder->add_first_prior_bin_oper('-', FunctionWrapper<torch::Tensor(torch::Tensor, torch::Tensor)>{[](auto x, auto y) { return x - y; }});
	builder->add_second_prior_bin_oper('*', FunctionWrapper<torch::Tensor(torch::Tensor, torch::Tensor)>{[](auto x, auto y) { return x * y; }});
	builder->add_second_prior_bin_oper('/', FunctionWrapper<torch::Tensor(torch::Tensor, torch::Tensor)>{[](auto x, auto y) { return x / y; }});
	builder->add_third_prior_bin_oper('^', FunctionWrapper<torch::Tensor(torch::Tensor, torch::Tensor)>{[](auto x, auto y) { return torch::pow(x,y); }});
	builder->add_unary_oper('-', FunctionWrapper<torch::Tensor(torch::Tensor)>{[](auto x) { return -x; }});
	builder->add_function('s', "sin", FunctionWrapper<torch::Tensor(torch::Tensor)>{[](auto x) { return torch::sin(x); }});
	builder->add_function('c', "cos", FunctionWrapper<torch::Tensor(torch::Tensor)>{[](auto x) { return torch::cos(x); }});
}
void Adam::set_hamilton(std::string func, int dim) {
	builder->clear_variables();
	for (int i = 0; i < dim; i++) {
		std::string xkey = std::move(absl::StrCat("x", i + 1));
		std::string ukey = std::move(absl::StrCat("u", i + 1));
		std::string psikey = std::move(absl::StrCat("psi", i + 1));
		builder->add_variable(std::move(xkey), FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
		[idx = i + 1](auto, auto params) { return params[idx]; }});
		builder->add_variable(std::move(ukey), FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
		[idx = i](auto args, auto) { return args[idx]; }});
		builder->add_variable(std::move(psikey), FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
		[idx = i + 1, dim](auto, auto params) { return params[idx + dim]; }});
	}
	builder->add_variable("t", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](auto, auto params) { return params[0]; }});
	hamilton = std::move(builder->build(func));
}
std::vector<double> Adam::optimize()
{
	size_t dim_theta = edges.size_row();
	std::vector<torch::Tensor> args;
	std::vector<double> theta;
	std::vector<torch::Tensor> param;
	auto left_edge_acceptor = edges.make_column_acceptor(0);
	auto right_edge_acceptor = edges.make_column_acceptor(1);
	for (int i = 0; i < dim_theta; i++) theta.push_back(left_edge_acceptor(i));
	for (auto i : params) param.push_back(torch::tensor({i}));
	for (int i = 0; i < dim_theta; i++) { args.push_back(torch::tensor({theta[i]}, torch::requires_grad())); m.push_back(0); v.push_back(0); }
	for (int epoch = 0; epoch < epochs; epoch++)
	{
		(-1 * hamilton(args, param)).backward();
		for (int i = 0; i < dim_theta; i++)
		{
			auto grad = args[i].grad();
			if (grad.defined() && grad.numel() > 0)
			{
				double grad_val = grad.item<double>();
				m[i] = beta1 * m[i] + (1 - beta1) * grad_val;
				v[i] = beta2 * v[i] + (1 - beta2) * pow(grad_val,2);
				theta[i] += (-learning_rate * m[i]) / (sqrt(v[i] + epsilon));
				args[i].data() = torch::tensor({theta[i]}, torch::requires_grad());
			}
			if (args[i].grad().defined())
				args[i].grad().zero_();
		}
	}
	for (int i = 0; i < dim_theta; i++)
	{
		if (theta[i] < left_edge_acceptor(i))
			theta[i] = left_edge_acceptor(i);
		if (theta[i] > right_edge_acceptor(i))
			theta[i] = right_edge_acceptor(i);
	}
	return theta;
}
