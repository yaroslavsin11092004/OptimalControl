#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "IncludeFiles.h"
class tokenizer {
	private:
		const char* input;
		std::string parse_token();
		const std::string tokens[28] = { 
			"+", "-", "*", "/", "^", "sin", "cos", "(",")",
			"x1", "x2", "x3", "x4", "x5", "x6", 
			"u1", "u2", "u3", "u4", "u5", "u6",
			"psi1", "psi2", "psi3", "psi4", "psi5", "psi6",
			"t" };
	public:
		tokenizer() : input(nullptr) {};
		void set_input_ptr(const char* input_) { input = input_; }
		std::vector<std::string> tokenize();
};
#endif
