#include "Adam.h"
using namespace std;
int main()
{
	Hamilton func = [](vector<torch::Tensor>& args, vector<torch::Tensor>& params)
	{
		return torch::pow(args[0],2) - params[0] * torch::pow(args[1],3);
	};
	matrix<double> edges(2,2);
	vector<double> params = {1};
	edges.set_row(0,{1.0e-8, 5});
	edges.set_row(1,{1.0e-8, 5});
	Adam adam;
	adam.set_hamilton(func);
	adam.set_epochs(50);
	adam.set_learning_rate(0.1);
	adam.set_edges(std::move(edges));
	adam.set_params(std::move(params));
	auto res = adam.optimize();
	for (auto& val : res) cout << val << " ";
	cout << endl;
	return 0;
}
