#pragma once
#include "../math/vec/vec.h"
#include "boundings.h"

namespace zee {
namespace shape {

	typedef math::vec2i size2;
	typedef math::vec2f size2f;

namespace impl {

	struct legal_rect { };

	struct illegal_rect {
	private:
		struct dummy {};

	public:
		typedef void element_type;

	private:
		illegal_rect(dummy) = delete;
	};

	template<typename ElemT, bool IsValidRect = is_vec_element<ElemT>::value>
	struct rect_base_impl;

	template<typename ElemT>
	struct rect_base_impl<ElemT, false> : illegal_rect {
	};

	template<typename ElemT>
	struct rect_base_impl<ElemT, true> : legal_rect {
		typedef ElemT element_type;
		union {
			struct {
				element_type left, top, right, bottom;
			};
			math::vec_base<2, element_type> data[2];
		};

		constexpr rect_base_impl() noexcept : rect_base_impl(0, 0, 0, 0) { }

		template<typename RetT = element_type> constexpr RetT get_left() const noexcept { return data[0][0]; }
		template<typename RetT = element_type> constexpr RetT get_top() const noexcept { return data[0][1]; }
		template<typename RetT = element_type> constexpr RetT get_right() const noexcept { return data[1][0]; }
		template<typename RetT = element_type> constexpr RetT get_bottom() const noexcept { return data[1][1]; }

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T,
			std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
			constexpr rect_base_impl(Elem1T new_left, Elem2T new_top, Elem3T new_right, Elem4T new_bottom) noexcept
			: data{ { (element_type)new_left, (element_type)new_top }, { (element_type)new_right, (element_type)new_bottom } } {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T>::value, int> = 0>
		constexpr rect_base_impl(const math::vec_base<2, Elem1T>& lt, const math::vec_base<2, Elem2T>& rb) noexcept
			: data{ lt, rb } {
		}

		template<typename OtherElemT>
		constexpr rect_base_impl(const rect_base_impl<OtherElemT, true>& other) noexcept
			: rect_base_impl(
				other.get_left<element_type>(),
				other.get_top<element_type>(),
				other.get_right<element_type>(),
				other.get_bottom<element_type>()) {
		}

		template<typename OtherElemT>
		rect_base_impl& operator=(const rect_base_impl<OtherElemT, true>& other) noexcept {
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

		constexpr math::vec_base<2, element_type> size() const noexcept {
			return { width(), height() };
		}

		template<typename ElemT = element_type>
		constexpr std::enable_if_t<is_vec_element<ElemT>::value, math::vec_base<2, ElemT>>
			origin() const noexcept {
			return {
				(ElemT)get_left() + (ElemT)width() * 0.5,
				(ElemT)get_top() + (ElemT)height() * 0.5
			};
		}

		constexpr rect_base_impl<element_type> normalize() const noexcept {
			return {
				math::min(get_left(), get_right()), math::min(get_top(), get_bottom()),
				math::max(get_left(), get_right()), math::max(get_top(), get_bottom())
			};
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_element<OtherElemT>::value, bool>
			intersect_rect(const rect_base_impl<OtherElemT>& other) const noexcept {
			return !(get_right() < other.get_left()
				|| get_left() > other.get_right()
				|| get_bottom() < other.get_top()
				|| other.get_bottom() < get_top()
				);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_element<OtherElemT>::value, bool>
			intersect_rect_with_normalize(const rect_base_impl<OtherElemT, true>& other) const noexcept {
			const auto n_this = normalize();
			const auto n_other = other.normalize();

			return !(n_this.get_right() < n_other.get_left()
				|| n_this.get_left() > n_other.get_right()
				|| n_this.get_bottom() < n_other.get_top()
				|| n_other.get_bottom() < n_this.get_top()
				);
		}

		template<typename OtherElemT>
		constexpr auto
			intersect_rect_area(const rect_base_impl<OtherElemT, true>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			const rect_base_impl<promotion_t> ret = {
				math::max(get_left(),		other.get_left())  ,
				math::max(get_top(),		other.get_top())   ,
				math::min(get_right(),		other.get_right()) ,
				math::min(get_bottom(),		other.get_bottom())
			};

			if (ret.width() != 0 || ret.height() != 0) {
				return ret;
			}

			return rect_base_impl<promotion_t>{};
		}

		template<typename OtherElemT>
		constexpr auto
			intersect_rect_area_with_normalize(const rect_base_impl<OtherElemT, true>& other) const noexcept {
			const auto n_l = normalize();
			const auto n_r = other.normalize();
			typedef promotion_t<OtherElemT> promotion_t;
			const rect_base_impl<promotion_t> ret = {
				math::max(n_l.get_left(),		n_r.get_left())  ,
				math::max(n_l.get_top(),		n_r.get_top())   ,
				math::min(n_l.get_right(),		n_r.get_right()) ,
				math::min(n_l.get_bottom(),		n_r.get_bottom())
			};

			if (ret.width() != 0 || ret.height() != 0) {
				return ret;
			}

			return rect_base_impl<promotion_t>{};
		}

	};

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base_impl<promotion_t<LeftElemT, RightElemT>>>
		operator+(const rect_base_impl<LeftElemT, true>& l, const math::vec_base<2, RightElemT>& r) noexcept {
		return {
			l.get_left() + r.data[0], l.get_top() + r.data[1],
			l.get_right() + r.data[0], l.get_bottom() + r.data[1]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base_impl<promotion_t<LeftElemT, RightElemT>>>
		operator+(const math::vec_base<2, RightElemT>& l, const rect_base_impl<LeftElemT, true>& r) noexcept {
		return {
			l.data[0] + r.get_left() , l.data[1] + r.get_top()   ,
			l.data[0] + r.get_right(), l.data[1] + r.get_bottom()
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base_impl<promotion_t<LeftElemT, RightElemT>>>
		operator-(const rect_base_impl<LeftElemT, true>& l, const math::vec_base<2, RightElemT>& r) noexcept {
		return {
			l.get_left() - r.data[0], l.get_top() - r.data[1],
			l.get_right() - r.data[0], l.get_bottom() - r.data[1]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base_impl<promotion_t<LeftElemT, RightElemT>>>
		operator-(const math::vec_base<2, RightElemT>& l, const rect_base_impl<LeftElemT, true>& r) noexcept {
		return {
			l.data[0] - r.get_left() , l.data[1] - r.get_top()   ,
			l.data[0] - r.get_right(), l.data[1] - r.get_bottom()
		};
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base_impl<promotion_t<LeftElemT, RightElemT>>>&
		operator+=(rect_base_impl<LeftElemT, true>& l, const math::vec_base<2, RightElemT>& r) noexcept {
		l.left += (LeftElemT)r.data[0]; l.right += (LeftElemT)r.data[0];
		l.top += (LeftElemT)r.data[1]; l.bottom += (LeftElemT)r.data[1];
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, rect_base_impl<promotion_t<LeftElemT, RightElemT>>>&
		operator-=(rect_base_impl<LeftElemT, true>& l, const math::vec_base<2, RightElemT>& r) noexcept {
		l.left -= (LeftElemT)r.x; l.right -= (LeftElemT)r.x;
		l.top -= (LeftElemT)r.y; l.bottom -= (LeftElemT)r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator==(const rect_base_impl<LeftElemT, true>& l, const rect_base_impl<RightElemT, true>& r) noexcept {
		return l.data[0] == r.data[0]
			&& l.data[1] == r.data[1];
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator!=(const rect_base_impl<LeftElemT, true>& l, const rect_base_impl<RightElemT, true>& r) noexcept {
		return !(l == r);
	}

}//namespace zee::shape::impl

	template<typename T>
	struct rect_base : impl::rect_base_impl<T> {
	private:
		typedef impl::rect_base_impl<T> base_type;

	public:
		typedef T element_type;

		constexpr rect_base() noexcept : base_type(0, 0, 0, 0) { }

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T,
			std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
			constexpr rect_base(Elem1T new_left, Elem2T new_top, Elem3T new_right, Elem4T new_bottom) noexcept
			: base_type(new_left, new_top, new_right, new_bottom) {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T>::value, int> = 0>
		constexpr rect_base(const math::vec_base<2, Elem1T>& lt, const math::vec_base<2, Elem2T>& rb) noexcept
			: base_type(lt, rb) {
		}

		template<typename OtherElemT>
		constexpr rect_base(const rect_base<OtherElemT>& other) noexcept
			: base_type(
				other.get_left<element_type>(),
				other.get_top<element_type>(),
				other.get_right<element_type>(),
				other.get_bottom<element_type>()) {
		}

		template<typename OtherElemT>
		constexpr rect_base(const impl::rect_base_impl<OtherElemT, true>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		rect_base& operator=(const rect_base<OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		rect_base& operator=(const impl::rect_base_impl<OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

	};

	template struct rect_base<float>;
	template struct rect_base<int32>;

	typedef rect_base<float> rectf;
	typedef rect_base<int32> recti;

}//namespace zee::shape
}//namespace zee