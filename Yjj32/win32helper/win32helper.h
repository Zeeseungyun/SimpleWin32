#pragma once
#include <core/string.h>
#include <shape/shape.h>
#include "windows_with_macro.h"

namespace yjj {
namespace win32helper {
	tstring error_to_tstring(DWORD error_code) noexcept;
	tstring last_error_to_tstring() noexcept;

}//namespace yjj::win32helper
	typedef decltype(RECT().top) win_shape_element_type;

	template<typename ElemT>
	constexpr RECT to_win32(const shape::basic_rect<ElemT>& rc) noexcept {
		return {
			rc.get_left<win_shape_element_type>()  ,
			rc.get_top<win_shape_element_type>()   ,
			rc.get_right<win_shape_element_type>() ,
			rc.get_bottom<win_shape_element_type>()
		};
	}

	constexpr shape::recti to_yjj(const RECT& rc) noexcept {
		return {
			rc.left   ,
			rc.top    ,
			rc.right  ,
			rc.bottom
		};
	}

	template<typename ElemT>
	constexpr POINT to_win32(const math::basic_vec<2, ElemT>& pt) noexcept {
		return { (win_shape_element_type)pt.data[0], (win_shape_element_type)pt.data[1] };
	}

	constexpr math::vec2i to_yjj(const POINT& pt) noexcept {
		return { pt.x, pt.y };
	}

}//namespace yjj
