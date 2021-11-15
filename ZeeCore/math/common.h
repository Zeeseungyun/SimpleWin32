#pragma once
#include <cmath>
#include <limits>
#include <algorithm>
#include "../core/core_base.h"
#include "../core/type_supports.h"

namespace zee {
namespace math {
	constexpr default_floating_point_t pi_v = (default_floating_point_t)3.14159265359f;
	constexpr default_floating_point_t inv_pi_v = (default_floating_point_t)1 / pi_v;

	// Basic operations 
	using std::abs;	using std::fmod; using std::remainder;

	// Exponential functions
	using std::exp; using std::exp2; using std::expm1; 
	using std::log; using std::log10; using std::log2;

	// Power functions
	using std::pow; using std::sqrt; using std::cbrt; using std::hypot;

	//Trigonometric functions
	using std::cos;	 using std::sin;  using std::tan;
	using std::acos; using std::asin; using std::atan;
	using std::atan2;

	//Hyperbolic functions
	using std::sinh; using std::cosh; using std::tanh;
	using std::asinh; using std::acosh; using std::atanh;

	//Nearest integer floating-point operations
	using std::ceil; using std::floor; using std::trunc; using std::round;
	using std::isnan; using std::isinf; using std::isnormal;
	
	using std::max; using std::min;

	constexpr default_floating_point_t epsilon = std::numeric_limits<default_floating_point_t>::epsilon();

	template<typename T, typename RetT = promotion_t<default_floating_point_t, T>>
	constexpr std::enable_if_t<std::is_signed<T>::value, RetT>
		reciprocal(T v) noexcept {
		return (RetT)1 / v;
	}

namespace impl {
	template<typename RetT, typename SrcT, typename MinT, typename MaxT>
	constexpr RetT clamp_impl(const SrcT& src_v, const MinT& min_v, const MaxT& max_v) noexcept {
		return min(max(src_v, min_v), max_v);
	}
	
}//namespace zee::math::impl

	template<typename T>
	constexpr std::enable_if_t<std::is_arithmetic<T>::value, T> 
		clamp(const T& src_v, const T& min_v, const T& max_v) noexcept {
		return impl::clamp_impl<T>(src_v, min_v, max_v);
	}

	template<typename T>
	constexpr std::enable_if_t<is_arithmetic<T>::value, bool> 
		is_zero(T v) noexcept {
		return v == 0;
	}

	template<typename T>
	constexpr std::enable_if_t<is_arithmetic<T>::value, bool> 
		is_not_zero(T v) noexcept {
		return v != 0;
	}

	template<typename LeftT, typename RightT>
	std::enable_if_t<is_arithmetic<LeftT, RightT>::value, bool>
		is_equal(LeftT l, RightT r) noexcept {
		return l == r;
	}

	template<typename LeftT, typename RightT>
	std::enable_if_t<is_arithmetic<LeftT, RightT>::value, bool>
		is_not_equal(LeftT l, RightT r) noexcept {
		return l != r;
	}

	template<typename T, typename EpsT = T>
	std::enable_if_t<is_floating_point<T, EpsT>::value, bool>
		is_near_zero(T v, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return abs(v) <= eps;
	}

	template<typename T, typename EpsT = T>
	std::enable_if_t<is_floating_point<T, EpsT>::value, bool>
		is_near_not_zero(T v, EpsT eps = std::numeric_limits<promotion_t<EpsT>>::epsilon()) noexcept {
		return !is_near_zero(v);
	}

	template<typename LeftT, typename RightT, typename EpsT = promotion_t<LeftT, RightT>>
	std::enable_if_t<is_floating_point<promotion_t<LeftT, RightT>, EpsT>::value, bool>
		is_near_equal(LeftT l, RightT r, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return abs(l - r) <= eps;
	}

	template<typename LeftT, typename RightT, typename EpsT = promotion_t<LeftT, RightT>>
	std::enable_if_t<is_floating_point<promotion_t<LeftT, RightT>, EpsT>::value, bool>
		is_near_not_equal(LeftT l, RightT r, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return !is_near_equal(l, r, eps);
	}

	//for integers..

	template<typename T>
	std::enable_if_t<!is_floating_point<T>::value && is_arithmetic<T>::value, bool>
		is_near_zero(T v, default_floating_point_t unused = 0) noexcept {
		return is_zero(v);
	}

	template<typename T>
	std::enable_if_t<!is_floating_point<T>::value && is_arithmetic<T>::value, bool>
		is_near_not_zero(T v, default_floating_point_t unused = 0) noexcept {
		return !is_near_zero(v);
	}

	template<typename LeftT, typename RightT>
	std::enable_if_t<!is_floating_point<promotion_t<LeftT, RightT>>::value && is_arithmetic<LeftT, RightT>::value, bool>
		is_near_equal(LeftT l, RightT r, default_floating_point_t unused = 0) noexcept {
		return is_equal(l, r);
	}

	template<typename LeftT, typename RightT>
	std::enable_if_t<!is_floating_point<promotion_t<LeftT, RightT>>::value && is_arithmetic<LeftT, RightT>::value, bool>
		is_near_not_equal(LeftT l, RightT r, default_floating_point_t unused = 0) noexcept {
		return !is_equal(l, r);
	}

}//namespace zee::math
}//namespace zee