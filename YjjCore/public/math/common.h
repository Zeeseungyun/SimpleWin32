#pragma once
#include <cmath>
#include <limits>
#include "../core/core_base.h"
#include "../util/type_supports.h"

namespace yjj {
namespace math {
namespace constants {
	template<typename FloatT>
	static constexpr std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT> 
		pi_v = (FloatT)3.14159265358979323846264338327950288;

	template<typename FloatT>
	static constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		inv_pi_v = 1 / pi_v<FloatT>;

	template<typename FloatT>
	static constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		half_pi_v = pi_v<FloatT> / 2;

}//namespace yjj::math::constants

	template<typename FloatT = default_floating_point_t>
	constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		pi() noexcept {
		return constants::pi_v<FloatT>;
	}

	template<typename FloatT = default_floating_point_t>
	constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		half_pi() noexcept {
		return constants::half_pi_v<FloatT>;
	}

	template<typename FloatT = default_floating_point_t>
	constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		inv_pi() noexcept {
		return constants::inv_pi_v<FloatT>;
	}

	template<typename FloatT>
	constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		rad_to_deg(FloatT rad) noexcept {
		return rad * inv_pi<FloatT>() * 180;
	}

	template<typename FloatT>
	constexpr
		std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		deg_to_rad(FloatT deg) noexcept {
		return deg / 180 * pi<FloatT>();
	}

	template<typename FloatT>
	std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		deg_to_normalize(FloatT deg) noexcept {
		return std::fmod(deg, (FloatT)360);
	}

	template<typename T>
	constexpr std::enable_if_t<std::is_integral<T>::value, T>
		deg_to_normalize(T deg) noexcept {
		return deg % 360;
	}

	template<typename FloatT>
	std::enable_if_t<std::is_floating_point<FloatT>::value, FloatT>
		rad_to_normalize(FloatT deg) noexcept {
		return std::fmod(deg, 2 * pi<FloatT>());
	}

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
	
	//using std::max; using std::min;

	template<typename T, typename U>
	constexpr std::enable_if_t<are_all_arithemtic<T, U>::value, promotion_t<T, U>>
		max(const T& l, const U& r) noexcept {
		return promotion_t<T, U>(l < r ? r : l);
	}

	template<typename T, typename U, typename ...Args>
	constexpr std::enable_if_t<are_all_arithemtic<T, Args...>::value, promotion_t<T, Args...>>
		max(const T& l, const U& r, const Args&... args) noexcept {
		return max(max(l, r), args...);
	}

	template<typename T, typename U>
	constexpr std::enable_if_t<are_all_arithemtic<T, U>::value, promotion_t<T, U>>
		min(const T& l, const U& r) noexcept {
		typedef promotion_t<T, U> promotion_t;
		return promotion_t(l > r ? r : l);
	}

	template<typename T, typename U, typename ...Args>
	constexpr std::enable_if_t<are_all_arithemtic<T, Args...>::value, promotion_t<T, Args...>>
		min(const T& l, const U& r, const Args&... args) noexcept {
		typedef promotion_t<T, Args...> promotion_t;
		return min((promotion_t)min((promotion_t)l, (promotion_t)r), (promotion_t)args...);
	}

	template<typename T>
	constexpr 
		std::enable_if_t<std::is_floating_point<T>::value, T> 
		epsilon() noexcept {
		return std::numeric_limits<T>::epsilon();
	}

	template<typename T>
	constexpr
		std::enable_if_t<is_arithmetic_not_floating<T>::value, T>
		epsilon() noexcept {
		return 0;
	}

	constexpr default_floating_point_t epsilon_v = std::numeric_limits<default_floating_point_t>::epsilon();

	template<typename T, typename RetT = promotion_t<default_floating_point_t, T>>
	constexpr std::enable_if_t<std::is_arithmetic<T>::value, RetT>
		reciprocal(const T& v) noexcept {
		return (RetT)1 / v;
	}

namespace impl {
	template<typename RetT, typename SrcT, typename MinT, typename MaxT>
	constexpr RetT clamp_impl(const SrcT& src_v, const MinT& min_v, const MaxT& max_v) noexcept {
		return min(max(src_v, min_v), max_v);
	}
	
}//namespace yjj::math::impl

