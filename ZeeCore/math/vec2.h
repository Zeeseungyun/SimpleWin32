#pragma once
#include "common.h"
#include "../core/type_supports.h"

namespace zee {
namespace math {
namespace impl {

	template<typename VecElemT, bool IsValidVectorElementType = is_valid_vector_element_type<VecElemT>::value>
	struct vec2_base;

	template<typename VecElemT>
	struct vec2_base<VecElemT, false> {
		vec2_base() noexcept = delete;
		vec2_base(const vec2_base&) noexcept = delete;
		vec2_base(vec2_base&&) noexcept = delete;
		vec2_base& operator=(const vec2_base&) noexcept = delete;
		vec2_base& operator=(vec2_base&&) noexcept = delete;
	};

	template<typename VecElemT>
	struct vec2_base<VecElemT, true> {
		typedef VecElemT element_type;
		element_type x, y;
		
		constexpr vec2_base() noexcept
			: vec2_base(0, 0) {
		}

		constexpr vec2_base(element_type new_x, element_type new_y) noexcept
			: x(new_x), y(new_y) {
		}

		template<typename OtherElemT>
		constexpr vec2_base(const vec2_base<OtherElemT, true>& other) noexcept
			: x((element_type)other.x), y((element_type)other.y) {
		}

		template<typename OtherElemT>
		vec2_base& operator=(const vec2_base<OtherElemT, true>& other) noexcept {
			if (this != &other) {
				x = other.x;
				y = other.y;
			}
			return *this;
		}

		struct constants;

	public:
		template<typename OtherElemT>
		promotion_t<VecElemT, OtherElemT> dot(const vec2_base<OtherElemT, true>& other) noexcept {
			return dot(*this, other);
		}
	};
	
	template<typename VecElemT>
	struct vec2_base<VecElemT, true>::constants {
		typedef VecElemT element_type;
		static constexpr vec2_base<element_type> zero = {};
		static constexpr vec2_base<element_type> one = { 1, 1 };
		static constexpr vec2_base<element_type> unit_x = { 1, 0 };
		static constexpr vec2_base<element_type> unit_y = { 0, 1 };
	};

}// namespace zee::math::impl

	template<typename CheckT, typename ...Args>
	struct is_valid_vec2 : std::false_type { };
	
	template<typename CheckT>
	struct is_valid_vec2<impl::vec2_base<CheckT, true>> : std::true_type { };
	
	template<typename CheckT, typename ...Args>
	struct is_valid_vec2<impl::vec2_base<CheckT, true>, Args...> : std::conditional_t<is_valid_vec2<Args...>::value, std::true_type, std::false_type > { };

	template<typename T, typename ...Args>
	struct vec2_promotion_type {
		typedef void type;
	};

	template<typename T, typename ...Args>
	struct vec2_promotion_type<impl::vec2_base<T>, Args...> {
		typedef typename vec2_promotion_type<Args...>::promotion_type promotion_type;
		typedef impl::vec2_base<decltype(T(0) + promotion_type(0))> type;
	};

	template<typename LeftT, typename RightT>
	struct vec2_promotion_type {
		typedef void type;
	};

	template<typename LeftT, typename RightT>
	struct vec2_promotion_type<impl::vec2_base<LeftT>, impl::vec2_base<RightT>> {
		typedef typename impl::vec2_base<LeftT>::element_type left_element_type;
		typedef typename impl::vec2_base<RightT>::element_type right_element_type;
		typedef decltype(left_element_type(0) + right_element_type(0)) promotion_type;
	};

	template<typename... Args>
	using vec2_promotion_t = typename vec2_promotion_type<Args...>::type;

namespace impl {
	template<typename VecElemT>
	constexpr const vec2_base<VecElemT, true>& 
		operator+(const vec2_base<VecElemT, true>& v) noexcept {
		return v;
	}

	template<typename VecElemT>
	constexpr vec2_base<VecElemT, true> 
		operator-(const vec2_base<VecElemT, true>& v) noexcept {
		return { -v.x, -v.y };
	}

	template<typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_valid_vector_element_type<RightT>::value, vec2_base<promotion_t<LeftElemT, RightT>>>
		operator*(const vec2_base<LeftElemT, true>& l, RightT r) noexcept {
		return { l.x * r , l.y * r };
	}

