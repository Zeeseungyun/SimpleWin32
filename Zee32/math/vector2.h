#pragma once
#include "common.h"

namespace zee {
namespace math {
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
		return impl::clamp_impl(src_v, min_v, max_v);
	}

}//namespace zee::math 
}//namespace zee 