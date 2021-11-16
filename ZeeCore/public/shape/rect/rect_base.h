#pragma once
#include "../../math/vec/vec.h"

namespace zee {
namespace math {

	typedef vec2i size2;
	typedef vec2f size2f;

	template<typename ElemT = float>
	using point_base = vec<2, ElemT>;
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
			point_base<element_type> data[2];
		};

		constexpr rect_base() noexcept : rect_base(0, 0, 0, 0) { }

		template<typename RetT = element_type> constexpr RetT get_left() const noexcept { return data[0][0]; }
		template<typename RetT = element_type> constexpr RetT get_top() const noexcept { return data[0][1]; }
		template<typename RetT = element_type> constexpr RetT get_right() const noexcept { return data[1][0]; }
		template<typename RetT = element_type> constexpr RetT get_bottom() const noexcept { return data[1][1]; }

		template<typename OtherElemT, std::enable_if_t<is_vec_elem<OtherElemT>::value, int> = 0>
		constexpr rect_base(OtherElemT new_left, OtherElemT new_top, OtherElemT new_right, OtherElemT new_bottom) noexcept
			: data{ { (element_type)new_left, (element_type)new_top }, { (element_type)new_right, (element_type)new_bottom } } {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_elem<OtherElemT>::value, int> = 0>
		constexpr rect_base(const vec<2, OtherElemT>& lt, const vec<2, OtherElemT>& rb) noexcept
			: data{ lt, rb } {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_elem<OtherElemT>::value, int> = 0>
		constexpr rect_base(const rect_base<OtherElemT>& other) noexcept
			: rect_base(
				other.get_left<element_type>(),
				other.get_top<element_type>(),
				other.get_right<element_type>(),
				other.get_bottom<element_type>()) {
		}

		template<typename OtherElemT, std::enable_if_t<is_vec_elem<OtherElemT>::value, int> = 0>
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

		constexpr vec<2, element_type> size() const noexcept {
			return { width(), height() };
		}

		template<typename ElemT = element_type>
		constexpr 
			std::enable_if_t<is_vec_elem<ElemT>::value, vec<2, ElemT>>
			origin() const noexcept {
			return {
				(ElemT)get_left() + (ElemT)width() * 0.5,
				(ElemT)get_top() + (ElemT)height() * 0.5
			};
		}

		constexpr rect_base<element_type> normalize() const noexcept {
			return {
				min(v.get_left(), v.get_right()), min(v.get_top(), v.get_bottom()),
				max(v.get_left(), v.get_right()), max(v.get_top(), v.get_bottom())
			};
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_elem<OtherElemT>::value, bool>
			bool intersect_rect(const rect_base<OtherElemT>& other) noexcept {
			return !(get_right() < other.get_left() 
				|| get_left() > other.get_right() 
				|| get_bottom() < other.get_top() 
				|| other.get_bottom() < get_top()
				);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_elem<OtherElemT>::value, bool>
			bool intersect_rect_with_normalize(const rect_base<OtherElemT>& other) noexcept {
			const auto n_this = normalize();
			const auto n_other = other.normalize();

			return !(n_this.get_right() < n_other.get_left() 
				|| n_this.get_left() > n_other.get_right() 
				|| n_this.get_bottom() < n_other.get_top() 
				|| n_other.get_bottom() < n_this.get_top()
				);
		}

		template<typename LeftElemT, typename RightElemT>
		constexpr rect_base<promotion_t<LeftElemT, RightElemT>> intersect_rect_area_with_normalize(const rect_base<RightElemT>& other) noexcept {
			const auto n_l = normalize(l);
			const auto n_r = normalize(r);
			typedef promotion_t<LeftElemT, RightElemT> promotion_t;
			impl::rect_base<promotion_t> ret = {
				max((promotion_t)n_l.left,  (promotion_t)n_r.left) , max((promotion_t)n_l.top,    (promotion_t)n_r.top),
				min((promotion_t)n_l.right, (promotion_t)n_r.right), min((promotion_t)n_l.bottom, (promotion_t)n_r.bottom)
			};

			if (ret.width() != 0 || ret.height() != 0) {
				return ret;
			}

			return {};
		}

	};
	
	template<typename LeftElemT, typename RightElemT>
	constexpr 
		std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator+(const rect_base<LeftElemT>& l, const vec<2, RightElemT>& r) noexcept {
		return {
			l.get_left()  - r[axis_idx::x], l.get_top()    - r[axis_idx::y],
			l.get_right() - r[axis_idx::x], l.get_bottom() - r[axis_idx::y]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator+(const vec<2, RightElemT>& l, const rect_base<LeftElemT>& r) noexcept {
		return {
			l[axis_idx::x] - r.get_left() , l[axis_idx::y] - r.get_top()   ,
			l[axis_idx::x] - r.get_right(), l[axis_idx::y] - r.get_bottom()
		};
	}
	
	template<typename LeftElemT, typename RightElemT>
	constexpr 
		std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator-(const rect_base<LeftElemT>& l, const vec<2, RightElemT>& r) noexcept {
		return {
			l.get_left()  - r[axis_idx::x], l.get_top()    - r[axis_idx::y],
			l.get_right() - r[axis_idx::x], l.get_bottom() - r[axis_idx::y]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>
		operator-(const vec<2, RightElemT>& l, const rect_base<LeftElemT>& r) noexcept {
		return {
			l[axis_idx::x] - r.get_left() , l[axis_idx::y] - r.get_top()   ,
			l[axis_idx::x] - r.get_right(), l[axis_idx::y] - r.get_bottom()
		};
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>&
		operator+=(rect_base<LeftElemT>& l, const vec<2, RightElemT>& r) noexcept {
		l.left += (LeftElemT)r[axis_idx::x]; l.right  += (LeftElemT)r[axis_idx::x];
		l.top  += (LeftElemT)r[axis_idx::y]; l.bottom += (LeftElemT)r[axis_idx::y];
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<is_all_vec_elem<LeftElemT, RightElemT>::value, rect_base<promotion_t<LeftElemT, RightElemT>>>& 
		operator-=(rect_base<LeftElemT>& l, const vec<2, RightElemT>& r) noexcept {
		l.left -= (LeftElemT)r.x; l.right -= (LeftElemT)r.x;
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
		typedef vec<2, ElemT> point_type;
		typedef ElemT element_type;
		point_type pt = {};
		element_type radius = 0;

		constexpr circle_base() noexcept : circle_base(0) { }

		constexpr circle_base(const point_type& new_pt, element_type new_radius) noexcept
			: pt(new_pt), radius(new_radius) {
		}

		constexpr explicit circle_base(element_type new_radius) noexcept
			: pt(point_type{}), radius(new_radius) {
		}
	};

	template<typename ElemT>
	constexpr bool is_zero(const rect_base<ElemT>& rt) noexcept {
		return rt.data[0].is_zero()
			&& rt.data[1].is_zero();
	}


	template<typename LeftElemT, typename RightElemT>
	constexpr impl::rect_base<promotion_t<LeftElemT, RightElemT>> intersect_rect_area(const impl::rect_base<LeftElemT>& l, const impl::rect_base<RightElemT>& r) noexcept {
		typedef promotion_t<LeftElemT, RightElemT> promotion_t;
		impl::rect_base<promotion_t> ret = {
			max((promotion_t)l.left,  (promotion_t)r.left) , max((promotion_t)l.top,    (promotion_t)r.top),
			min((promotion_t)l.right, (promotion_t)r.right), min((promotion_t)l.bottom, (promotion_t)r.bottom)
		};
		
		if (ret.size().is_positive()) {
			return ret;
		}

		return {};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool intersect_rect_with_normalize(const impl::rect_base<LeftElemT>& l, const impl::rect_base<RightElemT>& r) noexcept {
		const auto n_l = normalize(l); 
		const auto n_r = normalize(r);
		return !(n_l.right < n_r.left || n_l.left > n_r.right || n_l.bottom < n_r.top || n_r.bottom < n_l.top);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr impl::rect_base<promotion_t<LeftElemT, RightElemT>> intersect_rect_area_with_normalize(const impl::rect_base<LeftElemT>& l, const impl::rect_base<RightElemT>& r) noexcept {
		const auto n_l = normalize(l); 
		const auto n_r = normalize(r);
		typedef promotion_t<LeftElemT, RightElemT> promotion_t;
		impl::rect_base<promotion_t> ret = {
			max((promotion_t)n_l.left,  (promotion_t)n_r.left) , max((promotion_t)n_l.top,    (promotion_t)n_r.top),
			min((promotion_t)n_l.right, (promotion_t)n_r.right), min((promotion_t)n_l.bottom, (promotion_t)n_r.bottom)
		};

		if (ret.width() != 0 || ret.height() != 0) {
			return ret;
		}

		return {};
	}

	template<typename RectElemT>
	constexpr impl::point_base<RectElemT> origin(const impl::rect_base<RectElemT>& v) noexcept {
		return v.origin();
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool intersect(const circle_base<LeftElemT>& l, const circle_base<RightElemT>& r) noexcept {
		const promotion_t<LeftElemT, RightElemT> rad = l.radius + r.radius;
		return distance_sq(l.pt, r.pt) <= rad * rad;
	}

}//namespace zee::math 
}//namespace zee