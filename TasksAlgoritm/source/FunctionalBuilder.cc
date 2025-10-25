#include "FunctionalBuilder.h"
bool FunctionalBuilder::is_number(std::string& token) {
	for (int i = 0; i < token.size(); i++) {
		if (!(std::isdigit(token[i]) || token[i] == '.')) 
			return false;
	}
	return true;
}
void FunctionalBuilder::make_store_table(int dim) {
	store.clear();
	for (int i = 0; i < dim; i++) {
		std::string num = std::to_string(i + 1);
		std::string xkey = absl::StrCat("x", num);
		std::string ukey = absl::StrCat("u", num);
		store.insert_or_assign(std::move(xkey), FunctionWrapper<FunctionalSignature>{ 
		[ind = i + 1](auto idx, auto, auto x, auto, auto) { return x(idx, ind); }});
		store.insert_or_assign(std::move(ukey), FunctionWrapper<FunctionalSignature>{ 
		[ind = i + 1](auto idx, auto s, auto, auto u, auto optim_u) { return s * (optim_u(idx, ind) - u(idx, ind)) + u(idx, ind); }});
	}
	store.insert_or_assign("t", FunctionWrapper<FunctionalSignature>{[](auto idx, auto, auto x, auto, auto) { return x(idx,0);}});
}
FunctionWrapper<FunctionalBuilder::FunctionalSignature> FunctionalBuilder::build(std::string& input) {
	std::string tr_input = transform_expression(input, *transformer);
	tokenizer.set_input_ptr(tr_input.c_str());
	auto tokens = tokenizer.tokenize();
	std::stack<std::string> operators;
	std::stack<FunctionWrapper<FunctionalSignature>> operands;
	for (auto& t : tokens) {
		if (t == "(") continue;
		else if (t == "+" || t == "-" || t == "/" || t == "*" || t == "sin" || t == "cos" || t == "^")
			operators.push(std::move(t));
		else if (is_number(t)){
			double val = std::stod(t.c_str());
			operands.push(FunctionWrapper<FunctionalSignature>{ [val](auto, auto, auto, auto, auto) { return val; }});
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
