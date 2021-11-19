#pragma once
#include "../math/vec/vec.h"

namespace zee {
namespace shape {

	template<typename ElemT>
	struct circle_base {
		typedef ElemT element_type;
		math::vec<2, ElemT> origin;
		element_type radius;

		constexpr circle_base() noexcept : circle_base(0) { }

		template<typename OriginElemT, typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr circle_base(const math::vec<2, OriginElemT>& new_pt, RadElemT new_radius) noexcept
			: origin(new_pt), radius((element_type)new_radius) {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		constexpr circle_base(const circle_base<OtherElemT>& other) noexcept 
			: circle_base(other.origin, other.radius) { 
		}

		constexpr circle_base& operator=(const circle_base& other) noexcept {
			if (this != &other) {
				origin = other.origin;
				radius = other.radius;
			}
			return *this;
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		circle_base& operator=(const circle_base<OtherElemT>& other) noexcept {
			origin = other.origin;
			radius = other.radius;
			return *this;
		}

		template<typename RadElemT, std::enable_if_t<is_vec_element<RadElemT>::value, int> = 0>
		constexpr explicit circle_base(RadElemT new_radius) noexcept
			: radius((element_type)new_radius) {
		}

		template<typename OtherElemT>
		constexpr bool intersect(const circle_base<OtherElemT>& other) const noexcept {
			const promotion_t<element_type, OtherElemT> rad = radius + other.radius;
			return distance_sq(origin, other.origin) <= rad * rad;
		}
	};

	template<typename LeftElemT, typename RightElemT>
	constexpr bool intersect(const circle_base<LeftElemT>& l, const circle_base<RightElemT>& r) noexcept {
		return l.intersect(r);
	}

	template struct circle_base<float>;
	template struct circle_base<int32>;

	typedef circle_base<float> circlef;
	typedef circle_base<int32> circlei;
}//namespace zee::shape
}//namespace zee