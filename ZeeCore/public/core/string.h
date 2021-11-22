#pragma once
#include "core_base.h"
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

	struct format_helper {

		static int calculate_buffer_size(const wchar_t* format, ...) noexcept;
		static int calculate_buffer_size(const char* format, ...) noexcept;
		static int calculate_buffer_size(const wchar_t* format, va_list args) noexcept;
		static int calculate_buffer_size(const char* format, va_list args) noexcept;
	};

	std::string string_format(const char* format, ...) noexcept;
	int string_format(std::string& buffer_, const char* format, ...) noexcept;

	std::string string_vformat(const char* format, va_list args) noexcept;
	int string_vformat(std::string& buffer_, const char* format, va_list args) noexcept;

	std::wstring wstring_format(const wchar_t* format, ...) noexcept;
	int wstring_format(std::wstring& buffer_, const wchar_t* format, ...) noexcept;

	std::wstring wstring_vformat(const wchar_t* format, va_list args) noexcept;
	int wstring_vformat(std::wstring& buffer_, const wchar_t* format, va_list args) noexcept;

	tstring tstring_format(const TCHAR* format, ...) noexcept;
	int tstring_format(tstring& buffer_, const TCHAR* format, ...) noexcept;

	tstring tstring_vformat(const TCHAR* format, va_list args) noexcept;
	int tstring_vformat(tstring& buffer_, const TCHAR* format, va_list args) noexcept;

	tstring current_time_to_tstring() noexcept;

	int tvprintf(TCHAR* buffer, size_t buf_size, const TCHAR* format, va_list args);
	int tsprintf(TCHAR* buffer, size_t buf_size, const TCHAR* format, ...);

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

