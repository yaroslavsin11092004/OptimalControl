#include "EquationsBuilder.h"
bool EquationBuilder::is_number(std::string& token) {
	for (int i = 0; i < token.size(); i++) {
		if (!(std::isdigit(token[i]) || token[i] == '.'))
			return false;
	}
	return true;
}
EquationBuilder::EquationBuilder() {
	store.insert_or_assign("x1", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(0,0); }});
	store.insert_or_assign("x2", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(1,0); }});
	store.insert_or_assign("x3", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(2,0); }});
	store.insert_or_assign("x4", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(3,0); }});
	store.insert_or_assign("x5", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(4,0); }});
	store.insert_or_assign("x6", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(5,0); }});
	store.insert_or_assign("u1", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(0,1); }});
	store.insert_or_assign("u2", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(1,1); }});
	store.insert_or_assign("u3", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(2,1); }});
	store.insert_or_assign("u4", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(3,1); }});
	store.insert_or_assign("u5", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(4,1); }});
	store.insert_or_assign("u6", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(5,1); }});
	store.insert_or_assign("psi1", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(0,2); }});
	store.insert_or_assign("psi2", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(1,2); }});
	store.insert_or_assign("psi3", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(2,2); }});
	store.insert_or_assign("psi4", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(3,2); }});
	store.insert_or_assign("psi5", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(4,2); }});
	store.insert_or_assign("psi6", FunctionWrapper<EquationSignature>{[](auto, auto params) { return params(5,2); }});
	store.insert_or_assign("t", FunctionWrapper<EquationSignature>{[](auto t, auto) { return t; }});
}
FunctionWrapper<EquationBuilder::EquationSignature> EquationBuilder::build(std::string& input) {
	std::string tr_input = transform_expression(input);
	tokenizer.set_input_ptr(tr_input.c_str());
	auto tokens = tokenizer.tokenize();
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<EquationSignature>> operands;
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "*" || t == "+" || t == "-" || t == "/" || t == "^" || t == "sin" || t == "cos")
			operators.push(std::move(t));
		else if (is_number(t)) {
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<EquationSignature>{[val](auto, auto) { return val; }});
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
				operands.push(l.combine(std::plus<double>{}, r));
			}
			else if (op == "-") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::minus<double>{}, r));
			}
			else if (op == "/") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::divides<double>{}, r));
			}
			else if (op == "*") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::multiplies<double>{}, r));
			}
			else if (op == "^") {
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine([](double x, double y) { return std::pow(x,y); }, r));
			}
			else if (op == "cos") {
				auto val = operands.top();
				operands.pop();
				FunctionWrapper<double(double)> oper{[](double x) { return std::cos(x); }};
				operands.push(oper.compose(val));
			}
			else if (op == "sin") {
				auto val = operands.top();
				operands.pop();
				FunctionWrapper<double(double)> oper{[](double x) { return std::sin(x); }};
				operands.push(oper.compose(val));
			}
		}
		else operands.push(store.at(t));
	}
	return operands.top();
}
