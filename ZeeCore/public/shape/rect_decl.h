#pragma once
#include "../util/type_supports.h"
namespace zee {
namespace shape {
namespace impl {
	template<typename ElemT, bool IsValidRect = is_vec_element<ElemT>::value>
	struct rect_base_impl;
}//namespace zee::shape::impl
}//namespace zee::shape
}//namespace zee