#pragma once
#include <memory>
#include <functional>
#include <limits>
#include <map>
#include "random.h"

namespace zee {
	typedef std::size_t size_t;
	typedef size_t delegate_handle_t;

namespace impl {
	template<typename FuncT>
	struct is_delegate_type : 
		std::conditional_t <
			std::is_function<FuncT>::value && std::is_void<decltype(FuncT()())>::value, 
			std::true_type, 
			std::false_type
		>
	{
	};

	template<typename RetT, typename ...Args>
	struct delegate_invoker {
		typedef std::function<RetT(Args...)> func_type;

		void bind(RetT(*new_func)(Args...)) {
			func_ = new_func;
			check_func_ = std::function<bool(void)>{};
		}

		template<typename LambdaT>
		void bind(LambdaT&& new_lambda) {
			func_ = new_lambda;
			check_func_ = std::function<bool(void)>{};
		}

		template<typename ClassT>
		void bind(const std::shared_ptr<ClassT>& sp, RetT(ClassT::* new_func)(Args...)) {
			std::weak_ptr<ClassT> wp = sp;
			check_func_ = [wp]() ->bool {
				return !wp.expired();
			};

			func_ = [this, wp, new_func](Args... args) -> RetT {
				if (!wp.expired()) {
					(wp.lock().get()->*new_func)(std::forward<Args>(args)...);
				} else {
					reset();
				}
			};
		}

		void reset() {
			func_type temp;
			func_.swap(temp);
		}

		RetT invoke(Args&&...args) {
			return func_(std::forward<Args>(args)...);
		}

		bool is_valid() const {
			if (func_) {
				if (check_func_) {
					return check_func_();
				}
				return true;
			}
			return false;
		}

	private:
		std::function<bool(void)> check_func_;
		func_type func_;
	};

	template<typename T> struct is_callable_type : std::false_type {	};
	template<typename RetT, typename... Args> struct is_callable_type<RetT(Args...)> : std::true_type { };
	template<typename RetT, typename... Args> struct is_callable_type<RetT(*)(Args...)> : std::true_type { };
	template<typename RetT, typename... Args> struct is_callable_type<RetT(&&)(Args...)> : std::true_type { };

	inline delegate_handle_t generate_handle_value() {
		return rand(std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max());
	}

}//namespace zee::impl 

	constexpr delegate_handle_t invalid_delegate_handle_v = 0;

	template<typename FuncT> struct delegate { };
	template<typename RetT, typename ... Args> 	struct delegate<RetT(Args...)> {	
		typedef impl::delegate_invoker<RetT, Args...> invoke_type;

		void bind_raw(RetT(*new_func)(Args...)) {
			invoker_.bind(new_func);
		}

		template<typename LambdaT>
		void bind_lambda(LambdaT&& new_func) {
			invoker_.bind(std::forward<LambdaT>(new_func));
		}

		template<typename ClassT>
		void bind_sp(const std::shared_ptr<ClassT>& sp, RetT(ClassT::* new_func)(Args...)) {
			if (sp) {
				invoker_.bind(sp, new_func);
			}
		}

		void execute(Args&&...args) {
			invoker_.invoke(std::forward<Args>(args)...);
		}

		void clear() {
			invoker_.reset();
		}

	private:
		invoke_type invoker_;
	};

	template<typename FuncT> struct multicast_delegate { };
	template<typename ... Args> struct multicast_delegate<void(Args...)> {
		typedef impl::delegate_invoker<void, Args...>  invoke_type;

		delegate_handle_t add_raw(void(*new_func)(Args...)) {
			delegate_handle_t new_handle = make_new_handle();
			invoker_list_[new_handle].bind(new_func);
			return new_handle;
		}

		template<typename LambdaT>
		delegate_handle_t add_lambda(LambdaT&& new_func) {
			delegate_handle_t new_handle = make_new_handle();
			invoker_list_[new_handle].bind(std::forward<LambdaT>(new_func));
			return new_handle;
		}

		template<typename ClassT>
		delegate_handle_t add_sp(const std::shared_ptr<ClassT>& sp, void(ClassT::* new_func)(Args...)) {
			delegate_handle_t new_handle = make_new_handle();
			invoker_list_[new_handle].bind(sp, new_func);
			return new_handle;
		}

		void broadcast(Args&&...args) noexcept {
			for (auto iter = begin(invoker_list_); iter != end(invoker_list_);) {
				if (iter->second.is_valid()) {
					iter->second.invoke(std::forward<Args>(args)...);
					++iter;
				} else {
					iter = invoker_list_.erase(iter);
				}
			}
		}

		void unbind(delegate_handle_t remove_handle) noexcept {
			if (invoker_list_.find(remove_handle) != end(invoker_list_)) {
				invoker_list_.erase(remove_handle);
			}
		}

		void clear() noexcept {
			invoker_list_.empty();
		}

	private:
		delegate_handle_t make_new_handle() const {
			delegate_handle_t ret = invalid_delegate_handle_v;
			while (ret == invalid_delegate_handle_v) {
				ret = impl::generate_handle_value();
				if (end(invoker_list_) != invoker_list_.find(ret)) {
					ret = invalid_delegate_handle_v;
				}
			}

			return ret;
		}

	private:
		std::map<delegate_handle_t, invoke_type> invoker_list_;
	};
}