#include "FuncCalcBuilder.h"
bool FuncCalcBuilder::is_number(std::string& input) {
	for (auto& t : input) {
		if (!(std::isdigit(t) || t == '.'))
			return false;
	}
	return true;
}
void FuncCalcBuilder::make_store_table(int dim) {
	store.clear();
	for (int i = 0; i < dim; i++) {
		std::string num = std::to_string(i + 1);
		std::string xkey = absl::StrCat("x", num);
		std::string ukey = absl::StrCat("u", num);
		store.insert_or_assign(std::move(xkey), FunctionWrapper<FuncCalcSignature>{[ind = i + 1](auto idx, auto x, auto) { return x(idx, ind); }});
		store.insert_or_assign(std::move(ukey), FunctionWrapper<FuncCalcSignature>{[ind = i + 1](auto idx, auto, auto u) { return u(idx, ind); }});
	}
	store.insert_or_assign("t", FunctionWrapper<FuncCalcSignature>{[](auto idx, auto x, auto) { return x(idx, 0); }});
}
FunctionWrapper<FuncCalcBuilder::FuncCalcSignature> FuncCalcBuilder::build(std::string& input) {
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<FuncCalcSignature>> operands;
	std::string tr_string = transform_expression(input, *transformer);
	tokenizer.set_input_ptr(tr_string.c_str());
	auto tokens = tokenizer.tokenize();
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "-" || t == "+" || t == "/" || t == "*" | t == "^" || t == "cos" || t == "sin")
			operators.push(std::move(t));
		else if (is_number(t)) {
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<FuncCalcSignature>{[val](auto,auto,auto) { return val; }});
		}
		else if (t == ")") {
			if (operands.empty() || operators.empty()) continue;
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
				operands.push(l.combine(std::minus<double>{},r));
			}
			else if (op == "*") { 
				auto r = operands.top();
				operands.pop();
				auto l = operands.top();
				operands.pop();
				operands.push(l.combine(std::multiplies<double>{},r));
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
				auto oper = FunctionWrapper<double(double)>{[](double x){return std::sin(x);}};
				operands.push(oper.compose(val));
			}
			else if (op == "cos") {
				auto val = operands.top();
				operands.pop();
				auto oper = FunctionWrapper<double(double)>{[](double x){return std::cos(x);}};
				operands.push(oper.compose(val));
			}
		}
		else operands.push(store.at(t));
	}
	return operands.top();
}