	template<typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_valid_vector_element_type<LeftT>::value, vec2_base<promotion_t<LeftT, RightElemT>>>
		operator*(LeftT l, const vec2_base<RightElemT, true>& r) noexcept {
		return { l * r.x , l * r.y };
	}

	template<typename LeftElemT, typename RightT>
	constexpr std::enable_if_t<is_valid_vector_element_type<RightT>::value, vec2_base<promotion_t<LeftElemT, RightT>>>
		operator/(const vec2_base<LeftElemT, true>& l, RightT r) noexcept {
		return { l.x / r , l.y / r };
	}

	template<typename LeftT, typename RightElemT>
	constexpr std::enable_if_t<is_valid_vector_element_type<LeftT>::value, vec2_base<promotion_t<LeftT, RightElemT>>>
		operator/(LeftT l, const vec2_base<RightElemT, true>& r) noexcept {
		return { l / r.x , l / r.y };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr vec2_base<promotion_t<LeftElemT, RightElemT>>
		operator+(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return { l.x + r.x , l.y + r.y };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr vec2_base<promotion_t<LeftElemT, RightElemT>>
		operator-(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return { l.x - r.x , l.y - r.y };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool 
		operator==(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return l.x == r.x
			&& l.y == r.y;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool 
		operator!=(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return l.x != r.x
			&& l.y != r.y;
	}

	template<typename LeftElemT, typename RightT>
	std::enable_if_t<is_valid_vector_element_type<RightT>::value, vec2_base<LeftElemT>&>
		operator*=(const vec2_base<LeftElemT, true>& l, RightT r) noexcept {
		l.x *= (LeftElemT)r;
		l.y *= (LeftElemT)r;
		return *this;
	}

	template<typename LeftElemT, typename RightT>
	std::enable_if_t<is_valid_vector_element_type<RightT>::value, vec2_base<LeftElemT>&>
		operator/=(const vec2_base<LeftElemT, true>& l, RightT r) noexcept {
		l.x *= (LeftElemT)r;
		l.y *= (LeftElemT)r;
		return *this;
	}

	template<typename LeftElemT, typename RightElemT>
	vec2_base<LeftElemT, true>& 
		operator+=(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		l.x += (LeftElemT)r.x;
		l.y += (LeftElemT)r.y;
		return *this;
	}

	template<typename LeftElemT, typename RightElemT>
	vec2_base<LeftElemT, true>& 
		operator-=(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		l.x -= (LeftElemT)r.x;
		l.y -= (LeftElemT)r.y;
		return *this;
	}

	//common vector functions..

	template<typename LeftElemT, typename RightElemT>
	constexpr promotion_t<LeftElemT, RightElemT> 
		dot(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return l.x * r.x + l.y * r.y;
	}

	template<typename VecElemT>
	constexpr VecElemT 
		length_sq(const vec2_base<VecElemT, true>& v) noexcept {
		return dot(v, v);
	}

	template<typename VecElemT>
	promotion_t<default_floating_point_t, VecElemT> 
		length(const vec2_base<VecElemT, true>& v) noexcept {
		return sqrt(promotion_t<default_floating_point_t, VecElemT>(length_sq(v)));
	}

	template<typename VecElemT> 
	vec2_base<promotion_t<default_floating_point_t, VecElemT>>
		normalize(const vec2_base<VecElemT, true>& v) noexcept {
		typedef promotion_t<default_floating_point_t, VecElemT> promotion_t;
		const promotion_t rec_len = reciprocal((promotion_t)length(v));
		return {v.x * rec_len  , v.y * rec_len };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr vec2_base<promotion_t<LeftElemT, RightElemT>>
		reflect(const vec2_base<LeftElemT, true>& v, const vec2_base<RightElemT, true>& n) noexcept {
		return v - 2 * dot(v, n) * n;
	}

	//todo:: lerp,

	template<typename VecElemT>
	constexpr bool 
		is_normalize(const vec2_base<VecElemT, true>& v) noexcept {
		return length_sq(v) == 1;
	}

	template<typename VecElemT>
	constexpr bool 
		is_zero(const vec2_base<VecElemT, true>& v) noexcept {
		return v.x == 0 && v.y == 0;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool
		is_equal(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return l == r;
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr bool
		is_not_equal(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return l != r;
	}

	template<typename VecElemT>
	constexpr bool 
		is_not_zero(const vec2_base<VecElemT, true>& v) noexcept {
		return !is_zero(v);
	}

	template<typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	constexpr std::enable_if_t<is_floating_point<VecElemT, EpsT>::value, bool>
		is_near_zero(const vec2_base<VecElemT, true>& v, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return is_near_zero(v.x, eps) && is_near_zero(v.y, eps);
	}

	template<typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	constexpr std::enable_if_t<is_floating_point<VecElemT, EpsT>::value, bool>
		is_near_not_zero(const vec2_base<VecElemT, true>& v, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return !is_near_zero(v, eps);
	}

	template<typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<default_floating_point_t, promotion_t<LeftElemT, RightElemT>>>
	std::enable_if_t<is_floating_point<promotion_t<LeftElemT, RightElemT>, EpsT>::value, bool>
		is_near_equal(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return is_near_equal(l.x, r.x, eps) 
			&& is_near_equal(l.y, r.y, eps);
	}

	template<typename LeftElemT, typename RightElemT, typename EpsT = promotion_t<default_floating_point_t, promotion_t<LeftElemT, RightElemT>>>
	std::enable_if_t<is_floating_point<promotion_t<LeftElemT, RightElemT>, EpsT>::value, bool>
		is_near_not_equal(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return !is_near_equal(l, r, eps);
	}

	template<typename VecElemT, typename EpsT = promotion_t<default_floating_point_t, VecElemT>>
	std::enable_if_t<is_floating_point<VecElemT, EpsT>::value, bool> 
		is_near_normalize(const vec2_base<VecElemT, true>& v, EpsT eps = std::numeric_limits<EpsT>::epsilon()) noexcept {
		return length_sq(v) <= eps;
	}

	template<typename VecElemT>
	vec2_base<VecElemT, true> abs(const vec2_base<VecElemT, true>& v) noexcept {
		return { abs(v.x), abs(v.y) };
	}

	template<typename VecElemT>
	std::enable_if_t<is_floating_point<VecElemT>::value, vec2_base<VecElemT, true>>
		ceil(const vec2_base<VecElemT, true>& v) noexcept {
		return { ceil(v.x), ceil(v.y) };
	}

	template<typename VecElemT>
	std::enable_if_t<is_floating_point<VecElemT>::value, vec2_base<VecElemT, true>>
		floor(const vec2_base<VecElemT, true>& v) noexcept {
		return { floor(v.x), floor(v.y) };
	}

	template<typename VecElemT>
	std::enable_if_t<is_floating_point<VecElemT>::value, vec2_base<VecElemT, true>>
		trunc(const vec2_base<VecElemT, true>& v) noexcept {
		return { trunc(v.x), trunc(v.y) };
	}

	template<typename VecElemT>
	std::enable_if_t<is_floating_point<VecElemT>::value, vec2_base<VecElemT, true>>
		round(const vec2_base<VecElemT, true>& v) noexcept {
		return { round(v.x), round(v.y) };
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr vec2_base<promotion_t<LeftElemT, RightElemT>>
		min(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		typedef promotion_t<LeftElemT, RightElemT> promotion_t;
		return { math::min((promotion_t)l.x, (promotion_t)r.x), math::min((promotion_t)l.y, (promotion_t)r.y) };
	}

	template<typename LeftElemT, typename RightElemT, typename... Args>
	constexpr std::enable_if_t<is_valid_vec2<vec2_base<LeftElemT>, vec2_base<RightElemT>, Args...>::value,
		vec2_promotion_t< vec2_base<LeftElemT>, vec2_base<RightElemT>, Args...>>
		min(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r, const Args& ...args) noexcept {
		return min(min(l, r), args...);
	}

	template<typename LeftElemT, typename RightElemT>
	constexpr vec2_base<promotion_t<LeftElemT, RightElemT>>
		max(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		typedef promotion_t<LeftElemT, RightElemT> promotion_t;
		return { math::max((promotion_t)l.x, (promotion_t)r.x), math::max((promotion_t)l.y, (promotion_t)r.y) };
	}

	template<typename LeftElemT, typename RightElemT, typename... Args>
	constexpr std::enable_if_t<is_valid_vec2<vec2_base<LeftElemT>, vec2_base<RightElemT>, Args...>::value,
		vec2_promotion_t< vec2_base<LeftElemT>, vec2_base<RightElemT>, Args...>>
		max(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r, const Args& ...args) noexcept {
		return max(max(l, r), args...);
	}

	template<typename SrcElemT, typename MinElemT, typename MaxElemT>
	constexpr vec2_base<promotion_t<SrcElemT, MinElemT, MaxElemT>>
		clamp(const vec2_base<SrcElemT, true>& src_v, const const vec2_base<MinElemT, true>& min_v, const const vec2_base<MaxElemT, true>& max_v) noexcept {
		return impl::clamp_impl<vec2_base<promotion_t<SrcElemT, MinElemT, MaxElemT>>>(src_v, min_v, max_v);
	}
	
	//Integer specialization

	template<typename VecElemT>
	constexpr std::enable_if_t<!is_floating_point<VecElemT>::value, bool>
		is_near_normalize(const vec2_base<VecElemT, true>& v, default_floating_point_t unused = 0) noexcept {
		return is_normalize(v);
	}

	template<typename VecElemT>
	constexpr std::enable_if_t<!is_floating_point<VecElemT>::value, bool>
		is_near_zero(const vec2_base<VecElemT, true>& v, default_floating_point_t unused = 0) noexcept {
		return is_zero(v);
	}

	template<typename VecElemT>
	constexpr std::enable_if_t<!is_floating_point<VecElemT>::value, bool>
		is_near_not_zero(const vec2_base<VecElemT, true>& v, default_floating_point_t unused = 0) noexcept {
		return is_not_zero(v);
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<!is_floating_point<promotion_t<LeftElemT, RightElemT>>::value, bool>
		is_near_equal(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r, default_floating_point_t unused = 0) noexcept {
		return l == r;
	}

	template<typename LeftElemT, typename RightElemT>
	std::enable_if_t<!is_floating_point<promotion_t<LeftElemT, RightElemT>>::value, bool>
		is_near_not_equal(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r, default_floating_point_t unused = 0) noexcept {
		return l != r;
	}

	template<typename VecElemT>
	std::enable_if_t<!is_floating_point<VecElemT>::value, const vec2_base<VecElemT, true>&>
		ceil(const vec2_base<VecElemT, true>& v) noexcept {
		return v;
	}

	template<typename VecElemT>
	std::enable_if_t<!is_floating_point<VecElemT>::value, const vec2_base<VecElemT, true>&>
		floor(const vec2_base<VecElemT, true>& v) noexcept {
		return v;
	}

	template<typename VecElemT>
	std::enable_if_t<!is_floating_point<VecElemT>::value, const vec2_base<VecElemT, true>&>
		trunc(const vec2_base<VecElemT, true>& v) noexcept {
		return v;
	}

	template<typename VecElemT>
	std::enable_if_t<!is_floating_point<VecElemT>::value, const vec2_base<VecElemT, true>&>
		round(const vec2_base<VecElemT, true>& v) noexcept {
		return v;
	}

	//vec2f only functions..

	template<typename LeftElemT, typename RightElemT>
	constexpr promotion_t<LeftElemT, RightElemT> 
		ccw(const vec2_base<LeftElemT, true>& l, const vec2_base<RightElemT, true>& r) noexcept {
		return l.x * r.y - l.y * r.x;
	}


}//namespace zee::math::impl

	struct vec2 {
		typedef float element_type;
		element_type x = 0.0f, y = 0.0f;
		struct constants;
	};

	struct vec2::constants {
		static constexpr vec2 zero = { 0.0f, 0.0f };
		static constexpr vec2 unit_x = { 1.0f, 0.0f };
		static constexpr vec2 unit_y = { 0.0f, 1.0f };
	};

	constexpr const vec2& operator+(const vec2& v) noexcept {
		return v;
	}

	constexpr vec2 operator-(const vec2& v) noexcept {
		return { -v.x, -v.y };
	}

	constexpr vec2 operator+(const vec2& l, const vec2& r) noexcept {
		return {l.x + r.x , l.x + r.x };
	}

	constexpr vec2 operator-(const vec2& l, const vec2& r) noexcept {
		return { l.x - r.x , l.y - r.y };
	}

	constexpr vec2 operator*(const vec2& l, vec2::element_type r) noexcept {
		return { l.x * r, l.y * r };
	}

	constexpr vec2 operator*(vec2::element_type l, const vec2& r) noexcept {
		return r * l;
	}

	constexpr vec2 operator/(const vec2& l, vec2::element_type r) noexcept {
		return { l.x / r , l.y / r };
	}

	constexpr vec2 operator/(vec2::element_type l, const vec2& r) noexcept {
		return { l / r.x , l / r.y };
	}

	inline vec2& operator+=(vec2& l, const vec2& r) noexcept {
		l.x += r.x; l.y += r.y;
		return l;
	}

	inline vec2& operator-=(vec2& l, const vec2& r) noexcept {
		l.x -= r.x; l.y -= r.y;
		return l;
	}

	inline vec2& operator*=(vec2& l, vec2::element_type r) noexcept {
		l.x *= r; l.y *= r;
		return l;
	}

	inline vec2& operator/=(vec2& l, vec2::element_type r) noexcept {
		l.x /= r; l.y /= r;
		return l;
	}

	constexpr bool operator==(const vec2& l, const vec2& r) noexcept {
		return l.x == r.x
			&& l.y == r.y;
	}

	constexpr bool operator!=(const vec2& l, const vec2& r) noexcept {
		return !(l == r);
	}

	constexpr bool is_zero(const vec2& v) noexcept {
		return v.x == 0 && v.y == 0;
	}

	constexpr vec2::element_type dot(const vec2& l, const vec2& r) noexcept {
		return l.x* r.x + l.y * r.y;
	}

	inline vec2::element_type length(const vec2& v) noexcept {
		return sqrt(dot(v, v));
	}

	constexpr vec2::element_type length_sq(const vec2& v) noexcept {
		return dot(v, v);
	}

	inline vec2 normalize(const vec2& v) noexcept {
		vec2::element_type len_sq = length_sq(v);
		if (len_sq > 0) {
			len_sq = sqrt(len_sq);
			return { v.x / len_sq, v.y / len_sq };
		}
		return vec2::constants::zero;
	}

	constexpr vec2 reciprocal(const vec2& v) noexcept {
		return { 1.0f / v.x, 1.0f / v.y };
	}

	inline vec2 abs(const vec2& v) noexcept {
		return { abs(v.x), abs(v.y) };
	}

	inline vec2 ceil(const vec2& v) noexcept {
		return { ceil(v.x), ceil(v.y) };
	}

	inline vec2 floor(const vec2& v) noexcept {
		return { floor(v.x), floor(v.y) };
	}

	inline vec2 trunc(const vec2& v) noexcept {
		return { trunc(v.x), trunc(v.y) };
	}

	inline vec2 round(const vec2& v) noexcept {
		return { round(v.x), round(v.y) };
	}

	inline vec2 fmod(const vec2& l, vec2::element_type r) noexcept {
		return { fmod(l.x, r), fmod(l.y,r) };
	}

	inline vec2 fmod(const vec2& l, const vec2& r) noexcept {
		return { fmod(l.x, r.x), fmod(l.y, r.y) };
	}

	constexpr bool is_equal(const vec2& l, const vec2& r) noexcept {
		return l == r;
	}

	constexpr bool is_not_equal(const vec2& l, const vec2& r) noexcept {
		return l != r;
	}

	inline bool is_near_equal(const vec2& l, const vec2& r, vec2::element_type eps = epsilon) noexcept {
		const vec2 result = abs(l - r);
		return result.x < eps && result.y < eps;
	}

	constexpr vec2 min(const vec2& l, const vec2& r) noexcept {
		return { min(l.x, r.x), min(l.y, r.y) };
	}

	constexpr vec2 max(const vec2& l, const vec2& r) noexcept {
		return { max(l.x, r.x), max(l.y, r.y) };
	}

	constexpr vec2 clamp(const vec2& src_v, const vec2& min_v, const vec2& max_v) noexcept {
		return impl::clamp_impl<vec2>(src_v, min_v, max_v);
	}

}//namespace zee::math 
}//namespace zee 