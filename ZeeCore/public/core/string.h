#pragma once
#include <string>

#ifndef TEXT
	#ifdef UNICODE
		typedef wchar_t TCHAR;
		#define TEXT(x) L ## x
	#else
		typedef char TCHAR;
		#define TEXT(x) x
	#endif //UNICODE
#endif //TEXT

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

