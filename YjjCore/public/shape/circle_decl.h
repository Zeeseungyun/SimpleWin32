#pragma once
#include "../util/type_supports.h"
namespace yjj {
namespace shape {
namespace impl {
	template<typename ElemT, bool IsValidElemType = is_vec_element<ElemT>::value>
	struct basic_circle_impl;
}//namespace yjj::shape::impl
	template<typename ElemT>
	struct basic_circle;
}//namespace yjj::shape
}//namespace yjj