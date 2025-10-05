#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H
#include <concepts>
#include <functional>
template<typename Signature>
class FunctionWrapper;
template<typename R, typename... Args>
class FunctionWrapper<R(Args...)> {
	private:
		std::function<R(Args...)> func;
	public:
		template<typename F>
		requires std::invocable<R, Args...> &&
		std::convertible_to<std::invoke_result<F,Args...>,R>
		FunctionWrapper(F f) : func(f) {}
		FunctionWrapper() = default;
		FunctionWrapper(const std::function<R(Args...)>& f) : func(f) {}
		FunctionWrapper(std::function<R(Args...)>&& f) : func(std::move(f)){}
		R operator()(Args... args) const {
			return func(args...);
		}
		template<typename NewR, typename... NewArgs>
		auto compose(const FunctionWrapper<NewR(NewArgs...)> g) -> FunctionWrapper<R(NewArgs...)> {
			return FunctionWrapper<R(NewArgs...)>{
				[func = this->func, g](NewArgs... args) -> R {
					return func(g(args...));
				}};
		}
		template<typename G, typename H>
		auto compose(G g, H h) -> FunctionWrapper<R(typename G::argument_type, typename H::argument_type)> {
			return FunctionWrapper<R(typename G::argument_type, typename H::argument_type)> {
				[func = this->func, g,h](auto x, auto y) -> R {
					return func(g(x), h(y));
				}
			};
		}
		template<typename Op, typename OtherR, typename OtherArgs>
		requires std::invocable<Op, OtherR, OtherArgs>
		auto combine(Op op, FunctionWrapper<OtherR(OtherArgs)> other) const {
			using CommonType = std::invoke_result_t<Op,R,OtherR>;
			return FunctionWrapper<CommonType(Args..., OtherArgs)> {
				[func = this->func, op, other](auto x, auto y) -> CommonType {
					return op(func(x), other(y));
				}
			};
		}
		template<typename Op>
		auto combine(Op op, FunctionWrapper<R(Args...)> other) {
			return FunctionWrapper<R(Args...)> {
				[func = this-> func, op, other](Args... args) {
					return op(func(args...), other(args...));
				}
			};
		}
};
#endif
