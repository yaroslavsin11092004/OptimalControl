#include "HamiltonBuilder.h"
bool HamiltonBuilder::is_number(std::string& token) {
	for (int i = 0; i < token.size(); i++) {
		if (!(std::isdigit(token[i]) || token[i] == '.'))
			return false;
	}
	return true;
}
void HamiltonBuilder::make_store_table(int dim) {
	store.clear();
	for (int i = 0; i < dim; i++) {
		std::string num = std::to_string(i + 1);
		std::string xkey = absl::StrCat("x", num);
		std::string ukey = absl::StrCat("u", num);
		std::string psikey = absl::StrCat("psi", num);
		store.insert_or_assign(std::move(xkey), FunctionWrapper<HamiltonSignature>{[ind = i + 1](auto, auto params) { return params[ind]; }});
		store.insert_or_assign(std::move(ukey), FunctionWrapper<HamiltonSignature>{[i](auto args, auto) { return args[i]; }});
		store.insert_or_assign(std::move(psikey), FunctionWrapper<HamiltonSignature>{[ind = i + 1, dim](auto, auto params) { return params[ind + dim]; }});
	}
	store.insert_or_assign("t", FunctionWrapper<HamiltonSignature>{[](auto, auto params) { return params[0]; }});
}
FunctionWrapper<HamiltonBuilder::HamiltonSignature> HamiltonBuilder::build(std::string& input) {
	auto tr_input = transform_expression(input, *transformer);
	tokenizer.set_input_ptr(tr_input.c_str());
	auto tokens = tokenizer.tokenize();
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<HamiltonSignature>> operands;
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "+" || t == "-" || t == "/" || t == "*" || t == "sin" || t == "cos" || t == "^")
			operators.push(t);
		else if (is_number(t)){
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<HamiltonSignature>{[val](auto, auto){return torch::tensor(val); }});
		}
		else if (t == ")") {
			if (operators.empty() || operands.empty()) continue;
			auto op = operators.top();
			operators.pop();
			if (op == "+") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](torch::Tensor x, torch::Tensor y) { return x + y; }, r));
			}
			else if (op == "-") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](torch::Tensor x, torch::Tensor y) { return x - y; }, r));
			}
			else if (op == "*") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](torch::Tensor x, torch::Tensor y) { return x * y;}, r));
			}
			else if (op == "/") { 
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](torch::Tensor x, torch::Tensor y) { return x / y;}, r));
			}
			else if (op == "^") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](torch::Tensor x, torch::Tensor y) { return torch::pow(x,y);}, r));
			}
			else if (op == "sin") {
				auto args = operands.top();
				operands.pop();
				auto oper = FunctionWrapper<torch::Tensor(torch::Tensor)>{[](torch::Tensor x) { return torch::sin(x); }};
				operands.push(oper.compose(args));
			}
			else if (op == "cos") {
				auto args = operands.top();
				operands.pop();
				auto oper = FunctionWrapper<torch::Tensor(torch::Tensor)>{[](torch::Tensor x) { return torch::cos(x); }};
				operands.push(oper.compose(args));
			}
		}
		else operands.push(store.at(t));
	}
	return operands.top();
}
