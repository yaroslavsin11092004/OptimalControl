#include "FunctionalBuilder.h"
bool FunctionalBuilder::is_number(std::string& token) {
	for (int i = 0; i < token.size(); i++) {
		if (!(std::isdigit(token[i]) || token[i] == '.')) 
			return false;
	}
	return true;
}
FunctionalBuilder::FunctionalBuilder() {
	store.insert_or_assign("x1", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx,1); }});
	store.insert_or_assign("x2", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx,2); }});
	store.insert_or_assign("x3", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx,3); }});
	store.insert_or_assign("x4", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx,4); }});
	store.insert_or_assign("x5", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx,5); }});
	store.insert_or_assign("x6", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx,6); }});
	store.insert_or_assign("u1", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return s * (optim_u(idx,1) - u(idx,1)) + u(idx,1); }});
	store.insert_or_assign("u2", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return s * (optim_u(idx,2) - u(idx,2)) + u(idx,2); }});
	store.insert_or_assign("u3", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return s * (optim_u(idx,3) - u(idx,3)) + u(idx,3); }});
	store.insert_or_assign("u4", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return s * (optim_u(idx,4) - u(idx,4)) + u(idx,4); }});
	store.insert_or_assign("u5", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return s * (optim_u(idx,5) - u(idx,5)) + u(idx,5); }});
	store.insert_or_assign("u6", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return s * (optim_u(idx,6) - u(idx,6)) + u(idx,6); }});
	store.insert_or_assign("t", FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
	[](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return x(idx, 0); }});
}
FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)> FunctionalBuilder::build(const char* input) {
	tokenizer.set_input_ptr(input);
	auto tokens = tokenizer.tokenize();
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>> operands;
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "+" || t == "-" || t == "/" || t == "*" || t == "sin" || t == "cos" || t == "^")
			operators.push(std::move(t));
		else if (is_number(t)){
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<double(int, double, matrix<double>&, matrix<double>&, matrix<double>&)>{
			[val](int idx, double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) { return val; }});
		}
		else if (t == ")") {
			auto op = operators.top();
			operators.pop();
			if (op == "+") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::plus<double>{}, r));
			}
			else if (op == "-") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::minus<double>{}, r));
			}
			else if (op == "*") { 
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::multiplies<double>{}, r));
			}
			else if (op == "/") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::divides<double>{}, r));
			}
			else if (op == "^") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](double x, double y) { return std::pow(x,y); }, r));
			}
			else if (op == "sin") {
				auto val = operands.top();
				operands.pop();
				auto oper = FunctionWrapper<double(double)>{[](double x) { return std::sin(x); }};
				operands.push(oper.compose(val));
			}
			else if (op == "cos") {
				auto val = operands.top();
				operands.pop();
				auto oper = FunctionWrapper<double(double)>{[](double x) { return std::cos(x); }};
				operands.push(oper.compose(val));
			}
		}
		else operands.push(store.at(t));
	}
	return operands.top();
}
