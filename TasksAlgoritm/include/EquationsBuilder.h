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
		std::unique_ptr<Transformer<std::string::const_iterator>> transformer;
	public:
		EquationBuilder() { transformer = std::make_unique<Transformer<std::string::const_iterator>>(); };
		void make_store_table(int dim);
		FunctionWrapper<EquationSignature> build(std::string& input);
};
#endif
