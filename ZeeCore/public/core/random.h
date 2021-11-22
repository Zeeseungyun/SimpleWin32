#pragma once
#include "../util/type_supports.h"
#include "../math/common.h"
#include <random>
#include <limits>

namespace zee {
namespace impl {

	template<typename T, 
		bool IsFloat = std::is_floating_point<T>::value, 
		bool IsInteger = std::is_integral<T>::value> 
		struct uniform_distiribution_selector {
		typedef void type;
	};

	template<typename T>
	struct uniform_distiribution_selector<T, true, false> {
		typedef std::uniform_real_distribution<T> type;
	};

	template<typename T>
	struct uniform_distiribution_selector<T, false, true> {
		typedef std::uniform_int_distribution<T> type;
	};

	template<typename T>
	struct is_random_arg :
		std::conditional_t <
		std::is_arithmetic<T>::value,
		std::true_type,
		std::false_type
		> {
	};

	template<typename... Args>
	struct is_all_random_arg : advance::unary::are_all_A<is_random_arg, Args...> {

	};

	std::default_random_engine& get_engine();

}//namespace impl 
	
	template<typename Arg1T, typename Arg2T>
	std::enable_if_t<impl::is_all_random_arg<Arg1T, Arg2T>::value, promotion_t<Arg1T, Arg2T>>
		rand(Arg1T first , Arg2T last) noexcept {
		typedef promotion_t<Arg1T, Arg2T> promotion_t;
		const promotion_t first_ = math::min(first, last);
		const promotion_t last_ = math::max(first, last);
		typename impl::uniform_distiribution_selector<promotion_t>::type dist(first_, last_);
		return dist(impl::get_engine());
	}
	
	template<typename T = int32>
	std::enable_if_t<impl::is_random_arg<T>::value, T>
		rand(T end = std::numeric_limits<T>::max()) noexcept {
		typename impl::uniform_distiribution_selector<T>::type dist(end < 0 ? end : 0, end < 0 ? 0 : end);
		return dist(impl::get_engine());
	}

}//namespace zee
