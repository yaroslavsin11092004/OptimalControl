#include "Tokenizer.h"
std::string tokenizer::parse_token() {
	while(std::isspace(*input)) ++input;
	if (std::isdigit(*input)) {
		std::string buffer;
		while(std::isdigit(*input) || *input == '.') buffer.push_back(*input++);
		return std::move(buffer);
	}
	for (auto& t : tokens) {
		if (std::strncmp(input, t.c_str(), t.size()) == 0){
			input += t.size();
			return t;
		}
	}
	return "";
}
std::vector<std::string> tokenizer::tokenize() {
	std::vector<std::string> result;
	while(*input != '\0') result.push_back(parse_token());
	return std::move(result);
}
