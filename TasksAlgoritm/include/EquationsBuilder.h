#ifndef EQUATION_BUILDER_H
#define EQUATION_BUILDER_H
#include "Tokenizer.h"
#include <absl/container/flat_hash_map.h>
class EquationBuilder {
	private:
		tokenizer tokenizer;
		absl::flat_hash_map<std::string, FunctionWrapper<double(double, matrix<double>)>> store;
		bool is_number(std::string& token);
	public:
		EquationBuilder();
		FunctionWrapper<double(double, matrix<double>)> build(const char* input);
};
#endif
