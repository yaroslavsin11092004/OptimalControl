#ifndef HAMILTON_BUILDER_H
#define HAMILTON_BUILDER_H
#include "Tokenizer.h"
#include "Transformer.h"
class HamiltonBuilder {
	private:
		using HamiltonSignature = torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&);
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<HamiltonSignature>> store;
		bool is_number(std::string& token);
		std::unique_ptr<Transformer<std::string::const_iterator>> transformer;
	public:
		HamiltonBuilder() { transformer = std::make_unique<Transformer<std::string::const_iterator>>(); }
		void make_store_table(int dim);
		FunctionWrapper<HamiltonSignature> build(std::string& input);
};
#endif
