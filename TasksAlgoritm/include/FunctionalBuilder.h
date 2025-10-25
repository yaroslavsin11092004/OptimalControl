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
		std::unique_ptr<Transformer<std::string::const_iterator>> transformer;
	public:
		FunctionalBuilder() { transformer = std::make_unique<Transformer<std::string::const_iterator>>(); }
		void make_store_table(int dim);
		FunctionWrapper<FunctionalSignature> build(std::string& input);
};
#endif
