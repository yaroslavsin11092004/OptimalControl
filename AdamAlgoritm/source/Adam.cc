#include "Adam.h"
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
