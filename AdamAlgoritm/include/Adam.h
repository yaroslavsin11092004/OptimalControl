#ifndef OPTIMIZE_H
#define OPTIMIZE_H
#include "IncludeFiles.h"
#include <MathPlot/function_builder.h>
using Hamilton = FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params)>;
class Adam
{
	private:
		Hamilton hamilton;
		int epochs = 200;
		double learning_rate = 1.0e-1;
		matrix<double> edges;
		std::vector<double> params;
		std::vector<double> m;
		std::vector<double> v;
		double beta1 = 0.9;
		double beta2 = 0.999;
		double epsilon = 1.0e-8;
		std::unique_ptr<ExpressionBuilder<torch::Tensor, std::vector<torch::Tensor>&, std::vector<torch::Tensor>&>> builder;
	public:
		Adam();
		~Adam() = default;
		std::vector<double> optimize();
		void set_learning_rate(double value) { learning_rate = value; }
		void set_epochs(int value) { epochs = value; }
		void set_hamilton(std::string func, int dim);
		void set_params(std::vector<double> value) { params = std::move(value); }
		void set_edges(matrix<double> value) { edges = std::move(value); }
};
#endif
