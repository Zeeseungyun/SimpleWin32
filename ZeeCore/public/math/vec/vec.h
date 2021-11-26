#pragma once
#include "vec_impl.h"
namespace zee {
namespace math {
		
	////////////////////////////////
	// 
	// Explicit instantiation.
	// 
	////////////////////////////////

	template struct basic_vec<2, int32>;
	template struct basic_vec<2, float>;
	template struct basic_vec<2, double>;

	template struct basic_vec<3, int32>;
	template struct basic_vec<3, float>;
	template struct basic_vec<3, double>;

	template struct basic_vec<4, int32>;
	template struct basic_vec<4, float>;
	template struct basic_vec<4, double>;

	////////////////////////////////
	// 
	// Type redefinition.
	// 
	////////////////////////////////

	typedef basic_vec<2, float>  vec2;
	typedef basic_vec<2, int32>  vec2i;
	typedef basic_vec<2, float>  vec2f;
	typedef basic_vec<2, double> vec2df;

	typedef basic_vec<3, float>  vec3;
	typedef basic_vec<3, int32>  vec3i;
	typedef basic_vec<3, float>  vec3f;
	typedef basic_vec<3, double> vec3df;

	typedef basic_vec<4, float>  vec4;
	typedef basic_vec<4, int32>  vec4i;
	typedef basic_vec<4, float>  vec4f;
	typedef basic_vec<4, double> vec4df;

}//namespace zee::math
}//namespace zee
