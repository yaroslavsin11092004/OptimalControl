#ifndef FUNCTIONAL_BUILDER_H
#define FUNCTIONAL_BUILDER_H
#include "Tokenizer.h"
class FunctionalBuilder {
	private:
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>> store;
		bool is_number(std::string& token);
	public:
		FunctionalBuilder();
		FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)> build(const char* input);
};
#endif
