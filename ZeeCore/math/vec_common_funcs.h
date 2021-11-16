#pragma once
#include "vec_base.h"
namespace zee {
namespace math {

	struct axis_idx {
		enum {
			x, y, z, w
		};
	};

	////////////////////////////////
	// 
	// Common function.
	// 
	////////////////////////////////

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, bool>
		is_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_equal(l[i], r[i])) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, bool>
		is_not_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return !is_equal(l, r);
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_all_vec_elem<VecElemT>::value, bool>
		is_zero(const vec_base<CompSize, VecElemT>& v) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_zero(v[i])) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_all_vec_elem<VecElemT>::value, bool>
		is_not_zero(const vec_base<CompSize, VecElemT>& v) noexcept {
		return !is_zero(v);
	}
	
	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<LeftElemT, RightElemT>>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT, EpsT>::value, bool>
		is_near_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r, EpsT eps = epsilon<EpsT>()) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_near_equal(l[i], r[i], eps)) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<LeftElemT, RightElemT>>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT, EpsT>::value, bool>
		is_near_not_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r, EpsT eps = epsilon<EpsT>()) noexcept {
		return !is_near_equal(l, r, eps);
	}

	template<size_t CompSize, typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value, bool>
		is_near_zero(const vec_base<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_near_zero(v[i], eps)) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value, bool>
		is_near_not_zero(const vec_base<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) {
		return !is_near_zero(v, eps);
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, const vec_base<CompSize, VecElemT>&>
		plus(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, vec_base<CompSize, VecElemT>>
		minus(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = -v[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		sub(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] - r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		assign_sub(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] -= r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		add(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] + r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		assign_add(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] += r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		mul(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] * r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		assign_mul(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] *= r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftT, RightElemT>::value && !is_vec<LeftT>::value, vec_base<CompSize, promotion_t<LeftT, RightElemT>>>
		mul(LeftT l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l * r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightT>>>
		mul(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] * r;
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, LeftElemT>&>
		assign_mul(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] *= r;
		}
		return l;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		div(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] / r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		assign_div(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] /= r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftT, RightElemT>::value && !is_vec<LeftT>::value, vec_base<CompSize, promotion_t<LeftT, RightElemT>>>
		div(LeftT l, const vec_base<CompSize, RightElemT>& r) noexcept {
		typedef promotion_t<LeftT, RightElemT> promotion_t;
		vec_base<CompSize, promotion_t<LeftT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)l / r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightT>>>
		div(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		typedef promotion_t<LeftElemT, RightT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] / r;
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, LeftElemT>&>
		assign_div(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] /= r;
		}
		return l;
	}

	////////////////////////////////
	// 
	// Common operator.
	// 
	////////////////////////////////

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, bool>
		operator==(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return is_equal(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, bool>
		operator!=(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return is_not_equal(l, r);
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, const vec_base<CompSize, VecElemT>&>
		operator+(const vec_base<CompSize, VecElemT>& v) noexcept {
		return plus(v);
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, vec_base<CompSize, VecElemT>>
		operator-(const vec_base<CompSize, VecElemT>& v) noexcept {
		return minus(v);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		operator-(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return sub(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		operator-=(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return assign_sub(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		operator+(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return add(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		operator+=(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return assign_add(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		operator*(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return mul(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		operator*=(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return assign_mul(l, r);
	}

	template<size_t CompSize, typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftT, RightElemT>::value && !is_vec<LeftT>::value, vec_base<CompSize, promotion_t<LeftT, RightElemT>>>
		operator*(LeftT l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return mul(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightT>>>
		operator*(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		return mul(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, LeftElemT>&>
		operator*=(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		return assign_mul(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		operator/(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return div(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, LeftElemT>&>
		operator/(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return assign_div(l, r);
	}

	template<size_t CompSize, typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftT, RightElemT>::value && !is_vec<LeftT>::value, vec_base<CompSize, promotion_t<LeftT, RightElemT>>>
		operator/(LeftT l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return div(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightT>>>
		operator/(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		return div(l, r);
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value, vec_base<CompSize, LeftElemT>&>
		operator/=(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		return assign_div(l, r);
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value, vec_base<CompSize, VecElemT>>
		abs(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = abs(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value, vec_base<CompSize, VecElemT>>
		ceil(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = ceil(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value, vec_base<CompSize, VecElemT>>
		floor(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = floor(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value, vec_base<CompSize, VecElemT>>
		trunc(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = trunc(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value, vec_base<CompSize, VecElemT>>
		round(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = round(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT, typename RemT = VecElemT>
	std::enable_if_t<std::is_floating_point<promotion_t<VecElemT, RemT>>::value, vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, RemT r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = fmod((promotion_t)v[i], (promotion_t)r);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT, typename RemT>
	std::enable_if_t<std::is_floating_point<promotion_t<VecElemT, RemT>>::value, vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, const vec_base<CompSize, RemT>& r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = fmod((promotion_t)v[i], (promotion_t)r[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		promotion_t<LeftElemT, RightElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret += l[i] * r[i];
		}
		return ret;
	}

	////////////////////////////////
	// 
	// Specialization for vec2, 3, 4
	//
	////////////////////////////////

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<2, LeftElemT>& l, const vec_base<2, RightElemT>& r) noexcept {
		return l[axis_idx::x] * r[axis_idx::x]
			+ l[axis_idx::y] * r[axis_idx::y];
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<3, LeftElemT>& l, const vec_base<3, RightElemT>& r) noexcept {
		return l[axis_idx::x] * r[axis_idx::x]
			+ l[axis_idx::y] * r[axis_idx::y]
			+ l[axis_idx::z] * r[axis_idx::z];
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<4, LeftElemT>& l, const vec_base<4, RightElemT>& r) noexcept {
		return l[axis_idx::x] * r[axis_idx::x]
			+ l[axis_idx::y] * r[axis_idx::y]
			+ l[axis_idx::z] * r[axis_idx::z]
			+ l[axis_idx::w] * r[axis_idx::w];
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, VecElemT>
		length_sq(const vec_base<CompSize, VecElemT>& v) noexcept {
		return dot(v, v);
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value, promotion_t<VecElemT, default_floating_point_t>>
		length(const vec_base<CompSize, VecElemT>& v) noexcept {
		return sqrt(length_sq(v));
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, vec_base<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
		reciprocal(const vec_base<CompSize, VecElemT>& v) noexcept {
		typedef promotion_t<default_floating_point_t, VecElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)1 / v[i];
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value, vec_base<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
		sqrt(const vec_base<CompSize, VecElemT>& v) noexcept {
		typedef promotion_t<default_floating_point_t, VecElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = sqrt((promotion_t)v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<
		is_all_vec_elem<LeftElemT, RightElemT>::value,
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>
		>
		min(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		typedef promotion_t<LeftElemT, RightElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = min((promotion_t)l[i], (promotion_t)r[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename... VecArgs>
	constexpr std::enable_if_t<
		is_all_vec_elem<LeftElemT, RightElemT>::value && 
		is_all_vec_with_size<CompSize, VecArgs...>::value, 
		vec_promotion_t<vec_base<CompSize, LeftElemT>, vec_base<CompSize, RightElemT>, VecArgs...>
		>
		min(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r, VecArgs ...args) noexcept {
		typedef vec_promotion_t<vec_base<CompSize, LeftElemT>, vec_base<CompSize, RightElemT>, VecArgs...> vec_promotion_t;
		vec_promotion_t ret;
		typedef typename vec_promotion_t::element_type promotion_t;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = min( (promotion_t)l[i], (promotion_t)r[i], (promotion_t)(args[i])...);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<
		is_all_vec_elem<LeftElemT, RightElemT>::value,
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>
		>
		max(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		typedef promotion_t<LeftElemT, RightElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = max((promotion_t)l[i], (promotion_t)r[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename... VecArgs>
	constexpr std::enable_if_t<
		is_all_vec_elem<LeftElemT, RightElemT>::value&&
		is_all_vec_with_size<CompSize, VecArgs...>::value &&
		(CompSize > 0),
		vec_promotion_t<vec_base<CompSize, LeftElemT>, vec_base<CompSize, RightElemT>, VecArgs...>
		>
		max(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r, VecArgs ...args) noexcept {
		typedef vec_promotion_t<vec_base<CompSize, LeftElemT>, vec_base<CompSize, RightElemT>, VecArgs...> vec_promotion_t;
		vec_promotion_t ret;
		typedef typename vec_promotion_t::element_type promotion_t;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = max((promotion_t)l[i], (promotion_t)r[i], (promotion_t)(args[i])...);
		}
		return ret;
	}

	template<size_t CompSize, typename SrcElemT, typename MinElemT, typename MaxElemT>
	constexpr std::enable_if_t<
		is_all_vec_elem<SrcElemT, MinElemT, MaxElemT>::value,
		vec_base<CompSize, promotion_t<SrcElemT, MinElemT, MaxElemT>>
		>
		clamp(const vec_base<CompSize, SrcElemT>& src_v, const vec_base<CompSize, MinElemT>& min_v, const vec_base<CompSize, MaxElemT>& max_v) noexcept {
		typedef promotion_t<SrcElemT, MinElemT, MaxElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = clamp((promotion_t)src_v[i], (promotion_t)min_v[i], (promotion_t)max_v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value, vec_base<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
		normalize(const vec_base<CompSize, VecElemT>& v) noexcept {
		typedef promotion_t<default_floating_point_t, VecElemT> promotion_t;
		const promotion_t rec_len = reciprocal((promotion_t)length(v));
		vec_base<CompSize, promotion_t> ret = v;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] *= rec_len;
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, bool>
		is_normalize(const vec_base<CompSize, VecElemT>& v) noexcept {
		return length_sq(v) == 1;
	}

	template<size_t CompSize, typename VecElemT, typename EpsT>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value, bool>
		is_near_normalize(const vec_base<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) noexcept {
		return is_near_equal(length_sq(v), 1, eps);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		distance_sq(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return length_sq(sub(l, r));
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<default_floating_point_t, LeftElemT, RightElemT>>
		distance(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		typedef promotion_t<default_floating_point_t, LeftElemT, RightElemT> promotion_t;
		return sqrt((promotion_t)distance_sq(l, r));
	}

	template<size_t CompSize, typename FirstElemT, typename LastElemT, typename DeltaT>
	constexpr std::enable_if_t<is_all_vec_elem<FirstElemT, LastElemT>::value 
		&& std::is_floating_point<DeltaT>::value 
		&& (CompSize > 0), 
		vec_base<CompSize, promotion_t<DeltaT, FirstElemT, LastElemT>>>
		lerp(const vec_base<CompSize, FirstElemT>& f, const vec_base<CompSize, LastElemT>& l, DeltaT d) noexcept {
		typedef promotion_t<DeltaT, FirstElemT, LastElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = lerp(f[i], l[i], d);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		reflect(const vec_base<CompSize, LeftElemT>& v, const vec_base<CompSize, RightElemT>& n) noexcept {
		return sub(v, mul(2, mul(n, dot(v, n))));
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		ccw(const vec_base<2, LeftElemT>& l, const vec_base<2, RightElemT>& r) noexcept {
		return {
			l[axis_idx::x] * r[axis_idx::y] - r[axis_idx::x] * l[axis_idx::y]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_base<3, promotion_t<LeftElemT, RightElemT>>>
		cross(const vec_base<3, LeftElemT>& l, const vec_base<3, RightElemT>& r) noexcept {
		return { 
			l[axis_idx::y] * r[axis_idx::z] - r[axis_idx::y] * l[axis_idx::z],
			l[axis_idx::z] * r[axis_idx::x] - r[axis_idx::z] * l[axis_idx::x],
			l[axis_idx::x] * r[axis_idx::y] - r[axis_idx::x] * l[axis_idx::y]
		};
	}

	////////////////////////////////
	// 
	// Specialization for integer
	//
	////////////////////////////////

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value, const vec_base<CompSize, VecElemT>&>
		ceil(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value, const vec_base<CompSize, VecElemT>&>
		floor(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value, const vec_base<CompSize, VecElemT>&>
		trunc(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value, const vec_base<CompSize, VecElemT>&>
		round(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}
	
	template<size_t CompSize, typename VecElemT, typename RemT>
	std::enable_if_t<is_all_vec_elem_not_floating<VecElemT, RemT>::value, vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, RemT r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)v[i] % (promotion_t)r;
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT, typename RemT>
	std::enable_if_t<is_all_vec_elem_not_floating<VecElemT, RemT>::value, vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, const vec_base<CompSize, RemT>& r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)v[i] % (promotion_t)r[i];
		}
		return ret;
	}

}//namespace zee::math
}//namespace zee