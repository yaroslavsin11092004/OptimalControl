#include "EquationsBuilder.h"
bool EquationBuilder::is_number(std::string& token) {
	for (int i = 0; i < token.size(); i++) {
		if (!(std::isdigit(token[i]) || token[i] == '.'))
			return false;
	}
	return true;
}
EquationBuilder::EquationBuilder() {
	store.insert_or_assign("x1", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(0,0); }});
	store.insert_or_assign("x2", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(1,0); }});
	store.insert_or_assign("x3", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(2,0); }});
	store.insert_or_assign("x4", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(3,0); }});
	store.insert_or_assign("x5", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(4,0); }});
	store.insert_or_assign("x6", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(5,0); }});
	store.insert_or_assign("u1", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(0,1); }});
	store.insert_or_assign("u2", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(1,1); }});
	store.insert_or_assign("u3", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(2,1); }});
	store.insert_or_assign("u4", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(3,1); }});
	store.insert_or_assign("u5", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(4,1); }});
	store.insert_or_assign("u6", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(5,1); }});
	store.insert_or_assign("psi1", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(0,2); }});
	store.insert_or_assign("psi2", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(1,2); }});
	store.insert_or_assign("psi3", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(2,2); }});
	store.insert_or_assign("psi4", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(3,2); }});
	store.insert_or_assign("psi5", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(4,2); }});
	store.insert_or_assign("psi6", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return params(5,2); }});
	store.insert_or_assign("t", FunctionWrapper<double(double, matrix<double>)>{[](double t, matrix<double> params) { return t; }});
}
FunctionWrapper<double(double, matrix<double>)> EquationBuilder::build(const char* input) {
	tokenizer.set_input_ptr(input);
	auto tokens = tokenizer.tokenize();
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<double(double, matrix<double>)>> operands;
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "*" || t == "+" || t == "-" || t == "/" || t == "^" || t == "sin" || t == "cos")
			operators.push(std::move(t));
		else if (is_number(t)) {
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<double(double, matrix<double>)>{[val](double t, matrix<double> params) { return val; }});
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
