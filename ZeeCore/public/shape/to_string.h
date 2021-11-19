#pragma once
#include "../core/string.h"
#include "rect.h"
#include "circle.h"

namespace zee {
namespace shape {
	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, tstring>
		to_string(const rect_base<T>& v) noexcept {
		return tstring_format(_T("rect(l(%f), t(%f), r(%f), b(%f))")
			, v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, tstring>
		to_string(const rect_base<T>& v) noexcept {
		return tstring_format(_T("rect(l(%d), t(%d), r(%d), b(%d))")
			, v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, tstring>
		to_string(const circle_base<T>& v) noexcept {
		return tstring_format(_T("circle(origin(%f, %f), radius(%f))"),
			v.origin.x, 
			v.origin.y,
			v.radius			
		);
	}

	template<typename T>
	std::enable_if_t<is_vec_element<T>::value && !std::is_floating_point<T>::value, tstring>
		to_string(const circle_base<T>& v) noexcept {
		return tstring_format(_T("circle(origin(%d, %d), radius(%d))"),
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}
namespace impl {

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, tstring>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return tstring_format(_T("rect(l(%f), t(%f), r(%f), b(%f))")
			, v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, tstring>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return tstring_format(_T("rect(l(%d), t(%d), r(%d), b(%d))")
			, v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}
}//namespace zee::shape::impl
}//namespace zee::shape 
}//namespace zee
