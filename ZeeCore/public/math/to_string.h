#pragma once
#include "../core/string.h"
#include "vec/vec.h"

namespace zee {
namespace math {
	template<typename T>
	std::enable_if_t<std::is_arithmetic<T>::value, tstring>
		to_string(const T& v) noexcept {
#ifdef UNICODE
		return std::to_wstring(v);
#else 
		return std::to_string(v);
#endif
	}

	template<size_t CompSize, typename T>
	std::enable_if_t<std::is_floating_point<T>::value, tstring>
		to_string(const vec<CompSize, T>& v) noexcept {
		tstring ret = tstring_format(_T("vec%d(")
			, CompSize
		);

		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += tstring_format(_T("%f, "), v[i]);
		}

		ret += to_string(v[CompSize - 1]);
		ret += _T(")");
		return ret;
	}

	template<size_t CompSize, typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, tstring>
		to_string(const vec<CompSize, T>& v) noexcept {
		tstring ret = tstring_format(_T("vec%d(")
			, CompSize
		);

		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += tstring_format(_T("%d, "), v[i]);
		}

		ret += to_string(v[CompSize - 1]);
		ret += _T(")");
		return ret;
	}

namespace impl {
	template<size_t CompSize, typename T>
	std::enable_if_t<std::is_floating_point<T>::value, tstring>
		to_string(const impl::vec_impl<CompSize, T, true>& v) noexcept {
		tstring ret = tstring_format(_T("vec%d(")
			, CompSize
		);

		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += tstring_format(_T("%f, "), v[i]);
		}

		ret += math::to_string(v[CompSize - 1]);
		ret += _T(")");
		return ret;
	}

	template<size_t CompSize, typename T>
	std::enable_if_t<!std::is_floating_point<T>::value, tstring>
		to_string(const impl::vec_impl<CompSize, T, true>& v) noexcept {
		tstring ret = tstring_format(_T("vec%d(")
			, CompSize
		);

		for (size_t i = 0; i != CompSize - 1; ++i) {
			ret += tstring_format(_T("%d, "), v[i]);
		}

		ret += math::to_string(v[CompSize - 1]);
		ret += _T(")");
		return ret;
	}

}//namespace zee::math::impl
}//namespace zee::math 
}//namespace zee