	template<typename SrcT, typename MinT, typename MaxT>
	constexpr std::enable_if_t<are_all_arithemtic<SrcT, MinT, MaxT>::value, promotion_t<SrcT, MinT, MaxT>>
		clamp(const SrcT& src_v, const MinT& min_v, const MaxT& max_v) noexcept {
		typedef promotion_t<SrcT, MinT, MaxT> promotion_t;
		return impl::clamp_impl<promotion_t>((promotion_t)src_v, (promotion_t)min_v, (promotion_t)max_v);
	}

	template<typename T>
	constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
		saturate(const T& v) noexcept {
		return clamp(v, (T)0, (T)1);
	}

	template<typename T>
	constexpr std::enable_if_t<are_all_arithemtic<T>::value, bool>
		is_zero(T v) noexcept {
		return v == 0;
	}

	template<typename T>
	constexpr std::enable_if_t<are_all_arithemtic<T>::value, bool>
		is_not_zero(T v) noexcept {
		return v != 0;
	}

	template<typename LeftT, typename RightT>
	constexpr std::enable_if_t<are_all_arithemtic<LeftT, RightT>::value, bool>
		is_equal(LeftT l, RightT r) noexcept {
		return l == r;
	}

	template<typename LeftT, typename RightT>
	constexpr std::enable_if_t<are_all_arithemtic<LeftT, RightT>::value, bool>
		is_not_equal(LeftT l, RightT r) noexcept {
		return l != r;
	}

	template<typename T, typename EpsT = T>
	std::enable_if_t<are_all_arithemtic<T, EpsT>::value, bool>
		is_near_zero(T v, EpsT eps = epsilon<EpsT>()) noexcept {
		return abs(v) <= eps;
	}

	template<typename T, typename EpsT = T>
	std::enable_if_t<are_all_arithemtic<T, EpsT>::value, bool>
		is_near_not_zero(T v, EpsT eps = epsilon<EpsT>()) noexcept {
		return !is_near_zero(v);
	}

	template<typename LeftT, typename RightT, typename EpsT = promotion_t<LeftT, RightT>>
	std::enable_if_t<are_all_arithemtic<LeftT, RightT, EpsT>::value, bool>
		is_near_equal(LeftT l, RightT r, EpsT eps = epsilon<EpsT>()) noexcept {
		return abs(l - r) <= eps;
	}

	template<typename LeftT, typename RightT, typename EpsT = promotion_t<LeftT, RightT>>
	std::enable_if_t<are_all_arithemtic<LeftT, RightT, EpsT>::value, bool>
		is_near_not_equal(LeftT l, RightT r, EpsT eps = epsilon<EpsT>()) noexcept {
		return !is_near_equal(l, r, eps);
	}

	template<typename FirstT, typename LastT, typename DeltaT>
	constexpr std::enable_if_t<are_all_arithemtic<FirstT, LastT, DeltaT>::value && std::is_floating_point<DeltaT>::value, 
		promotion_t<FirstT, LastT, DeltaT>>
		lerp(FirstT f, LastT l, DeltaT d) noexcept {
		return f + (l - f) * clamp(d, 0, 1);
	}

	//template<size_t CompSize, typename VecElem1T, typename VecElem2T, typename VecElem3T, typename VecElem4T, typename DeltaT>
	//constexpr std::enable_if_t<are_all_arithemtic<VecElem1T, VecElem2T, VecElem3T, VecElem4T, DeltaT>::value 
	//	&& std::is_floating_point<DeltaT>::value,
	//	promotion_t<VecElem1T, VecElem2T, VecElem3T, VecElem4T, DeltaT>>
	//	catmullrom(
	//		const basic_vec<CompSize, VecElem1T>& p1,
	//		const basic_vec<CompSize, VecElem2T>& p2,
	//		const basic_vec<CompSize, VecElem3T>& p3,
	//		const basic_vec<CompSize, VecElem4T>& p4,
	//		DeltaT d) noexcept {
	//	typedef promotion_t<VecElem1T, VecElem2T, VecElem3T, VecElem4T, DeltaT> promotion_t;
	//	basic_vec<CompSize, promotion_t> v1 = p2;
	//	basic_vec<CompSize, promotion_t> v2 = p3;
	//	basic_vec<CompSize, promotion_t> t1 = (p3 - p1) * (promotion_t)0.5;
	//	basic_vec<CompSize, promotion_t> t2 = (p4 - p2) * (promotion_t)0.5;
	//	const promotion_t delta_cubic_mul_2 = 0;
	//	return p2;
	//}

}//namespace yjj::math
}//namespace yjj