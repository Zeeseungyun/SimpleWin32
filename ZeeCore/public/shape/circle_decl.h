#pragma once
#include "../util/type_supports.h"
namespace zee {
namespace shape {
namespace impl {
	template<typename ElemT, bool IsValidElemType = is_vec_element<ElemT>::value>
	struct circle_base_impl;
}//namespace zee::shape::impl
}//namespace zee::shape
}//namespace zee