#pragma once
#include "../core/string.h"
#include "rect.h"
#include "circle.h"

namespace zee {
namespace shape {
namespace impl {

#define ZEE_CIRCLE_STR_FORMAT(fmt) "circle(origin("#fmt", "#fmt"), radius("#fmt"))"

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const circle_base_impl<T>& v) noexcept {
		return string_format(
			ZEE_CIRCLE_STR_FORMAT(%f),
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base_impl<T>& v) noexcept {
		return wstring_format(
			ZEE_STR_CAT_L(ZEE_CIRCLE_STR_FORMAT(%f)),
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const circle_base_impl<T>& v) noexcept {
		return string_format(
			ZEE_CIRCLE_STR_FORMAT(%d),
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base_impl<T>& v) noexcept {
		return wstring_format(
			ZEE_STR_CAT_L(ZEE_CIRCLE_STR_FORMAT(%d)),
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}
#undef ZEE_CIRCLE_STR_FORMAT
#define ZEE_RECT_STR_FORMAT(fmt) "rect(l("#fmt"), t("#fmt"), r("#fmt"), b("#fmt"))"

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			ZEE_RECT_STR_FORMAT(%f),
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			ZEE_STR_CAT_L(ZEE_RECT_STR_FORMAT(%f)),
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}
	
	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			ZEE_RECT_STR_FORMAT(%d),
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			ZEE_STR_CAT_L(ZEE_RECT_STR_FORMAT(%d)),
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

}//namespace zee::shape::impl

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const rect_base<T>& v) noexcept {
		return impl::to_string(v);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const rect_base<T>& v) noexcept {
		return impl::to_string(v);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base<T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base<T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const circle_base<T>& v) noexcept {
		return impl::to_string(v);
	}

	template<typename T>
	std::enable_if_t<is_vec_element<T>::value && !std::is_floating_point<T>::value, std::string>
		to_string(const circle_base<T>& v) noexcept {
		return impl::to_string(v);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base<T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<typename T>
	std::enable_if_t<is_vec_element<T>::value && !std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base<T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<typename T>
	tstring to_tstring(const circle_base<T>& v) noexcept {
#ifdef UNICODE
		return to_wstring(v);
#else 
		return to_string(v);
#endif
	}

	template<typename T>
	tstring to_tstring(const rect_base<T>& v) noexcept {
#ifdef UNICODE
		return to_wstring(v);
#else 
		return to_string(v);
#endif
	}

}//namespace zee::shape 
}//namespace zee
