#include "HamiltonBuilder.h"
bool HamiltonBuilder::is_number(std::string& token) {
	for (int i = 0; i < token.size(); i++) {
		if (!(std::isdigit(token[i]) || token[i] == '.'))
			return false;
	}
	return true;
}
HamiltonBuilder::HamiltonBuilder(int dim) {
	store.insert_or_assign("x1", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[1]; }});
	store.insert_or_assign("x2", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[2]; }});
	store.insert_or_assign("x3", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[3]; }});
	store.insert_or_assign("x4", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[4]; }});
	store.insert_or_assign("x5", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[5]; }});
	store.insert_or_assign("x6", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[6]; }});
	store.insert_or_assign("u1", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return args[0]; }});
	store.insert_or_assign("u2", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return args[1]; }});
	store.insert_or_assign("u3", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return args[2]; }});
	store.insert_or_assign("u4", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return args[3]; }});
	store.insert_or_assign("u5", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return args[4]; }});
	store.insert_or_assign("u6", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return args[5]; }});
	store.insert_or_assign("psi1", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[dim](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[1 + dim]; }});
	store.insert_or_assign("psi2", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[dim](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[2 + dim]; }});
	store.insert_or_assign("psi3", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[dim](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[3 + dim]; }});
	store.insert_or_assign("psi4", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[dim](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[4 + dim]; }});
	store.insert_or_assign("psi5", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[dim](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[5 + dim]; }});
	store.insert_or_assign("psi6", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[dim](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[6 + dim]; }});
	store.insert_or_assign("t", FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{
	[](std::vector<torch::Tensor>& args, std::vector<torch::Tensor>& params) { return params[0]; }});
}
FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)> HamiltonBuilder::build(const char* input) {
	tokenizer.set_input_ptr(input);
	auto tokens = tokenizer.tokenize();
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>> operands;
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "+" || t == "-" || t == "/" || t == "*" || t == "sin" || t == "cos" || t == "^")
			operators.push(t);
		else if (is_number(t)){
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<torch::Tensor(std::vector<torch::Tensor>&, std::vector<torch::Tensor>&)>{[val](std::vector<torch::Tensor>&, std::vector<torch::Tensor>&){return torch::tensor(val); }});
		}
		else if (t == ")") {
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
