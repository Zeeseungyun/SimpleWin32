#pragma once
#include "vec_impl.h"
namespace zee {
namespace math {
		
	////////////////////////////////
	// 
	// Explicit instantiation.
	// 
	////////////////////////////////

	template struct vec_base<2, int32>;
	template struct vec_base<2, float>;
	template struct vec_base<2, double>;

	template struct vec_base<3, int32>;
	template struct vec_base<3, float>;
	template struct vec_base<3, double>;

	template struct vec_base<4, int32>;
	template struct vec_base<4, float>;
	template struct vec_base<4, double>;

	////////////////////////////////
	// 
	// Type redefinition.
	// 
	////////////////////////////////

	typedef vec_base<2, float>  vec2;
	typedef vec_base<2, int32>  vec2i;
	typedef vec_base<2, float>  vec2f;
	typedef vec_base<2, double> vec2df;

	typedef vec_base<3, float>  vec3;
	typedef vec_base<3, int32>  vec3i;
	typedef vec_base<3, float>  vec3f;
	typedef vec_base<3, double> vec3df;

	typedef vec_base<4, float>  vec4;
	typedef vec_base<4, int32>  vec4i;
	typedef vec_base<4, float>  vec4f;
	typedef vec_base<4, double> vec4df;

}//namespace zee::math
}//namespace zee
