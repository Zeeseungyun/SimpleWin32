#pragma once
#include <cmath>
#include <limits>
#include <algorithm>

namespace zee {
namespace math {
	constexpr float pi_v = 3.14159265359f;
	constexpr float inv_pi_v = 1 / pi_v;

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

	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	template<typename T>
	constexpr std::enable_if_t<std::is_arithmetic<T>::value, bool> is_zero(T value) noexcept {
		return value == 0;
	}

namespace impl {
	template<typename T>
	constexpr T clamp_impl(const T& src_v, const T& min_v, const T& max_v) noexcept {
		return min(max(src_v, min_v), max_v);
	}
	
}//namespace zee::math::impl

	template<typename T>
	constexpr std::enable_if_t<std::is_arithmetic<T>::value, T> 
		clamp(const T& src_v, const T& min_v, const T& max_v) noexcept {
		return impl::clamp_impl(src_v, min_v, max_v);
	}

}//namespace zee::math
}//namespace zee