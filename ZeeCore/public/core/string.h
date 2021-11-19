#pragma once
#include <string>
#include <tchar.h> //msvc extention header. windows only.

namespace zee {
	typedef std::basic_string<TCHAR> tstring;

	tstring current_time_to_string();
	tstring tstring_format(const TCHAR* format, ...);
	void tstring_format(tstring& buffer, const TCHAR* format, ...);

	template<typename T>
	std::enable_if_t<std::is_arithmetic<T>::value, tstring>
		to_string(const T& v) noexcept {
#ifdef UNICODE
		return std::to_wstring(v);
#else
		return std::to_string(v);
#endif 
	}
}
