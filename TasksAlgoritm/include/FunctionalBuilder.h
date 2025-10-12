#ifndef FUNCTIONAL_BUILDER_H
#define FUNCTIONAL_BUILDER_H
#include "Tokenizer.h"
#include "Transformer.h"
class FunctionalBuilder {
	private:
		using FunctionalSignature = double(int, double, matrix<double>&, matrix<double>&, matrix<double>&);
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<FunctionalSignature>> store;
		bool is_number(std::string& token);
	public:
		FunctionalBuilder();
		FunctionWrapper<FunctionalSignature> build(std::string& input);
};
#endif
