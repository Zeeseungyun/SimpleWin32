#pragma once
#include "../core/core_base.h"
#include "common.h"

namespace zee {
namespace math {

	template<size_t CompSize, typename VecElemT>
	struct vec_base {
		typedef VecElemT element_type;
		static constexpr size_t component_size = CompSize;
		union {
			element_type data[component_size];
		};

		element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		template<typename...Args>
		constexpr vec_base(Args&&...args) noexcept
			: data{ std::forward<Args>(args)... } {
			static_assert(is_all_vec_elem<Args...>::value && sizeof...(args) <= component_size, "invalid component size.");
		}

		template<typename...Args>
		constexpr vec_base() noexcept
			: data{ 0 } {
		}

		template<typename OtherElemT>
		vec_base(const vec_base<component_size, OtherElemT>& other) noexcept
			: vec_base() {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = other.data[i];
			}
		}

		template<typename OtherElemT>
		vec_base& operator=(const vec_base<component_size, OtherElemT>& other) noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = other.data[i];
			}
			return *this;
		}

		vec_base& operator=(const vec_base& other) noexcept {
			if (this != &other) {
				for (size_t i = 0; i != component_size; ++i) {
					data[i] = other.data[i];
				}
			}
			return *this;
		}

		constexpr size_t size() const { return component_size; }

		const element_type* begin() const { return data + 0; }
		const element_type* end() const { return data + component_size; }

		element_type* begin() { return data + 0; }
		element_type* end() { return data + component_size; }

		struct constants;
	};

	template<size_t CompSize, typename VecElemT>
	struct vec_base<CompSize, VecElemT>::constants {
		static constexpr vec_base<CompSize, VecElemT> zero = { };
	};

	template<typename VecElemT>
	struct vec_base<2, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 2;
		union {
			element_type x, y;
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_base() noexcept
			: vec_base(0, 0) {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<is_all_vec_elem<Elem1T, Elem2T>::value, int> = 0>
		constexpr vec_base(Elem1T new_x, Elem2T new_y) noexcept
			: data{ (element_type)new_x, (element_type)new_y } {
		}

		template<typename OtherElemT>
		constexpr vec_base(const vec_base<component_size, OtherElemT>& other) noexcept
			: vec_base(other[0], other[1]) {
		}

		template<typename OtherElemT>
		constexpr vec_base& operator=(const vec_base<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			return *this;
		}

		constexpr vec_base& operator=(const vec_base& other) noexcept {
			if (this != &other) {
				data[0] = other[0];
				data[1] = other[1];
			}

			return *this;
		}

		constexpr size_t size() const { return component_size; }

		const element_type* begin() const { return data + 0; }
		const element_type* end() const { return data + component_size; }

		element_type* begin() { return data + 0; }
		element_type* end() { return data + component_size; }

		struct constants;
	};

	template<typename VecElemT>
	struct vec_base<3, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 3;
		union {
			element_type x, y, z;
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_base() noexcept
			: vec_base(0, 0, 0) {
		}

		template<typename Elem1T, typename Elem2T, typename Elem3T, std::enable_if_t<is_all_vec_elem<Elem1T, Elem2T, Elem3T>::value, int> = 0>
		constexpr vec_base(Elem1T new_x, Elem2T new_y, Elem3T new_z) noexcept
			: data{ (element_type)new_x, (element_type)new_y, (element_type)new_z } {
		}

		template<typename OtherElemT>
		constexpr vec_base(const vec_base<component_size, OtherElemT>& other) noexcept
			: vec_base(other[0], other[1], other[2]) {
		}

		template<typename OtherElemT>
		constexpr vec_base& operator=(const vec_base<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			data[2] = other[2];
			return *this;
		}

		constexpr vec_base& operator=(const vec_base& other) noexcept {
			if (this != &other) {
				data[0] = other[0];
				data[1] = other[1];
				data[2] = other[2];
			}

			return *this;
		}

		constexpr size_t size() const { return component_size; }

		const element_type* begin() const { return data + 0; }
		const element_type* end() const { return data + component_size; }

		element_type* begin() { return data + 0; }
		element_type* end() { return data + component_size; }

		struct constants;
	};

	template<typename VecElemT>
	struct vec_base<4, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 4;
		union {
			element_type x, y, z, w;
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_base() noexcept
			: vec_base(0, 0, 0, 0) {
		}

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T, std::enable_if_t<is_all_vec_elem<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
		constexpr vec_base(Elem1T new_x, Elem2T new_y, Elem3T new_z, Elem4T new_w) noexcept
			: data{ (element_type)new_x, (element_type)new_y, (element_type)new_z, (element_type)new_w } {
		}

		template<typename OtherElemT>
		constexpr vec_base(const vec_base<component_size, OtherElemT>& other) noexcept
			: vec_base(other[0], other[1], other[2], other[3]) {
		}

		template<typename OtherElemT>
		constexpr vec_base& operator=(const vec_base<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			data[2] = other[2];
			data[3] = other[3];
			return *this;
		}

		constexpr vec_base& operator=(const vec_base& other) noexcept {
			if (this != &other) {
				data[0] = other[0];
				data[1] = other[1];
				data[2] = other[2];
				data[3] = other[3];
			}

			return *this;
		}

		constexpr size_t size() const { return component_size; }

		const element_type* begin() const { return data + 0; }
		const element_type* end() const { return data + component_size; }

		element_type* begin() { return data + 0; }
		element_type* end() { return data + component_size; }

		struct constants;
	};

	//Constants definitions

	template<typename VecElemT>
	struct vec_base<2, VecElemT>::constants {
		static constexpr vec_base<2, VecElemT> zero = {};
		static constexpr vec_base<2, VecElemT> one = { 1, 1 };
		static constexpr vec_base<2, VecElemT> unit_x = { 1, 0 };
		static constexpr vec_base<2, VecElemT> unit_y = { 0, 1 };
	};

	template<typename VecElemT>
	struct vec_base<3, VecElemT>::constants {
		static constexpr vec_base<3, VecElemT> zero = {};
		static constexpr vec_base<3, VecElemT> one = { 1, 1, 1 };
		static constexpr vec_base<3, VecElemT> unit_x = { 1, 0, 0 };
		static constexpr vec_base<3, VecElemT> unit_y = { 0, 1, 0 };
		static constexpr vec_base<3, VecElemT> unit_z = { 0, 0, 1 };
	};

	template<typename VecElemT>
	struct vec_base<4, VecElemT>::constants {
		static constexpr vec_base<4, VecElemT> zero = {};
		static constexpr vec_base<4, VecElemT> one = { 1, 1, 1, 1 };
		static constexpr vec_base<4, VecElemT> unit_x = { 1, 0, 0, 0 };
		static constexpr vec_base<4, VecElemT> unit_y = { 0, 1, 0, 0 };
		static constexpr vec_base<4, VecElemT> unit_z = { 0, 0, 1, 0 };
		static constexpr vec_base<4, VecElemT> unit_w = { 0, 0, 0, 1 };
	};

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), bool>
		is_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_equal(l[i], r[i])) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), bool>
		is_not_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return !is_equal(l, r);
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_all_vec_elem<VecElemT>::value && (CompSize > 0), bool>
		is_zero(const vec_base<CompSize, VecElemT>& v) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_zero(v[i])) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_all_vec_elem<VecElemT>::value && (CompSize > 0), bool>
		is_not_zero(const vec_base<CompSize, VecElemT>& v) noexcept {
		return !is_zero(v);
	}
	
	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<LeftElemT, RightElemT>>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT, EpsT>::value && (CompSize > 0), bool>
		is_near_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r, EpsT eps = epsilon<EpsT>()) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_near_equal(l[i], r[i], eps)) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<LeftElemT, RightElemT>>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT, EpsT>::value && (CompSize > 0), bool>
		is_near_not_equal(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r, EpsT eps = epsilon<EpsT>()) noexcept {
		return !is_near_equal(l, r, eps);
	}

	template<size_t CompSize, typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value && (CompSize > 0), bool>
		is_near_zero(const vec_base<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			if (!is_near_zero(v[i], eps)) {
				return false;
			}
		}
		return true;
	}

	template<size_t CompSize, typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value && (CompSize > 0), bool>
		is_near_not_zero(const vec_base<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) {
		return !is_near_zero(v, eps);
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		abs(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = abs(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		ceil(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = ceil(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		floor(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = floor(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		trunc(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = trunc(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		round(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = round(v[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT, typename RemT = VecElemT>
	std::enable_if_t<std::is_floating_point<promotion_t<VecElemT, RemT>>::value && (CompSize > 0), vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, RemT r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = fmod((promotion_t)v[i], (promotion_t)r);
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT, typename RemT>
	std::enable_if_t<std::is_floating_point<promotion_t<VecElemT, RemT>>::value && (CompSize > 0), vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, const vec_base<CompSize, RemT>& r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = fmod((promotion_t)v[i], (promotion_t)r[i]);
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		promotion_t<LeftElemT, RightElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret += l[i] * r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), VecElemT>
		length_sq(const vec_base<CompSize, VecElemT>& v) noexcept {
		return dot(v, v);
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), promotion_t<VecElemT, default_floating_point_t>>
		length(const vec_base<CompSize, VecElemT>& v) noexcept {
		return sqrt(length_sq(v));
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), const vec_base<CompSize, VecElemT>&>
		plus(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		minus(const vec_base<CompSize, VecElemT>& v) noexcept {
		vec_base<CompSize, VecElemT> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = -v[i];
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
		reciprocal(const vec_base<CompSize, VecElemT>& v) noexcept {
		typedef promotion_t<default_floating_point_t, VecElemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)1 / v[i];
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
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
		is_all_vec_elem<LeftElemT, RightElemT>::value &&
		(CompSize > 0),
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
		is_all_vec_with_size<CompSize, VecArgs...>::value && 
		(CompSize > 0), 
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
		is_all_vec_elem<LeftElemT, RightElemT>::value &&
		(CompSize > 0),
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
		is_all_vec_elem<SrcElemT, MinElemT, MaxElemT>::value &&
		(CompSize > 0),
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

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		sub(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] - r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, LeftElemT>>&
		assign_sub(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] -= r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		add(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] + r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, LeftElemT>>&
		assign_add(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] += r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		mul(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] * r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, LeftElemT>>&
		assign_mul(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] *= r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftT, RightElemT>::value && !is_vec<LeftT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftT, RightElemT>>>
		mul(LeftT l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l * r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftElemT, RightT>>>
		mul(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] * r;
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value && (CompSize > 0), vec_base<CompSize, LeftElemT>>&
		assign_mul(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] *= r;
		}
		return l;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		div(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] / r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), vec_base<CompSize, LeftElemT>>&
		assign_div(vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] /= r[i];
		}
		return l;
	}

	template<size_t CompSize, typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftT, RightElemT>::value && !is_vec<LeftT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftT, RightElemT>>>
		div(LeftT l, const vec_base<CompSize, RightElemT>& r) noexcept {
		typedef promotion_t<LeftT, RightElemT> promotion_t;
		vec_base<CompSize, promotion_t<LeftT, RightElemT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)l / r[i];
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<LeftElemT, RightT>>>
		div(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		typedef promotion_t<LeftElemT, RightT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = l[i] / r;
		}
		return ret;
	}

	template<size_t CompSize, typename LeftElemT, typename RightT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightT>::value && !is_vec<RightT>::value && (CompSize > 0), vec_base<CompSize, LeftElemT>>&
		assign_div(const vec_base<CompSize, LeftElemT>& l, RightT r) noexcept {
		vec_base<CompSize, promotion_t<LeftElemT, RightT>> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			l[i] /= r;
		}
		return l;
	}

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<std::is_floating_point<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>>
		normalize(const vec_base<CompSize, VecElemT>& v) noexcept {
		const VecElemT rec_len = reciprocal(length(v));
		vec_base<CompSize, VecElemT> ret = v;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] *= rec_len;
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value && (CompSize > 0), bool>
		is_normalize(const vec_base<CompSize, VecElemT>& v) noexcept {
		return length_sq(v) == 1;
	}

	template<size_t CompSize, typename VecElemT, typename EpsT>
	constexpr std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value && (CompSize > 0), bool>
		is_near_normalize(const vec_base<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) noexcept {
		return is_near_equal(length_sq(v), 1, eps);
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), promotion_t<LeftElemT, RightElemT>>
		distance_sq(const vec_base<CompSize, LeftElemT>& l, const vec_base<CompSize, RightElemT>& r) noexcept {
		return length_sq(sub(l, r));
	}

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), promotion_t<default_floating_point_t, LeftElemT, RightElemT>>
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

	//reflect

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value && (CompSize > 0), 
		vec_base<CompSize, promotion_t<LeftElemT, RightElemT>>>
		reflect(const vec_base<CompSize, LeftElemT>& v, const vec_base<CompSize, RightElemT>& n) noexcept {
		return sub(v, mul(n, 2 * dot(v, n)));
	}

	////////////////////////////////
	// 
	// Specialization for vec2, 3, 4
	//
	////////////////////////////////

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<2, LeftElemT>& l, const vec_base<2, RightElemT>& r) noexcept {
		return l[0] * r[0]
			+  l[1] * r[1];
	}
	
	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<3, LeftElemT>& l, const vec_base<3, RightElemT>& r) noexcept {
		return l[0] * r[0]
			+  l[1] * r[1]
			+  l[2] * r[2];
	}
	
	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_base<4, LeftElemT>& l, const vec_base<4, RightElemT>& r) noexcept {
		return l[0] * r[0]
			+  l[1] * r[1]
			+  l[2] * r[2]
			+  l[3] * r[3];
	}

	////////////////////////////////
	// 
	// Specialization for integer
	//
	////////////////////////////////

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value && (CompSize > 0), const vec_base<CompSize, VecElemT>&>
		ceil(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value && (CompSize > 0), const vec_base<CompSize, VecElemT>&>
		floor(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value && (CompSize > 0), const vec_base<CompSize, VecElemT>&>
		trunc(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem_not_floating<VecElemT>::value && (CompSize > 0), const vec_base<CompSize, VecElemT>&>
		round(const vec_base<CompSize, VecElemT>& v) noexcept {
		return v;
	}
	
	template<size_t CompSize, typename VecElemT, typename RemT>
	constexpr std::enable_if_t<is_all_vec_elem_not_floating<VecElemT, RemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, RemT r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)v[i] % (promotion_t)r;
		}
		return ret;
	}

	template<size_t CompSize, typename VecElemT, typename RemT>
	constexpr std::enable_if_t<is_all_vec_elem_not_floating<VecElemT, RemT>::value && (CompSize > 0), vec_base<CompSize, promotion_t<VecElemT, RemT>>>
		fmod(const vec_base<CompSize, VecElemT>& v, const vec_base<CompSize, RemT>& r) noexcept {
		typedef promotion_t<VecElemT, RemT> promotion_t;
		vec_base<CompSize, promotion_t> ret;
		for (size_t i = 0; i != CompSize; ++i) {
			ret[i] = (promotion_t)v[i] % (promotion_t)r[i];
		}
		return ret;
	}

	////////////////////////////////
	// 
	// Explicit instantiation.
	// 
	////////////////////////////////

	template struct vec_base<2, int32>;
	template struct vec_base<2, float>;
	//template struct vec_base<2, double>;

	template struct vec_base<3, int32>;
	template struct vec_base<3, float>;
	//template struct vec_base<3, double>;

	template struct vec_base<4, int32>;
	template struct vec_base<4, float>;
	//template struct vec_base<4, double>;
	
	////////////////////////////////
	// 
	// Type redefinition.
	// 
	////////////////////////////////

	template<size_t CompSize, typename VecElemT>
	using vec = vec_base<CompSize, VecElemT>;

	typedef vec<2, int32>  vec2;
	typedef vec<2, float>  vec2f;
	typedef vec<2, double> vec2df;

	typedef vec<3, int32>  vec3;
	typedef vec<3, float>  vec3f;
	typedef vec<3, double> vec3df;

	typedef vec<4, int32>  vec4;
	typedef vec<4, float>  vec4f;
	typedef vec<4, double> vec4df;

}//namespace zee::math
}//namespace zee 

void test() {
	constexpr zee::math::vec<3, int> temp1 = { 1, 2, 3 };
	constexpr zee::math::vec<3, double> temp2 = { 2, 4, 6 };
	zee::is_vec<zee::math::vec<3, double>>::value;
	constexpr zee::math::vec2f v1 = { 1, 0 };
	constexpr zee::math::vec2f v2 = { 3, 4 };
	constexpr zee::math::vec2f v3 = { 6, 5 };
	constexpr zee::math::vec2f v4 = { 7, 8 };
	constexpr zee::math::vec3f v5 = { 7, 8, 9 };

	//constexpr auto fmin = min(v1, v2, v3);
	//constexpr auto fmax = max(v1, v2, v3, v4);
	//constexpr auto fmax = max(v1, v2, v3, v4);
	constexpr auto fmax = reflect(v2, v1);
	//constexpr auto dssq = distance_sq(v1, v2);
	//constexpr auto fmd = fmod(zee::math::vec2{ 1,2 }, 2);
	//
	//constexpr auto lrp = lerp(v1, v2, 0.5);
	//
	////constexpr auto a22a = is_normalize(zee::math::vec<5, float>{1, 0, 0, 0, 0});
	//constexpr auto clmp = zee::math::clamp(3, 1, 2);
}
