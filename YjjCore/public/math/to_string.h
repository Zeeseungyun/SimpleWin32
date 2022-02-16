#pragma once
#include "../core/string.h"
#include "vec/vec.h"

namespace yjj {
namespace math {
namespace impl {

	template<size_t CompSize, typename T>
	auto to_wstring(const vec_impl<CompSize, T, true>& v) noexcept
		-> decltype(std::enable_if_t<std::is_floating_point<T>::value>(),
			std::basic_string<wchar_t>()) {
		typedef wchar_t char_type;
		std::basic_string<char_type> ret;
		basic_strmanip<char_type>::sprintf(ret, L"vec%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			basic_strmanip<char_type>::sprintf(ret, L"%f, ", v[i]);
		}
		basic_strmanip<char_type>::sprintf(ret, L"%f)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	auto to_wstring(const vec_impl<CompSize, T, true>& v) noexcept
		-> decltype(std::enable_if_t<!std::is_floating_point<T>::value>(),
			std::basic_string<wchar_t>()) {
		typedef wchar_t char_type;
		std::basic_string<char_type> ret;
		basic_strmanip<char_type>::sprintf(ret, L"vec%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			basic_strmanip<char_type>::sprintf(ret, L"%d, ", v[i]);
		}
		basic_strmanip<char_type>::sprintf(ret, L"%d)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	auto to_string(const vec_impl<CompSize, T, true>& v) noexcept
		-> decltype(std::enable_if_t<std::is_floating_point<T>::value>(),
			std::basic_string<char>()) {
		typedef char char_type;
		std::basic_string<char_type> ret;
		basic_strmanip<char_type>::sprintf(ret, "vec%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			basic_strmanip<char_type>::sprintf(ret, "%f, ", v[i]);
		}
		basic_strmanip<char_type>::sprintf(ret, "%f)", v[CompSize - 1]);
		return ret;
	}

	template<size_t CompSize, typename T>
	auto to_string(const vec_impl<CompSize, T, true>& v) noexcept 
		-> decltype( std::enable_if_t<!std::is_floating_point<T>::value>(), 
			std::basic_string<char>()) {
		typedef char char_type;
		std::basic_string<char_type> ret;
		basic_strmanip<char_type>::sprintf(ret, "vec%d(", CompSize);
		for (size_t i = 0; i != CompSize - 1; ++i) {
			basic_strmanip<char_type>::sprintf(ret, "%d, ", v[i]);
		}
		basic_strmanip<char_type>::sprintf(ret, "%d)", v[CompSize - 1]);
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

}//namespace yjj::math::impl

	template<size_t CompSize, typename T>
	auto to_string(const basic_vec<CompSize, T>& v) noexcept {
		return impl::to_string(v);
	}

	template<size_t CompSize, typename T>
	auto to_wstring(const basic_vec<CompSize, T>& v) noexcept {
		return impl::to_wstring(v);
	}

	template<size_t CompSize, typename T>
	tstring to_tstring(const basic_vec<CompSize, T>& v) noexcept {
		return impl::to_tstring(v);
	}

}//namespace yjj::math 
}//namespace yjj
