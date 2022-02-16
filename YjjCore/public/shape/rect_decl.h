#pragma once
#include "../util/type_supports.h"
namespace yjj {
namespace shape {
namespace impl {
	template<typename ElemT, bool IsValidRect = is_vec_element<ElemT>::value>
	struct basic_rect_impl;
}//namespace yjj::shape::impl
	
	template<typename ElemT>
	struct basic_rect;
}//namespace yjj::shape
}//namespace yjj