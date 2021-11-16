#pragma once
#include "vec_base.h"
#include "vec_common_funcs.h"
namespace zee {
namespace math {
		
	////////////////////////////////
	// 
	// Explicit instantiation.
	// 
	////////////////////////////////

	template struct vec_base<2, int32>;
	template struct vec_base<2, float>;
	//template struct vec_base<2, double>;

	template struct vec_base<3, int32>;
	template struct vec_base<3, float>;
	//template struct vec_base<3, double>;

	template struct vec_base<4, int32>;
	template struct vec_base<4, float>;
	//template struct vec_base<4, double>;
	
	////////////////////////////////
	// 
	// Type redefinition.
	// 
	////////////////////////////////

	template<size_t CompSize, typename VecElemT>
	using vec = std::conditional_t<is_vec_elem<VecElemT>::value && (CompSize > 0), vec_base<CompSize, VecElemT>, void>;

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
	constexpr auto t1 = v1.dot(v2);
	constexpr auto t2 = v1.ccw(v2);
	constexpr auto t3 = min(v1, v2);
	constexpr auto t4 = max(v1, v2, v3);
	constexpr auto t5 = v3.cross(v4);
	constexpr auto t6 = v3 + v4;
	constexpr auto t7 = v3 * v4;
	constexpr auto t8 = v3 * 2.0;
	constexpr auto t9 = v1 - 2 * dot(v1, v2) * v2;
	constexpr auto t10 = v1.reflect(v2);
	constexpr auto t11 = reflect(v1, v2);
}
