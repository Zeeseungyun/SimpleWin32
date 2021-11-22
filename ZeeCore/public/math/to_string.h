#pragma once
#include "../core/string.h"
#include "vec/vec.h"

namespace zee {
namespace math {
namespace impl {
	template<size_t CompSize, typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::wstring>
		to_wstring(const vec_impl<CompSize, T, true>& v) noexcept {
		std::wstring ret = wstring_sprintf(L"vec_base%d(" , CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += wstring_sprintf(L"%f, ", v[i]);
		}
		ret += wstring_sprintf(L"%f)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::wstring>
		to_wstring(const vec_impl<CompSize, T, true>& v) noexcept {
		std::wstring ret = wstring_sprintf(L"vec_base%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += wstring_sprintf(L"%d, ", v[i]);
		}
		ret += wstring_sprintf(L"%d)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	std::enable_if_t<std::is_floating_point<T>::value, std::string>
		to_string(const vec_impl<CompSize, T, true>& v) noexcept {
		std::string ret = string_sprintf("vec_base%d(" , CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += string_sprintf("%f, ", v[i]);
		}
		ret += string_sprintf("%f)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, std::string>
		to_string(const vec_impl<CompSize, T, true>& v) noexcept {
		std::string ret = string_sprintf("vec_base%d(" , CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += string_sprintf("%d, ", v[i]);
		}
		ret += string_sprintf("%d)", v[CompSize - 1]);
		return ret;
	}

}//namespace zee::math::impl

	template<size_t CompSize, typename T>
	auto to_string(const vec_base<CompSize, T>& v) noexcept {
		return impl::to_string(v);
	}

	template<size_t CompSize, typename T>
	auto to_wstring(const vec_base<CompSize, T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<size_t CompSize, typename T>
	tstring to_tstring(const vec_base<CompSize, T>& v) noexcept {
#ifdef UNICODE
		return to_wstring(v);
#else 
		return to_string(v);
#endif
	}

}//namespace zee::math 
}//namespace zee
