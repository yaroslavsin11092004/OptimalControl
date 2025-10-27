#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H
#include <absl/container/flat_hash_map.h>
#include <absl/container/inlined_vector.h>
#include "function_wrapper.h"
template<typename R, typename ...Args> 
class ExpressionBuilder {
	private:
		struct unary_operator {
			char code;
			FunctionWrapper<R(R)> value;
		};
		struct binary_operator{
			char code;
			FunctionWrapper<R(R,R)> value;
		};
		struct f {
			char code;
			std::string name;
			FunctionWrapper<R(R)> value;
		};
		absl::flat_hash_map<std::string, FunctionWrapper<R(Args...)>> store;
		absl::InlinedVector<unary_operator, 4> unary_operators;
		absl::InlinedVector<binary_operator, 4> first_prior_bin_operators;
		absl::InlinedVector<binary_operator, 4> second_prior_bin_operators;
		absl::InlinedVector<binary_operator, 4> third_prior_bin_operators;
		absl::InlinedVector<f, 8> func;
		std::function<bool(char)> is_free_member;
		std::function<R(const char*)> to_free_member;

		bool is_delim(char c) {
			return c == ' ';
		}
		bool is_op(char c) {
			for (int i = 0; i < unary_operators.size(); i++)
				if (c == unary_operators[i].code)
					return true;
			for (int i = 0; i < first_prior_bin_operators.size(); i++)
				if (c == first_prior_bin_operators[i].code)
					return true;
			for (int i = 0; i < second_prior_bin_operators.size(); i++)
				if (c == second_prior_bin_operators[i].code)
					return true;
			for (int i = 0; i < third_prior_bin_operators.size(); i++)
				if (c == third_prior_bin_operators[i].code)
					return true;
			return false;
		}
		bool is_func(std::string& s) {
			for (int i = 0; i < func.size(); i++)
				if (s == func[i].name)
					return true;
			return false;
		}
		int priority(char op) {
			if (op < 0)
				return 4;
			for (int i = 0; i < first_prior_bin_operators.size(); i++)
				if (op == first_prior_bin_operators[i].code)
					return 1;
			for (int i = 0; i < second_prior_bin_operators.size(); i++)
				if (op == second_prior_bin_operators[i].code)
					return 2;
			for (int i = 0; i < third_prior_bin_operators.size(); i++)
				if (op == third_prior_bin_operators[i].code)
					return 3;
			return -1;
		}
		void process_op(absl::InlinedVector<FunctionWrapper<R(Args...)>,16>& st, char op) {
			if (op < 0) {
				if (st.size() < 1)
					throw std::runtime_error("Not enough operands for unary operator");
				auto l = st.back();
				st.pop_back();
				for (int i = 0; i < unary_operators.size(); i++)
					if (-op == unary_operators[i].code) {
						st.push_back(unary_operators[i].value.compose(l));
						return;
					}
				for (int i = 0; i < func.size(); i++)
					if (-op == func[i].code) {
						st.push_back(func[i].value.compose(l));
						return;
					}
			} else {
				if (st.size() < 2)
					throw std::runtime_error("Not enough operands for binary operator");
				auto r = st.back();
				st.pop_back();
				auto l = st.back();
				st.pop_back();
				for (int i = 0; i < first_prior_bin_operators.size(); i++)
					if (op == first_prior_bin_operators[i].code) {
						st.push_back(l.combine(first_prior_bin_operators[i].value, r));
						return;
					}
				for (int i = 0; i < second_prior_bin_operators.size(); i++)
					if (op == second_prior_bin_operators[i].code) {
						st.push_back(l.combine(second_prior_bin_operators[i].value, r));
						return;
					}
				for (int i = 0; i < third_prior_bin_operators.size(); i++)
					if (op == third_prior_bin_operators[i].code) {
						st.push_back(l.combine(third_prior_bin_operators[i].value, r));
						return;
					}
			}
		}
	public:
		ExpressionBuilder() = default;
		~ExpressionBuilder() = default;
		void add_variable(std::string name, FunctionWrapper<R(Args...)> callback) {
			store.insert_or_assign(name, callback);
		}
		void add_first_prior_bin_oper(char code, FunctionWrapper<R(R,R)> callback) {
			if (4 >= first_prior_bin_operators.size())
				first_prior_bin_operators.push_back({code, callback});
		}
		void add_second_prior_bin_oper(char code,FunctionWrapper<R(R,R)> callback) {
			if (4 >= second_prior_bin_operators.size())
				second_prior_bin_operators.push_back({code, callback});
		}
		void add_third_prior_bin_oper(char code, FunctionWrapper<R(R,R)> callback) {
			if (4 >= third_prior_bin_operators.size())
				third_prior_bin_operators.push_back({code, callback});
		}
		void add_unary_oper(char code, FunctionWrapper<R(R)> callback) {
			if (4 >= unary_operators.size()) 
				unary_operators.push_back({code,callback});
		}
		void add_function(char code, std::string name, FunctionWrapper<R(R)> callback) {
			if (8 >= func.size())
				func.push_back({code, name, callback});
		}
		void set_is_free_member(std::function<bool(char)> callback) {
			is_free_member = callback;
		}
		void set_to_free_member(std::function<R(const char*)> callback) {
			to_free_member = callback;
		}
		FunctionWrapper<R(Args...)> build(std::string& s) {
			bool may_unary = true;
			absl::InlinedVector<FunctionWrapper<R(Args...)>, 16> st;
			absl::InlinedVector<char, 16> op;
			for (int i = 0; i < s.size(); i++) {
				if (!is_delim(s[i])) {
					if (s[i] == '(') {
						op.push_back('(');
						may_unary = true;
					} else if (s[i] == ')') {
						while(!op.empty() && op.back() != '(') {
							process_op(st, op.back());
							op.pop_back();
						}
						op.pop_back();
						may_unary = false;
					} else if (is_op(s[i])) {
						char cur_op = s[i];
						bool is_un;
						for (int k = 0; k < unary_operators.size(); k++)
							if (unary_operators[k].code == cur_op) {
								cur_op = -cur_op;
								break;
							}
						while(!op.empty() && op.back() != '(' && priority(op.back()) >= priority(cur_op)) {
							process_op(st, op.back());
							op.pop_back();
						}
						op.push_back(cur_op);
						may_unary = true;
					} else {
						std::string token;
						while(i < s.size() && (std::isalnum(s[i]) || s[i] == '.'))
							token.push_back(s[i++]);
						i--;
						if (store.contains(token)) {
							st.push_back(store.at(token));
							may_unary = false;
						} else if (is_free_member(token[0])) {
							try {
								R val = to_free_member(token.c_str());
								auto oper = FunctionWrapper<R(Args...)>{[val](Args...) { return val; }};
								st.push_back(oper);
								may_unary = false;
							} catch(const std::exception& e) {
								throw std::runtime_error(e.what());
							}
						} else if (is_func(token)) {
							char func_code = 0;
							for (int i = 0; i < func.size(); i++)
								if (token == func[i].name) {
									func_code = -func[i].code;
									break;
								}
							op.push_back(func_code);
							may_unary = true;
						}
					}
				}
			}
			while(!op.empty()) {
				process_op(st,op.back());
				op.pop_back();
			}
			return st.back();
		}
		void clear_all() {
			unary_operators.clear();
			first_prior_bin_operators.clear();
			second_prior_bin_operators.clear();
			third_prior_bin_operators.clear();
			store.clear();
			func.clear();
		}
		void clear_unary() {
			unary_operators.clear();
		}
		void clear_first_bin() {
			first_prior_bin_operators.clear();
		}
		void clear_second_bin() {
			second_prior_bin_operators.clear();
		}
		void clear_third_bin() {
			third_prior_bin_operators.clear();
		}
		void clear_variables() {
			store.clear();
		}
		void clear_func() {
			func.clear();
		}
};
#endif
