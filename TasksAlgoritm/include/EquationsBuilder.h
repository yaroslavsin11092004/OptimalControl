#ifndef EQUATION_BUILDER_H
#define EQUATION_BUILDER_H
#include "Tokenizer.h"
#include "Transformer.h"
class EquationBuilder {
	private:
		using EquationSignature = double(double, matrix<double>);
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<EquationSignature>> store;
		bool is_number(std::string& token);
	public:
		EquationBuilder();
		FunctionWrapper<EquationSignature> build(std::string& input);
};
#endif
