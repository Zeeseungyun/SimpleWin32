#pragma once
#include "common.h"
#include "../core/type_supports.h"

namespace zee {
namespace math {
namespace impl {
	template<typename ElemT>
	struct size2_base {
		typedef ElemT element_type;
		element_type width, height;

		constexpr size2_base() noexcept : size2_base(0, 0) { }

		constexpr size2_base(element_type new_width, element_type new_height) noexcept
			: width(new_width), height(new_height) {
		}

		template<typename OtherT>
		constexpr size2_base(const size2_base<OtherT>& other) noexcept
			: width(other.width), height(other.height) {
		}

		template<typename OtherT>
		size2_base& operator=(const size2_base<OtherT>& other) noexcept {
			width = other.width;
			height = other.height;
			return *this;
		}

		constexpr bool is_positive() const noexcept {
			return width >= 0 && height >= 0;
		}

		constexpr bool is_negative() const noexcept {
			return width < 0 || height < 0;
		}

	};

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator==(const size2_base<LeftElemT>& l, const size2_base<RightElemT>& r) noexcept {
		return l.width  == r.width
			&& l.height == l.height;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator!=(const size2_base<LeftElemT>& l, const size2_base<RightElemT>& r) noexcept {
		return !(l == r);
	}

	template<typename ElemT>
	struct point_base {
		typedef ElemT element_type;
		element_type x = 0, y = 0;

		constexpr point_base() noexcept : point_base(0, 0) { }
		constexpr point_base(element_type new_x, element_type new_y) noexcept
			: x(new_x), y(new_y) {
		}

		template<typename OtherT>
		constexpr point_base(const point_base<OtherT>& other) noexcept
			: x(other.x), y(other.y) {
		}

		template<typename OtherT>
		point_base& operator=(const point_base<OtherT>& other) noexcept {
			x = other.x;
			y = other.y;
			return *this;
		}

		struct constants;
	};

	template<typename ElemT>
	struct point_base<ElemT>::constants {
		static constexpr point_base<ElemT> zero = { };
		static constexpr point_base<ElemT> unit_x = { 1, 0 };
		static constexpr point_base<ElemT> unit_y = { 0, 1 };
	};

	template<typename ElemT>
	constexpr const point_base<ElemT>& operator+(const point_base<ElemT>& v) noexcept {
		return v;
	}

	template<typename ElemT>
	constexpr point_base<ElemT> operator-(const point_base<ElemT>& v) noexcept {
		return { -v.x, -v.y };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr point_base<promotion_t<LeftElemT, RightElemT>>
		operator+(const point_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		return { l.x + r.x, l.y + r.y };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr point_base<promotion_t<LeftElemT, RightElemT>> 
		operator-(const point_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		return { l.x - r.x , l.y - r.y };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<std::is_arithmetic<RightElemT>::value, point_base<promotion_t<LeftElemT, RightElemT>>> 
		operator*(const point_base<LeftElemT>& l, RightElemT r) noexcept {
		return { l.x * r , l.y * r };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<std::is_arithmetic<LeftElemT>::value, point_base<promotion_t<LeftElemT, RightElemT>>> 
		operator*(LeftElemT l, const point_base<RightElemT>& r) noexcept {
		return r * l;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<std::is_arithmetic<LeftElemT>::value, point_base<promotion_t<LeftElemT, RightElemT>>> 
		operator/(const point_base<LeftElemT>& l, RightElemT r) noexcept {
		return { l.x / r , l.y / r };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr std::enable_if_t<std::is_arithmetic<LeftElemT>::value, point_base<promotion_t<LeftElemT, RightElemT>>> 
		operator/(LeftElemT l, const point_base<RightElemT>& r) noexcept {
		return { l / r.x, l / r.y };
	}

	template<typename LeftElemT, typename RightElemT>
	inline point_base<LeftElemT>& operator+=(point_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		l.x += r.x; l.y += r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	inline point_base<LeftElemT>& operator-=(point_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		l.x -= r.x; l.y -= r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator==(const point_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		return l.x == r.x
			&& l.y == l.y;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator!=(const point_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		return !(l == r);
	}
	
	template<typename ElemT>
	struct rect_base {
		typedef ElemT element_type;
		element_type left = 0, top = 0, right = 0, bottom = 0;

		constexpr rect_base() noexcept : rect_base(0, 0, 0, 0) { }
		constexpr rect_base(element_type new_left, element_type new_top, element_type new_right, element_type new_bottom) noexcept
			: left(new_left), top(new_top), right(new_right), bottom(new_bottom) {
		}

		template<typename OtherT>
		constexpr rect_base(const rect_base<OtherT>& other) noexcept
			: left(other.left), top(other.top), right(other.right), left(other.bottm) {

		}

		template<typename OtherT>
		rect_base& operator=(const rect_base<OtherT>& other) noexcept {
			left   = other.left;
			top    = other.top;
			right  = other.right;
			bottom = other.bottom;
			return *this;
		}

		constexpr element_type width() const noexcept {
			return right - left;
		}

		constexpr element_type height() const noexcept {
			return bottom - top;
		}

		constexpr size2_base<element_type> size() const noexcept {
			return { width(), height() };
		}

		template<typename ElemT = element_type>
		constexpr point_base<ElemT> origin() const noexcept {
			return {
				(ElemT)left + (ElemT)width() * 0.5,
				(ElemT)top + (ElemT)height() * 0.5
			};
		}

		template<typename OtherT>
		constexpr operator rect_base<OtherT>() const {
			return { (OtherT)left, (OtherT)top, (OtherT)right, (OtherT)bottom };
		}
	};

	template<typename LeftElemT, typename RightElemT>
	constexpr rect_base<promotion_t<LeftElemT, RightElemT>> 
		operator+(const rect_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		return {
			l.left  + r.x, l.top    + r.y,
			l.right + r.x, l.bottom + r.y
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr rect_base<promotion_t<LeftElemT, RightElemT>> 
		operator-(const rect_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		return {
			l.left  - r.x, l.top    - r.y,
			l.right - r.x, l.bottom - r.y
		};
	}

	template<typename LeftElemT, typename RightElemT>
	inline rect_base<LeftElemT>& operator+=(rect_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		l.left += (LeftElemT)r.x; l.right  += (LeftElemT)r.x;
		l.top  += (LeftElemT)r.y; l.bottom += (LeftElemT)r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	inline rect_base<LeftElemT>& operator-=(rect_base<LeftElemT>& l, const point_base<RightElemT>& r) noexcept {
		l.left -= (LeftElemT)r.x; l.right  -= (LeftElemT)r.x;
		l.top  -= (LeftElemT)r.y; l.bottom -= (LeftElemT)r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator==(const rect_base<LeftElemT>& l, const rect_base<RightElemT>& r) noexcept {
		return l.left   == r.left
			&& l.top    == r.top
			&& l.right  == r.right
			&& l.bottom == r.bottom;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator!=(const rect_base<LeftElemT>& l, const rect_base<RightElemT>& r) noexcept {
		return !(l == r);
	}
	
	template<typename ElemT>
	struct circle_base {
		typedef point_base<ElemT> point_type;
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

	template struct size2_base<int32>;
	template struct size2_base<float>;

	template struct point_base<int32>;
	template struct point_base<float>;

	template struct rect_base<int32>;
	template struct rect_base<float>;

	template struct circle_base<int32>;
	template struct circle_base<float>;

}//namespace zee::math::impl 
	template<typename ElemT>
	constexpr bool is_zero(const impl::size2_base<ElemT>& v) noexcept {
		return v.width == 0 && v.height == 0;
	}

	template<typename ElemT>
	constexpr bool is_zero(const impl::point_base<ElemT>& v) noexcept {
		return v.x == 0 && v.y == 0;
	}

	template<typename ElemT>
	constexpr impl::point_base<ElemT> min(const impl::point_base<ElemT>& l, const impl::point_base<ElemT>& r) noexcept {
		return { min(l.x, r.x), min(l.y, r.y) };
	}

	template<typename ElemT>
	constexpr impl::point_base<ElemT> max(const impl::point_base<ElemT>& l, const impl::point_base<ElemT>& r) noexcept {
		return { max(l.x, r.x), max(l.y, r.y) };
	}

	template<typename ElemT>
	constexpr impl::point_base<ElemT> clamp(const impl::point_base<ElemT>& src_v, const impl::point_base<ElemT>& min_v, const impl::point_base<ElemT>& max_v) noexcept {
		return impl::clamp_impl(src_v, min_v, max_v);
	}

	template<typename ElemT>
	inline impl::point_base<ElemT> abs(const impl::point_base<ElemT>& v) noexcept {
		return { abs(v.x), abs(v.y) };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr promotion_t<LeftElemT, RightElemT> distance_sq(const impl::point_base<LeftElemT>& l, const impl::point_base<RightElemT>& r) noexcept {
		auto temp = l - r;
		return temp.x * temp.x + temp.y * temp.y;
	}

	template<typename LeftElemT, typename RightElemT>
	inline promotion_t<LeftElemT, RightElemT> distance(const impl::point_base<LeftElemT>& l, const impl::point_base<RightElemT>& r) noexcept {
		return sqrt(distance_sq(l, r));
	}
	
	template<typename ElemT>
	constexpr bool is_zero(const impl::rect_base<ElemT>& rt) noexcept {
		return rt.left == 0
			&& rt.top == 0
			&& rt.right == 0
			&& rt.bottom == 0;
	}

	template<typename ElemT>
	constexpr impl::rect_base<ElemT> normalize(const impl::rect_base<ElemT>& v) noexcept {
		return {
			min(v.left, v.right), min(v.top, v.bottom),
			max(v.left, v.right), max(v.top, v.bottom)
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool intersect_rect(const impl::rect_base<LeftElemT>& l, const impl::rect_base<RightElemT>& r) noexcept {
		return !(l.right < r.left || l.left > r.right || l.bottom < r.top || r.bottom < l.top);
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
	constexpr bool intersect(const impl::circle_base<LeftElemT>& l, const impl::circle_base<RightElemT>& r) noexcept {
		const promotion_t<LeftElemT, RightElemT> rad = l.radius + r.radius;
		return distance_sq(l.pt, r.pt) <= rad * rad;
	}

	typedef impl::size2_base<int32> size2;
	typedef impl::size2_base<float> size2f;

	typedef impl::point_base<int32> point;
	typedef impl::point_base<float> pointf;

	typedef impl::rect_base<int32> rect;
	typedef impl::rect_base<float> rectf;

	typedef impl::circle_base<int32> circle;
	typedef impl::circle_base<int32> circlef;

}//namespace zee::math 
}//namespace zee