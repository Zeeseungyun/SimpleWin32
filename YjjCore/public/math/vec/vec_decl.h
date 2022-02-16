#pragma once
namespace yjj {
namespace math {
namespace impl {
	struct legal_vec;
	struct illegal_vec;

	template<size_t CompSize, typename T, bool IsValidArgs> struct vec_impl;
}//namespace yjj::math::impl
	template<size_t CompSize, typename T> struct basic_vec;
}//namespace yjj::math  
}//namespace yjj 
