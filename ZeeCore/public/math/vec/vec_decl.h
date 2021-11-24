#pragma once
namespace zee {
namespace math {
namespace impl {
	struct legal_vec;
	struct illegal_vec;

	template<size_t CompSize, typename T, bool IsValidArgs> struct vec_impl;
}//namespace zee::math::impl
	template<size_t CompSize, typename T> struct vec_base;
}//namespace zee::math  
}//namespace zee 
