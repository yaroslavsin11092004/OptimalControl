#ifndef HAMILTON_BUILDER_H
#define HAMILTON_BUILDER_H
#include "Tokenizer.h"
class HamiltonBuilder {
	private:
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>> store;
		bool is_number(std::string& token);
	public:
		HamiltonBuilder(int dim);
		FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)> build(const char* input);
};
#endif
