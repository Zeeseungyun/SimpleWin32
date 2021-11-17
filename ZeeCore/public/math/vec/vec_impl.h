#pragma once
#include "../../core/core_base.h"
#include "../common.h"

namespace zee {
namespace math {

	template<size_t CompSize, typename VecElemT>
	struct vec_impl;

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_all_vec_elem<VecElemT>::value, bool>
		is_zero(const vec_impl<CompSize, VecElemT>& v) noexcept;

	template<size_t CompSize, typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<LeftElemT, RightElemT>>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT, EpsT>::value , bool>
		is_near_equal(const vec_impl<CompSize, LeftElemT>& l, const vec_impl<CompSize, RightElemT>& r, EpsT eps = epsilon<EpsT>()) noexcept;

	template<size_t CompSize, typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value, bool>
		is_near_zero(const vec_impl<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) noexcept;

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_impl<CompSize, LeftElemT>& l, const vec_impl<CompSize, RightElemT>& r) noexcept;

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_impl<2, LeftElemT>& l, const vec_impl<2, RightElemT>& r) noexcept;

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_impl<3, LeftElemT>& l, const vec_impl<2, RightElemT>& r) noexcept;

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		dot(const vec_impl<4, LeftElemT>& l, const vec_impl<2, RightElemT>& r) noexcept;

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, VecElemT>
		length_sq(const vec_impl<CompSize, VecElemT>& v) noexcept;

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value, promotion_t<VecElemT, default_floating_point_t>>
		length(const vec_impl<CompSize, VecElemT>& v) noexcept;

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, vec_impl<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
		reciprocal(const vec_impl<CompSize, VecElemT>& v) noexcept;

	template<size_t CompSize, typename VecElemT>
	std::enable_if_t<is_vec_elem<VecElemT>::value, vec_impl<CompSize, promotion_t<default_floating_point_t, VecElemT>>>
		normalize(const vec_impl<CompSize, VecElemT>& v) noexcept;

	template<size_t CompSize, typename VecElemT>
	constexpr std::enable_if_t<is_vec_elem<VecElemT>::value, bool>
		is_normalize(const vec_impl<CompSize, VecElemT>& v) noexcept;

	template<size_t CompSize, typename VecElemT, typename EpsT>
	std::enable_if_t<is_all_vec_elem<VecElemT, EpsT>::value, bool>
		is_near_normalize(const vec_impl<CompSize, VecElemT>& v, EpsT eps = epsilon<EpsT>()) noexcept;

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		distance_sq(const vec_impl<CompSize, LeftElemT>& l, const vec_impl<CompSize, RightElemT>& r) noexcept;

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<default_floating_point_t, LeftElemT, RightElemT>>
		distance(const vec_impl<CompSize, LeftElemT>& l, const vec_impl<CompSize, RightElemT>& r) noexcept;

	template<size_t CompSize, typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value,
		vec_impl<CompSize, promotion_t<LeftElemT, RightElemT>>>
		reflect(const vec_impl<CompSize, LeftElemT>& v, const vec_impl<CompSize, RightElemT>& n) noexcept;

	template<size_t CompSize, typename VecElemT>
	struct vec_impl {
		typedef std::conditional_t<is_vec_elem<VecElemT>::value, VecElemT, void> element_type;
		static constexpr size_t component_size = CompSize;
		union {
			element_type data[component_size];
		};

		element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		template<typename...Args>
		constexpr vec_impl(Args&&...args) noexcept
			: data{ std::forward<Args>(args)... } {
			static_assert(is_all_vec_elem<Args...>::value && sizeof...(args) <= component_size, "invalid component size.");
		}

		template<typename...Args>
		constexpr vec_impl() noexcept
			: data{ 0 } {
		}

		template<typename OtherElemT>
		vec_impl(const vec_impl<component_size, OtherElemT>& other) noexcept
			: vec_impl() {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = other.data[i];
			}
		}

		template<typename OtherElemT>
		vec_impl& operator=(const vec_impl<component_size, OtherElemT>& other) noexcept {
			for (size_t i = 0; i != component_size; ++i) {
				data[i] = other.data[i];
			}
			return *this;
		}

		vec_impl& operator=(const vec_impl& other) noexcept {
			if (this != &other) {
				for (size_t i = 0; i != component_size; ++i) {
					data[i] = other.data[i];
				}
			}
			return *this;
		}

		constexpr size_t size() const noexcept { return component_size; }

		const element_type* begin() const noexcept { return data + 0; }
		const element_type* end() const noexcept { return data + component_size; }

		element_type* begin() noexcept { return data + 0; }
		element_type* end() noexcept { return data + component_size; }

		constexpr bool is_zero() const noexcept {
			return math::is_zero(*this);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT, EpsT>::value, bool>
			is_near_equal(vec_impl<component_size, OtherElemT>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename EpsT = element_type>
		std::enable_if_t<is_all_vec_elem<element_type, EpsT>::value, bool>
			is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename OtherElemT>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			dot(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::dot(*this, other);
		}

		element_type length_sq() const noexcept {
			return math::length_sq(*this);
		}

		element_type length() const noexcept {
			return math::length(*this);
		}

		constexpr vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			reciprocal() const noexcept {
			return math::reciprocal(*this);
		}

		vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			normalize() const noexcept {
			return math::normalize(*this);
		}

		constexpr bool is_normalize() const noexcept {
			return math::is_normalize(*this);
		}

		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_normalize(*this, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance_sq(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance_sq(*this, other);
		}

		template<typename OtherElemT>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance(*this, other);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, vec_impl<component_size, promotion_t<element_type, OtherElemT>>>
			reflect(const vec_impl<component_size, OtherElemT>& normal_v) const noexcept {
			return math::reflect(*this, normal_v);
		}

		struct constants;
	};

	template<typename VecElemT> struct vec_impl<0, VecElemT> { };

	template<size_t CompSize, typename VecElemT>
	struct vec_impl<CompSize, VecElemT>::constants {
		static constexpr vec_impl<CompSize, VecElemT> zero = { };
	};

	////////////////////////////////
	// 
	// Vec2, 3, 4 Partial specialization.
	// 
	////////////////////////////////

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, promotion_t<LeftElemT, RightElemT>>
		ccw(const vec_impl<2, LeftElemT>& l, const vec_impl<2, RightElemT>& r) noexcept;

	template<typename VecElemT>
	struct vec_impl<2, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 2;
		union {
			struct {
				element_type x, y;
			};
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_impl() noexcept
			: vec_impl(0, 0) {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<is_all_vec_elem<Elem1T, Elem2T>::value, int> = 0>
		constexpr vec_impl(Elem1T new_x, Elem2T new_y) noexcept
			: data{ (element_type)new_x, (element_type)new_y } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT>& other) noexcept
			: vec_impl(other[0], other[1]) {
		}

		template<typename OtherElemT>
		constexpr vec_impl& operator=(const vec_impl<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			return *this;
		}

		constexpr vec_impl& operator=(const vec_impl& other) noexcept {
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

		constexpr bool is_zero() const noexcept {
			return math::is_zero(*this);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT, EpsT>::value, bool>
			is_near_equal(vec_impl<component_size, OtherElemT>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename EpsT = element_type>
		std::enable_if_t<is_all_vec_elem<element_type, EpsT>::value, bool>
			is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			dot(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::dot(*this, other);
		}

		constexpr element_type length_sq() const noexcept {
			return math::length_sq(*this);
		}

		element_type length() const noexcept {
			return math::length(*this);
		}

		constexpr vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			reciprocal() const noexcept {
			return math::reciprocal(*this);
		}

		vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			normalize() const noexcept {
			return math::normalize(*this);
		}

		constexpr bool is_normalize() const noexcept {
			return math::is_normalize(*this);
		}

		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_normalize(*this, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance_sq(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance_sq(*this, other);
		}

		template<typename OtherElemT>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance(*this, other);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, vec_impl<component_size, promotion_t<element_type, OtherElemT>>>
			reflect(const vec_impl<component_size, OtherElemT>& normal_v) const noexcept {
			return math::reflect(*this, normal_v);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			ccw(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::ccw(*this, other);
		}

		struct constants;
	};

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, vec_impl<3, promotion_t<LeftElemT, RightElemT>>>
		cross(const vec_impl<3, LeftElemT>& l, const vec_impl<3, RightElemT>& r) noexcept;

	template<typename VecElemT>
	struct vec_impl<3, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 3;
		union {
			struct {
				element_type x, y, z;
			};
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_impl() noexcept
			: vec_impl(0, 0, 0) {
		}

		template<typename Elem1T, typename Elem2T, typename Elem3T, std::enable_if_t<is_all_vec_elem<Elem1T, Elem2T, Elem3T>::value, int> = 0>
		constexpr vec_impl(Elem1T new_x, Elem2T new_y, Elem3T new_z) noexcept
			: data{ (element_type)new_x, (element_type)new_y, (element_type)new_z } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT>& other) noexcept
			: vec_impl(other[0], other[1], other[2]) {
		}

		template<typename OtherElemT>
		constexpr vec_impl& operator=(const vec_impl<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			data[2] = other[2];
			return *this;
		}

		constexpr vec_impl& operator=(const vec_impl& other) noexcept {
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

		constexpr bool is_zero() const noexcept {
			return math::is_zero(*this);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT, EpsT>::value, bool>
			is_near_equal(vec_impl<component_size, OtherElemT>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename EpsT = element_type>
		std::enable_if_t<is_all_vec_elem<element_type, EpsT>::value, bool>
			is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			dot(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::dot(*this, other);
		}

		constexpr element_type length_sq() const noexcept {
			return math::length_sq(*this);
		}

		element_type length() const noexcept {
			return math::length(*this);
		}

		constexpr vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			reciprocal() const noexcept {
			return math::reciprocal(*this);
		}

		vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			normalize() const noexcept {
			return math::normalize(*this);
		}

		constexpr bool is_normalize() const noexcept {
			return math::is_normalize(*this);
		}

		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_normalize(*this, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance_sq(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance_sq(*this, other);
		}

		template<typename OtherElemT>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance(*this, other);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, vec_impl<component_size, promotion_t<element_type, OtherElemT>>>
			reflect(const vec_impl<component_size, OtherElemT>& normal_v) const noexcept {
			return math::reflect(*this, normal_v);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, vec_impl<component_size, promotion_t<element_type, OtherElemT>>>
			cross(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::cross(*this, other);
		}

		struct constants;
	};

	template<typename VecElemT>
	struct vec_impl<4, VecElemT> {
		typedef VecElemT element_type;
		static constexpr size_t component_size = 4;
		union {
			struct {
				element_type x, y, z, w;
			};
			element_type data[component_size];
		};

		constexpr element_type& operator[](size_t idx) noexcept { return data[idx]; }
		constexpr const element_type& operator[](size_t idx) const noexcept { return data[idx]; }

		constexpr vec_impl() noexcept
			: vec_impl(0, 0, 0, 0) {
		}

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T, std::enable_if_t<is_all_vec_elem<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
		constexpr vec_impl(Elem1T new_x, Elem2T new_y, Elem3T new_z, Elem4T new_w) noexcept
			: data{ (element_type)new_x, (element_type)new_y, (element_type)new_z, (element_type)new_w } {
		}

		template<typename OtherElemT>
		constexpr vec_impl(const vec_impl<component_size, OtherElemT>& other) noexcept
			: vec_impl(other[0], other[1], other[2], other[3]) {
		}

		template<typename OtherElemT>
		constexpr vec_impl& operator=(const vec_impl<component_size, OtherElemT>& other) noexcept {
			data[0] = other[0];
			data[1] = other[1];
			data[2] = other[2];
			data[3] = other[3];
			return *this;
		}

		constexpr vec_impl& operator=(const vec_impl& other) noexcept {
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

		constexpr bool is_zero() const noexcept {
			return math::is_zero(*this);
		}

		template<typename OtherElemT, typename EpsT = promotion_t<element_type, OtherElemT>>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT, EpsT>::value, bool>
			is_near_equal(vec_impl<component_size, OtherElemT>& other, EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename EpsT = element_type>
		std::enable_if_t<is_all_vec_elem<element_type, EpsT>::value, bool>
			is_near_zero(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_equal(*this, other, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			dot(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::dot(*this, other);
		}

		constexpr element_type length_sq() const noexcept {
			return math::length_sq(*this);
		}

		element_type length() const noexcept {
			return math::length(*this);
		}

		constexpr vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			reciprocal() const noexcept {
			return math::reciprocal(*this);
		}

		vec_impl<component_size, promotion_t<default_floating_point_t, element_type>>
			normalize() const noexcept {
			return math::normalize(*this);
		}

		constexpr bool is_normalize() const noexcept {
			return math::is_normalize(*this);
		}

		template<typename EpsT = element_type>
		bool is_near_normalize(EpsT eps = epsilon<EpsT>()) const noexcept {
			return math::is_near_normalize(*this, eps);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance_sq(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance_sq(*this, other);
		}

		template<typename OtherElemT>
		std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, promotion_t<element_type, OtherElemT>>
			distance(const vec_impl<component_size, OtherElemT>& other) const noexcept {
			return math::distance(*this, other);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_all_vec_elem<element_type, OtherElemT>::value, vec_impl<component_size, promotion_t<element_type, OtherElemT>>>
			reflect(const vec_impl<component_size, OtherElemT>& normal_v) const noexcept {
			return math::reflect(*this, normal_v);
		}

		struct constants;
	};

	////////////////////////////////
	// 
	// Constants definition.
	// 
	////////////////////////////////

	template<typename VecElemT>
	struct vec_impl<2, VecElemT>::constants {
		static constexpr vec_impl<2, VecElemT> zero = {};
		static constexpr vec_impl<2, VecElemT> one = { 1, 1 };
		static constexpr vec_impl<2, VecElemT> unit_x = { 1, 0 };
		static constexpr vec_impl<2, VecElemT> unit_y = { 0, 1 };
	};

	template<typename VecElemT>
	struct vec_impl<3, VecElemT>::constants {
		static constexpr vec_impl<3, VecElemT> zero = {};
		static constexpr vec_impl<3, VecElemT> one = { 1, 1, 1 };
		static constexpr vec_impl<3, VecElemT> unit_x = { 1, 0, 0 };
		static constexpr vec_impl<3, VecElemT> unit_y = { 0, 1, 0 };
		static constexpr vec_impl<3, VecElemT> unit_z = { 0, 0, 1 };
	};

	template<typename VecElemT>
	struct vec_impl<4, VecElemT>::constants {
		static constexpr vec_impl<4, VecElemT> zero = {};
		static constexpr vec_impl<4, VecElemT> one = { 1, 1, 1, 1 };
		static constexpr vec_impl<4, VecElemT> unit_x = { 1, 0, 0, 0 };
		static constexpr vec_impl<4, VecElemT> unit_y = { 0, 1, 0, 0 };
		static constexpr vec_impl<4, VecElemT> unit_z = { 0, 0, 1, 0 };
		static constexpr vec_impl<4, VecElemT> unit_w = { 0, 0, 0, 1 };
	};

}//namespace zee::math
}//namespace zee 
