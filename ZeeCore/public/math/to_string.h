#pragma once
#include "../core/string.h"
#include "vec/vec.h"

namespace zee {
namespace math {
namespace impl {

	template<size_t CompSize, typename T>
	auto to_wstring(const vec_impl<CompSize, T, true>& v) noexcept
		-> decltype(std::enable_if_t<std::is_floating_point<T>::value>(),
			std::basic_string<char>()) {
		typedef char char_type;
		std::basic_string<char_type> ret;
		strmanip_base<char_type>::sprintf(ret, L"vec_base%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			strmanip::sprintf(ret, L"%f, ", v[i]);
		}
		strmanip::sprintf(ret, L"%f)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	auto to_wstring(const vec_impl<CompSize, T, true>& v) noexcept
		-> decltype(std::enable_if_t<!std::is_floating_point<T>::value>(),
			std::basic_string<char>()) {
		typedef char char_type;
		std::basic_string<char_type> ret;
		strmanip_base<char_type>::sprintf(ret, L"vec_base%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			strmanip::sprintf(ret, L"%d, ", v[i]);
		}
		strmanip::sprintf(ret, L"%d)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	auto to_string(const vec_impl<CompSize, T, true>& v) noexcept
		-> decltype(std::enable_if_t<std::is_floating_point<T>::value>(),
			std::basic_string<char>()) {
		typedef char char_type;
		std::basic_string<char_type> ret;
		strmanip_base<char_type>::sprintf(ret, "vec_base%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			strmanip::sprintf(ret, "%f, ", v[i]);
		}
		strmanip::sprintf(ret, "%f)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	auto to_string(const vec_impl<CompSize, T, true>& v) noexcept 
		-> decltype( std::enable_if_t<!std::is_floating_point<T>::value>(), 
			std::basic_string<char>()) {
		typedef char char_type;
		std::basic_string<char_type> ret;
		strmanip_base<char_type>::sprintf(ret, "vec_base%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			strmanip::sprintf(ret, "%d, ", v[i]);
		}
		strmanip::sprintf(ret, "%d)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	tstring to_tstring(const vec_impl<CompSize, T, true>& v) noexcept {
#ifdef UNICODE
		return to_wstring(v);
#else 
		return to_string(v);
#endif
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
		return impl::to_tstring(v);
	}

}//namespace zee::math 
}//namespace zee
