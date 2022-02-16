#pragma once
#include "../math/vec/vec.h"
#include "boundings.h"
#include "circle_decl.h"

namespace yjj {
namespace shape {
namespace impl {
	struct legal_circle { };

	struct illegal_circle {
	private:
		struct dummy {};

	public:
		typedef void element_type;

	private:
		illegal_circle(dummy) = delete;
	};

	//
	// Declared in circle_decl.h.
	//
	//template<typename ElemT, bool IsValidElemType = is_vec_element<ElemT>::value>
	//struct basic_circle_impl;

	template<typename ElemT>
	struct basic_circle_impl<ElemT, false> : illegal_circle {

	};

	template<typename ElemT>
	struct basic_circle_impl<ElemT, true> : legal_circle {
		typedef ElemT element_type;
		math::basic_vec<2, element_type> origin;
		element_type radius;

		constexpr basic_circle_impl() noexcept : basic_circle_impl(0) { }

		template<typename OriginElemT, typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr basic_circle_impl(const math::impl::vec_impl<2, OriginElemT, true>& new_origin, RadElemT new_radius) noexcept
			: origin(new_origin), radius((element_type)new_radius) {
		}

		template<typename OtherElemT>
		constexpr basic_circle_impl(const basic_circle_impl<OtherElemT, true>& other) noexcept
			: basic_circle_impl(other.origin, other.radius) {
		}

		template<typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr explicit basic_circle_impl(RadElemT new_radius) noexcept
			: radius((element_type)new_radius) {
		}

		constexpr basic_circle_impl& operator=(const basic_circle_impl& other) noexcept {
			if (this != &other) {
				origin = other.origin;
				radius = other.radius;
			}
			return *this;
		}

		template<typename OtherElemT>
		basic_circle_impl& operator=(const basic_circle_impl<OtherElemT, true>& other) noexcept {
			origin = other.origin;
			radius = (element_type)other.radius;
			return *this;
		}
		/*
		template<typename OtherElemT>
		constexpr bool intersect(const basic_circle_impl<OtherElemT, true>& other) const noexcept {
			const promotion_t<element_type, OtherElemT> rad = radius + other.radius;
			return origin.distance_sq(other.origin) <= rad * rad;
		}*/
	};

}//namespace yjj::shape::impl 
	
	template<typename T>
	struct basic_circle : impl::basic_circle_impl<T> {
	private:
		typedef impl::basic_circle_impl<T> base_type;

	public:
		typedef T element_type;

		constexpr basic_circle() noexcept : base_type(0) { }

		template<typename OriginElemT, typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr basic_circle(const math::basic_vec<2, OriginElemT>& new_origin, RadElemT new_radius) noexcept
			: base_type(new_origin, new_radius) {
		}

		template<typename OtherElemT>
		constexpr basic_circle(const impl::basic_circle_impl<OtherElemT, true>& other) noexcept
			: base_type(other.origin, other.radius) {
		}

		template<typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr explicit basic_circle(RadElemT new_radius) noexcept
			: base_type(new_radius) {
		}

		template<typename OtherElemT>
		basic_circle& operator=(const basic_circle<OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		basic_circle& operator=(const impl::basic_circle_impl<OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

	};
	
	template struct basic_circle<float>;
	template struct basic_circle<int32>;

	typedef basic_circle<float> circlef;
	typedef basic_circle<int32> circlei;

}//namespace yjj::shape
}//namespace yjj