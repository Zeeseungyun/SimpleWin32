#pragma once
#include <string>
#include <tchar.h> //msvc extention header. windows only.

namespace zee {
	typedef std::basic_string<TCHAR> tstring;

	std::string string_format(const char* format, ...) noexcept;
	void string_format(std::string& buffer, const char* format, ...) noexcept;

	std::wstring wstring_format(const wchar_t* format, ...) noexcept;
	void wstring_format(std::wstring& buffer, const wchar_t* format, ...) noexcept;

	tstring tstring_format(const TCHAR* format, ...) noexcept;
	void tstring_format(tstring& buffer, const TCHAR* format, ...) noexcept;

	tstring current_time_to_tstring() noexcept;

	using std::to_string;
	using std::to_wstring;

	template<typename T>
	tstring to_tstring(const T& v) noexcept {
#ifdef UNICODE
		return to_wstring(v);
#else
		return to_string(v);
#endif 
	}
}

#define ZEE_STR_CAT_L_IMPL(v) L ## v
#define ZEE_STR_CAT_L(v) ZEE_STR_CAT_L_IMPL(v)
