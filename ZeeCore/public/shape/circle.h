#pragma once
#include "../math/vec_base/vec_base.h"
#include "boundings.h"

namespace zee {
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

	template<typename ElemT, bool IsValidElemType = is_vec_element<ElemT>::value>
	struct circle_base_impl;

	template<typename ElemT>
	struct circle_base_impl<ElemT, false> : illegal_circle {

	};

	template<typename ElemT>
	struct circle_base_impl<ElemT, true> : legal_circle {
		typedef ElemT element_type;
		math::vec_base<2, element_type> origin;
		element_type radius;

		constexpr circle_base_impl() noexcept : circle_base_impl(0) { }

		template<typename OriginElemT, typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr circle_base_impl(const math::impl::vec_impl<2, OriginElemT, true>& new_origin, RadElemT new_radius) noexcept
			: origin(new_origin), radius((element_type)new_radius) {
		}

		template<typename OtherElemT>
		constexpr circle_base_impl(const circle_base_impl<OtherElemT, true>& other) noexcept
			: circle_base_impl(other.origin, other.radius) {
		}

		template<typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr explicit circle_base_impl(RadElemT new_radius) noexcept
			: radius((element_type)new_radius) {
		}

		constexpr circle_base_impl& operator=(const circle_base_impl& other) noexcept {
			if (this != &other) {
				origin = other.origin;
				radius = other.radius;
			}
			return *this;
		}

		template<typename OtherElemT>
		circle_base_impl& operator=(const circle_base_impl<OtherElemT, true>& other) noexcept {
			origin = other.origin;
			radius = other.radius;
			return *this;
		}

		template<typename OtherElemT>
		constexpr bool intersect(const circle_base_impl<OtherElemT, true>& other) const noexcept {
			const promotion_t<element_type, OtherElemT> rad = radius + other.radius;
			return origin.distance_sq(other.origin) <= rad * rad;
		}
	};

}//namespace zee::shape::impl 
	
	template<typename T>
	struct circle_base : impl::circle_base_impl<T> {
	private:
		typedef impl::circle_base_impl<T> base_type;

	public:
		typedef T element_type;

		constexpr circle_base() noexcept : base_type(0) { }

		template<typename OriginElemT, typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr circle_base(const math::vec_base<2, OriginElemT>& new_origin, RadElemT new_radius) noexcept
			: base_type(new_origin, new_radius) {
		}

		template<typename OtherElemT>
		constexpr circle_base(const impl::circle_base_impl<OtherElemT, true>& other) noexcept
			: base_type(other.origin, other.radius) {
		}

		template<typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr explicit circle_base(RadElemT new_radius) noexcept
			: base_type(new_radius) {
		}

		template<typename OtherElemT>
		circle_base& operator=(const circle_base<OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		circle_base& operator=(const impl::circle_base_impl<OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

	};
	
	template struct circle_base<float>;
	template struct circle_base<int32>;

	typedef circle_base<float> circlef;
	typedef circle_base<int32> circlei;

}//namespace zee::shape
}//namespace zee