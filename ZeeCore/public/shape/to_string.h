#pragma once
#include "../core/string.h"
#include "rect.h"
#include "circle.h"
#include "boundings.h"

namespace zee {
namespace shape {
namespace impl {

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const circle_base_impl<T>& v) noexcept {
		return string_sprintf(
			"circle(origin(%f, %f), radius(%f))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base_impl<T>& v) noexcept {
		return wstring_sprintf(
			"circle(origin(%f, %f), radius(%f))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const circle_base_impl<T>& v) noexcept {
		return string_sprintf(
			"circle(origin(%d, %d), radius(%d))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const circle_base_impl<T>& v) noexcept {
		return wstring_sprintf(
			L"circle(origin(%d, %d), radius(%d))",
			v.origin.x,
			v.origin.y,
			v.radius
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return string_sprintf(
			"rect(l(%f), t(%f), r(%f), b(%f))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base_impl<T, true>& v) noexcept {
		return wstring_sprintf(
			L"rect(l(%f), t(%f), r(%f), b(%f))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}
	
	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const rect_base_impl<T, true>& v) noexcept {
		return string_sprintf(
			"rect(l(%d), t(%d), r(%d), b(%d))",
			v.get_left(), v.get_top(), v.get_right(), v.get_bottom()
		);
	}

	template<typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const rect_base_impl<T, true>& v) noexcept {
		return wstring_sprintf(
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

	constexpr const char* to_string(collide_type v) noexcept {
		switch (v)
		{
		case zee::shape::collide_type::intersect: return "intersect"; break;
		case zee::shape::collide_type::contain: return "contain"; break;
		case zee::shape::collide_type::none: 
		default:
			break;
		}

		return "none"; 
	}

	constexpr const wchar_t* to_wstring(collide_type v) noexcept {
		switch (v)
		{
		case zee::shape::collide_type::intersect: return L"intersect"; break;
		case zee::shape::collide_type::contain: return L"contain"; break;
		case zee::shape::collide_type::none:
		default:
			break;
		}

		return L"none";
	}


	constexpr const TCHAR* to_tstring(collide_type v) noexcept {
#ifdef UNICODE
		return to_wstring(v);
#else 
		return to_string(v);
#endif
	}

}//namespace zee::shape 
}//namespace zee
