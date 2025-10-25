#ifndef FUNC_CALC_BUILDER_H
#define FUNC_CALC_BUILDER_H
#include "IncludeFiles.h"
#include "Tokenizer.h"
#include "Transformer.h"
class FuncCalcBuilder {
	private:
		using FuncCalcSignature = double(int, matrix<double>&, matrix<double>&);
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<FuncCalcSignature>> store;
		bool is_number(std::string& token);
		std::unique_ptr<Transformer<std::string::const_iterator>> transformer;
	public:
		FuncCalcBuilder() { transformer = std::make_unique<Transformer<std::string::const_iterator>>(); }
		void make_store_table(int dim);
		FunctionWrapper<FuncCalcSignature> build(std::string& input);
};
#endif
