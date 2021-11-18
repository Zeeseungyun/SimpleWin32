#pragma once
#include "vec_impl.h"
namespace zee {
namespace math {
		
	////////////////////////////////
	// 
	// Explicit instantiation.
	// 
	////////////////////////////////

	template struct vec<2, int32>;
	template struct vec<2, float>;
	template struct vec<2, double>;

	template struct vec<3, int32>;
	template struct vec<3, float>;
	template struct vec<3, double>;

	template struct vec<4, int32>;
	template struct vec<4, float>;
	template struct vec<4, double>;

	////////////////////////////////
	// 
	// Type redefinition.
	// 
	////////////////////////////////

	typedef vec<2, float>  vec2;
	typedef vec<2, int32>  vec2i;
	typedef vec<2, float>  vec2f;
	typedef vec<2, double> vec2df;

	typedef vec<3, float>  vec3;
	typedef vec<3, int32>  vec3i;
	typedef vec<3, float>  vec3f;
	typedef vec<3, double> vec3df;

	typedef vec<4, float>  vec4;
	typedef vec<4, int32>  vec4i;
	typedef vec<4, float>  vec4f;
	typedef vec<4, double> vec4df;

}//namespace zee::math
}//namespace zee

void test() {
	constexpr zee::math::vec2 v1 = { -1, 0 };
	constexpr zee::math::vec2 v2 = { 0, 1 };
	constexpr zee::math::vec3 v3 = { -1, 0, 0 };
	constexpr zee::math::vec3 v4 = { 0, 1, 0 };
	constexpr zee::math::vec2f v5 = { 10.0, -2.5 };
	constexpr zee::math::vec3f v6 = { 10.0, -2.5 , 0};
}
