#pragma once
#include "../math/vec/vec.h"
#include "boundings.h"
#include "rect_decl.h"

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
	
	//
	// Declared in rect_decl.h.
	//
	//template<typename ElemT, bool IsValidRect = is_vec_element<ElemT>::value>
	//struct basic_rect_impl;

	template<typename ElemT>
	struct basic_rect_impl<ElemT, false> : illegal_rect {
	};

	template<typename ElemT>
	struct basic_rect_impl<ElemT, true> : legal_rect {
		typedef ElemT element_type;
		union {
			struct {
				element_type left, top, right, bottom;
			};
			math::basic_vec<2, element_type> data[2];
		};

		constexpr basic_rect_impl() noexcept : basic_rect_impl(0, 0, 0, 0) { }

		template<typename RetT = element_type> constexpr RetT get_left() const noexcept { return (RetT)data[0][0]; }
		template<typename RetT = element_type> constexpr RetT get_top() const noexcept { return (RetT)data[0][1]; }
		template<typename RetT = element_type> constexpr RetT get_right() const noexcept { return (RetT)data[1][0]; }
		template<typename RetT = element_type> constexpr RetT get_bottom() const noexcept { return (RetT)data[1][1]; }

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T,
			std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
			constexpr basic_rect_impl(Elem1T new_left, Elem2T new_top, Elem3T new_right, Elem4T new_bottom) noexcept
			: data{ { (element_type)new_left, (element_type)new_top }, { (element_type)new_right, (element_type)new_bottom } } {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T>::value, int> = 0>
		constexpr basic_rect_impl(const math::basic_vec<2, Elem1T>& lt, const math::basic_vec<2, Elem2T>& rb) noexcept
			: data{ lt, rb } {
		}

		template<typename OtherElemT>
		constexpr basic_rect_impl(const basic_rect_impl<OtherElemT, true>& other) noexcept
			: basic_rect_impl(
				other.get_left<element_type>(),
				other.get_top<element_type>(),
				other.get_right<element_type>(),
				other.get_bottom<element_type>()) {
		}

		template<typename OtherElemT>
		basic_rect_impl& operator=(const basic_rect_impl<OtherElemT, true>& other) noexcept {
			left = other.get_left<element_type>();
			top = other.get_top<element_type>();
			right = other.get_right<element_type>();
			bottom = other.get_bottom<element_type>();
			return *this;
		}

		template<typename OtherElemT>
		basic_rect_impl& operator=(basic_rect_impl<OtherElemT, true>&& other) noexcept {
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

		constexpr math::basic_vec<2, element_type> size() const noexcept {
			return { width(), height() };
		}

		template<typename ElemT = element_type>
		constexpr std::enable_if_t<is_vec_element<ElemT>::value, math::basic_vec<2, ElemT>>
			origin() const noexcept {
			return {
				(ElemT)get_left() + (ElemT)width() * 0.5,
				(ElemT)get_top() + (ElemT)height() * 0.5
			};
		}

		constexpr basic_rect_impl<element_type> normalize() const noexcept {
			return {
				math::min(get_left(), get_right()), math::min(get_top(), get_bottom()),
				math::max(get_left(), get_right()), math::max(get_top(), get_bottom())
			};
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_element<OtherElemT>::value, bool>
			intersect_rect(const basic_rect_impl<OtherElemT>& other) const noexcept {
			return !(get_right() < other.get_left()
				|| get_left() > other.get_right()
				|| get_bottom() < other.get_top()
				|| other.get_bottom() < get_top()
				);
		}

		template<typename OtherElemT>
		constexpr std::enable_if_t<is_vec_element<OtherElemT>::value, bool>
			intersect_rect_with_normalize(const basic_rect_impl<OtherElemT, true>& other) const noexcept {
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
			intersect_rect_area(const basic_rect_impl<OtherElemT, true>& other) const noexcept {
			typedef promotion_t<element_type, OtherElemT> promotion_t;
			const basic_rect_impl<promotion_t> ret = {
				math::max(get_left(),		other.get_left())  ,
				math::max(get_top(),		other.get_top())   ,
				math::min(get_right(),		other.get_right()) ,
				math::min(get_bottom(),		other.get_bottom())
			};

			if (ret.width() != 0 || ret.height() != 0) {
				return ret;
			}

			return basic_rect_impl<promotion_t>{};
		}

		template<typename OtherElemT>
		constexpr auto
			intersect_rect_area_with_normalize(const basic_rect_impl<OtherElemT, true>& other) const noexcept {
			const auto n_l = normalize();
			const auto n_r = other.normalize();
			typedef promotion_t<OtherElemT> promotion_t;
			const basic_rect_impl<promotion_t> ret = {
				math::max(n_l.get_left(),		n_r.get_left())  ,
				math::max(n_l.get_top(),		n_r.get_top())   ,
				math::min(n_l.get_right(),		n_r.get_right()) ,
				math::min(n_l.get_bottom(),		n_r.get_bottom())
			};

			if (ret.width() != 0 || ret.height() != 0) {
				return ret;
			}

			return basic_rect_impl<promotion_t>{};
		}

	};

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, basic_rect_impl<promotion_t<LeftElemT, RightElemT>>>
		operator+(const basic_rect_impl<LeftElemT, true>& l, const math::basic_vec<2, RightElemT>& r) noexcept {
		return {
			l.get_left() + r.data[0], l.get_top() + r.data[1],
			l.get_right() + r.data[0], l.get_bottom() + r.data[1]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, basic_rect_impl<promotion_t<LeftElemT, RightElemT>>>
		operator+(const math::basic_vec<2, RightElemT>& l, const basic_rect_impl<LeftElemT, true>& r) noexcept {
		return {
			l.data[0] + r.get_left() , l.data[1] + r.get_top()   ,
			l.data[0] + r.get_right(), l.data[1] + r.get_bottom()
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, basic_rect_impl<promotion_t<LeftElemT, RightElemT>>>
		operator-(const basic_rect_impl<LeftElemT, true>& l, const math::basic_vec<2, RightElemT>& r) noexcept {
		return {
			l.get_left() - r.data[0], l.get_top() - r.data[1],
			l.get_right() - r.data[0], l.get_bottom() - r.data[1]
		};
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr
		std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, basic_rect_impl<promotion_t<LeftElemT, RightElemT>>>
		operator-(const math::basic_vec<2, RightElemT>& l, const basic_rect_impl<LeftElemT, true>& r) noexcept {
		return {
			l.data[0] - r.get_left() , l.data[1] - r.get_top()   ,
			l.data[0] - r.get_right(), l.data[1] - r.get_bottom()
		};
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, basic_rect_impl<promotion_t<LeftElemT, RightElemT>>>&
		operator+=(basic_rect_impl<LeftElemT, true>& l, const math::basic_vec<2, RightElemT>& r) noexcept {
		l.left += (LeftElemT)r.data[0]; l.right += (LeftElemT)r.data[0];
		l.top += (LeftElemT)r.data[1]; l.bottom += (LeftElemT)r.data[1];
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<are_all_vec_element<LeftElemT, RightElemT>::value, basic_rect_impl<promotion_t<LeftElemT, RightElemT>>>&
		operator-=(basic_rect_impl<LeftElemT, true>& l, const math::basic_vec<2, RightElemT>& r) noexcept {
		l.left -= (LeftElemT)r.x; l.right -= (LeftElemT)r.x;
		l.top -= (LeftElemT)r.y; l.bottom -= (LeftElemT)r.y;
		return l;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator==(const basic_rect_impl<LeftElemT, true>& l, const basic_rect_impl<RightElemT, true>& r) noexcept {
		return l.data[0] == r.data[0]
			&& l.data[1] == r.data[1];
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool operator!=(const basic_rect_impl<LeftElemT, true>& l, const basic_rect_impl<RightElemT, true>& r) noexcept {
		return !(l == r);
	}

}//namespace zee::shape::impl

	template<typename T>
	struct basic_rect : impl::basic_rect_impl<T> {
	private:
		typedef impl::basic_rect_impl<T> base_type;

	public:
		typedef T element_type;

		constexpr basic_rect() noexcept : base_type(0, 0, 0, 0) { }

		template<typename Elem1T, typename Elem2T, typename Elem3T, typename Elem4T,
			std::enable_if_t<are_all_vec_element<Elem1T, Elem2T, Elem3T, Elem4T>::value, int> = 0>
			constexpr basic_rect(Elem1T new_left, Elem2T new_top, Elem3T new_right, Elem4T new_bottom) noexcept
			: base_type(new_left, new_top, new_right, new_bottom) {
		}

		template<typename Elem1T, typename Elem2T, std::enable_if_t<are_all_vec_element<Elem1T, Elem2T>::value, int> = 0>
		constexpr basic_rect(const math::basic_vec<2, Elem1T>& lt, const math::basic_vec<2, Elem2T>& rb) noexcept
			: base_type(lt, rb) {
		}

		template<typename OtherElemT>
		constexpr basic_rect(const basic_rect<OtherElemT>& other) noexcept
			: base_type(
				other.get_left<element_type>(),
				other.get_top<element_type>(),
				other.get_right<element_type>(),
				other.get_bottom<element_type>()) {
		}

		template<typename OtherElemT>
		constexpr basic_rect(const impl::basic_rect_impl<OtherElemT, true>& other) noexcept
			: base_type(other) {
		}

		template<typename OtherElemT>
		basic_rect& operator=(const basic_rect<OtherElemT>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		basic_rect& operator=(basic_rect<OtherElemT>&& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		basic_rect& operator=(const impl::basic_rect_impl<OtherElemT, true>& other) noexcept {
			base_type::operator=(other);
			return *this;
		}

		template<typename OtherElemT>
		basic_rect& operator=(impl::basic_rect_impl<OtherElemT, true>&& other) noexcept {
			base_type::operator=(other);
			return *this;
		}
	};

	template struct basic_rect<float>;
	template struct basic_rect<int32>;

	typedef basic_rect<float> rectf;
	typedef basic_rect<int32> recti;

}//namespace zee::shape
}//namespace zee