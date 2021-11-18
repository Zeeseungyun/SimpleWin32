#pragma once
#include "../../math/vec/vec.h"

namespace zee {
namespace shape {

	typedef math::vec2i size2;
	typedef math::vec2f size2f;

	template<typename ElemT = float>
	using point_base = math::vec<2, ElemT>;
	typedef point_base<int32> point;
	typedef point_base<int32> pointi;
	typedef point_base<float> pointf;

	template<typename ElemT>
	struct rect_base {
		typedef ElemT element_type;
		union {
			struct {
				element_type left, top, right, bottom;
			};
			math::vec<2, element_type> data[2];
		};

		constexpr rect_base() noexcept : rect_base(0, 0, 0, 0) { }

		template<typename RetT = element_type> constexpr RetT get_left() const noexcept		{ return data[0][0]; }
		template<typename RetT = element_type> constexpr RetT get_top() const noexcept		{ return data[0][1]; }
		template<typename RetT = element_type> constexpr RetT get_right() const noexcept	{ return data[1][0]; }
		template<typename RetT = element_type> constexpr RetT get_bottom() const noexcept	{ return data[1][1]; }

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T,
			std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
		constexpr rect_base(Elem1T new_left, Elem2T new_top, Elem3T new_right, Elem4T new_bottom) noexcept
			: data{ { (element_type)new_left, (element_type)new_top }, { (element_type)new_right, (element_type)new_bottom } } {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T>::value, int> = 0>
		constexpr rect_base(const math::vec<2, Elem1T>& lt, const math::vec<2, Elem2T>& rb) noexcept
			: data{ lt, rb } {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		constexpr rect_base(const rect_base<OtherElemT>& other) noexcept
			: rect_base(
				other.get_left<element_type>(),
				other.get_top<element_type>(),
				other.get_right<element_type>(),
				other.get_bottom<element_type>()) {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_element<OtherElemT>::value, int> = 0>
		rect_base& operator=(const rect_base<OtherElemT>& other) noexcept {
			left = other.get_left<element_type>();
			top = other.get_top<element_type>();
			right = other.get_right<element_type>();
			bottom = other.get_bottom<element_type>();
			return *this;
		}

		constexpr element_type width() const noexcept {
			return get_right() - get_left();
		}

		constexpr element_type height() const noexcept {
			return get_bottom() - get_top();
		}

		constexpr math::vec<2, element_type> size() const noexcept {
			return { width(), height() };
		}

		template<typename ElemT = element_type>
		constexpr 
			std::enable_if_t<is_vec_element<ElemT>::value, math::vec<2, ElemT>>
			origin() const noexcept {
			return {
				(ElemT)get_left() + (ElemT)width() * 0.5,
				(ElemT)get_top() + (ElemT)height() * 0.5
			};
		}

		constexpr rect_base<element_type> normalize() const noexcept {
			return {
				min(get_left(), get_right()), min(get_top(), get_bottom()),
				max(get_left(), get_right()), max(get_top(), get_bottom())
			};
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_element<OtherElemT>::value, bool>
			intersect_rect(const rect_base<OtherElemT>& other) noexcept {
			return !(get_right() < other.get_left() 
				|| get_left() > other.get_right() 
				|| get_bottom() < other.get_top() 
				|| other.get_bottom() < get_top()
				);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_element<OtherElemT>::value, bool>
			intersect_rect_with_normalize(const rect_base<OtherElemT>& other) noexcept {
			const auto n_this = normalize();
			const auto n_other = other.normalize();

			return !(n_this.get_right() < n_other.get_left() 
				|| n_this.get_left() > n_other.get_right() 
				|| n_this.get_bottom() < n_other.get_top() 
				|| n_other.get_bottom() < n_this.get_top()
				);
		}

		template<typename LeftElemT, typename RightElemT>
		constexpr rect_base<promotion_t<LeftElemT, RightElemT>> 
			intersect_rect_area_with_normalize(const rect_base<RightElemT>& other) noexcept {
			const auto n_l = normalize(*this);
			const auto n_r = normalize(other);
			typedef promotion_t<LeftElemT, RightElemT> promotion_t;
			rect_base<promotion_t> ret = {
				max((promotion_t)n_l.get_left(),  (promotion_t)n_r.get_left()) , max((promotion_t)n_l.get_top(),    (promotion_t)n_r.get_top()),
				min((promotion_t)n_l.get_right, (promotion_t)n_r.get_right()), min((promotion_t)n_l.get_bottom(), (promotion_t)n_r.get_bottom())
			};

			if (ret.width() != 0 || ret.height() != 0) {
				return ret;
			}

			return {};
		}

	};
	
	template<typename LeftElemT, typename RightElemT>
	constexpr 
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator+(const rect_base<LeftElemT>& l, const math::vec<2, RightElemT>& r) noexcept {
		return {
			l.get_left()  - r.data[0], l.get_top()    - r.data[1],
			l.get_right() - r.data[0], l.get_bottom() - r.data[1]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator+(const math::vec<2, RightElemT>& l, const rect_base<LeftElemT>& r) noexcept {
		return {
			l.data[0] - r.get_left() , l.data[1] - r.get_top()   ,
			l.data[0] - r.get_right(), l.data[1] - r.get_bottom()
		};
	}
	
	template<typename LeftElemT, typename RightElemT>
	constexpr 
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator-(const rect_base<LeftElemT>& l, const math::vec<2, RightElemT>& r) noexcept {
		return {
			l.get_left()  - r.data[0], l.get_top()    - r.data[1],
			l.get_right() - r.data[0], l.get_bottom() - r.data[1]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator-(const math::vec<2, RightElemT>& l, const rect_base<LeftElemT>& r) noexcept {
		return {
			l.data[0] - r.get_left() , l.data[1] - r.get_top()   ,
			l.data[0] - r.get_right(), l.data[1] - r.get_bottom()
		};
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>&
		operator+=(rect_base<LeftElemT>& l, const math::vec<2, RightElemT>& r) noexcept {
		l.left += (LeftElemT)r.data[0]; l.right  += (LeftElemT)r.data[0];
		l.top  += (LeftElemT)r.data[1]; l.bottom += (LeftElemT)r.data[1];
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>& 
		operator-=(rect_base<LeftElemT>& l, const math::vec<2, RightElemT>& r) noexcept {
		l.left -= (LeftElemT)r.x; l.right  -= (LeftElemT)r.x;
		l.top  -= (LeftElemT)r.y; l.bottom -= (LeftElemT)r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator==(const rect_base<LeftElemT>& l, const rect_base<RightElemT>& r) noexcept {
		return l.data[0] == r.data[0]
			&& l.data[1] == r.data[1];
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator!=(const rect_base<LeftElemT>& l, const rect_base<RightElemT>& r) noexcept {
		return !(l == r);
	}

	template<typename ElemT>
	struct circle_base {
		typedef ElemT element_type;
		math::vec<2, ElemT> pt = {};
		element_type radius = 0;

		constexpr circle_base() noexcept : circle_base(0) { }

		constexpr circle_base(const math::vec<2, ElemT>& new_pt, element_type new_radius) noexcept
			: pt(new_pt), radius(new_radius) {
		}

		constexpr explicit circle_base(element_type new_radius) noexcept
			: pt(math::vec<2, ElemT>{}), radius(new_radius) {
		}
	};

	template<typename ElemT>
	constexpr bool is_zero(const rect_base<ElemT>& rt) noexcept {
		return rt.data[0].is_zero()
			&& rt.data[1].is_zero();
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool intersect(const circle_base<LeftElemT>& l, const circle_base<RightElemT>& r) noexcept {
		const promotion_t<LeftElemT, RightElemT> rad = l.radius + r.radius;
		return distance_sq(l.pt, r.pt) <= rad * rad;
	}

}//namespace zee::shape
}//namespace zee