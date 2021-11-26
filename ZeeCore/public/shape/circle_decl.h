#pragma once
#include "../util/type_supports.h"
namespace zee {
namespace shape {
namespace impl {
	template<typename ElemT, bool IsValidElemType = is_vec_element<ElemT>::value>
	struct basic_circle_impl;
}//namespace zee::shape::impl
	template<typename ElemT>
	struct basic_circle;
}//namespace zee::shape
}//namespace zee