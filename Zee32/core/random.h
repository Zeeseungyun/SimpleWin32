#pragma once
#include <random>
#include <limits>
#include "util/type_supports.h"

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

	template<typename T, typename... Args>
	struct is_valid_random_arg :
		std::conditional_t <
		is_valid_random_arg<T>::value && is_valid_random_arg<Args...>::value,
		std::true_type,
		std::false_type
		> {
	};

	template<typename T>
	struct is_valid_random_arg<T> : 
		std::conditional_t <
		std::is_arithmetic<T>::value, 
		std::true_type,
		std::false_type
	> {
	};
	
}//namespace impl 

	std::default_random_engine& get_engine();
	
	template<typename Arg1T, typename Arg2T>
	std::enable_if_t<impl::is_valid_random_arg<Arg1T, Arg2T>::value, promotion_t<Arg1T, Arg2T>>
		rand(Arg1T begin, Arg2T end) {
		typedef promotion_t<Arg1T, Arg2T> promotion_t;
		const promotion_t begin_ = begin < end ? begin : end;
		const promotion_t end_ = begin < end ? end : begin;
		typename impl::uniform_distiribution_selector<promotion_t>::type dist(begin_, end_);
		return dist(get_engine());
	}
	
	template<typename T>
	std::enable_if_t<impl::is_valid_random_arg<T>::value, T>
		rand(T end = std::numeric_limits<T>::max()) {
		typename impl::uniform_distiribution_selector<T>::type dist(end < 0 ? end : 0, end < 0 ? 0 : end);
		return dist(get_engine());
	}
}//namespace zee
