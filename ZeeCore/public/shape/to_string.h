#pragma once
#include "../core/string.h"
#include "rect.h"
#include "circle.h"

namespace zee {
namespace shape {
namespace impl {

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const circle_base_impl<T>& v) noexcept {
		return string_format(
			"circle(origin(%f, %f), radius(%f))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base_impl<T>& v) noexcept {
		return wstring_format(
			"circle(origin(%f, %f), radius(%f))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const circle_base_impl<T>& v) noexcept {
		return string_format(
			"circle(origin(%d, %d), radius(%d))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base_impl<T>& v) noexcept {
		return wstring_format(
			L"circle(origin(%d, %d), radius(%d))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			"rect(l(%f), t(%f), r(%f), b(%f))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			L"rect(l(%f), t(%f), r(%f), b(%f))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}
	
	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			"rect(l(%d), t(%d), r(%d), b(%d))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base_impl<T, true>& v) noexcept {
		return string_format(
			L"rect(l(%d), t(%d), r(%d), b(%d))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

}//namespace zee::shape::impl

	template<typename T>
	auto to_string(const rect_base<T>& v) noexcept {
		return impl::to_string(v);
	}

	template<typename T> auto
		to_wstring(const rect_base<T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<typename T>
	auto to_string(const circle_base<T>& v) noexcept {
		return impl::to_string(v);
	}

	template<typename T>
	auto to_wstring(const circle_base<T>& v) noexcept {
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
