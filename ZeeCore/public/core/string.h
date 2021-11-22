#pragma once
#include "core_base.h"
#include <string>
#include <cassert>
#include <cstdarg>

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

	std::string string_sprintf(const char* format, ...) noexcept;
	int string_sprintf(std::string& buffer_, const char* format, ...) noexcept;

	std::string string_vprintf(const char* format, va_list args) noexcept;
	int string_vprintf(std::string& buffer_, const char* format, va_list args) noexcept;

	std::wstring wstring_sprintf(const wchar_t* format, ...) noexcept;
	int wstring_sprintf(std::wstring& buffer_, const wchar_t* format, ...) noexcept;

	std::wstring wstring_vprintf(const wchar_t* format, va_list args) noexcept;
	int wstring_vprintf(std::wstring& buffer_, const wchar_t* format, va_list args) noexcept;

	tstring tstring_sprintf(const TCHAR* format, ...) noexcept;
	int tstring_sprintf(tstring& buffer_, const TCHAR* format, ...) noexcept;

	tstring tstring_vprintf(const TCHAR* format, va_list args) noexcept;
	int tstring_vprintf(tstring& buffer_, const TCHAR* format, va_list args) noexcept;

namespace impl {
	template<typename CharT>
	struct strmanip_base_impl {

	};
	/*
	* strmanip_base_impl마저 작성.
	* 
	*/
	template<>
	struct strmanip_base_impl<char> : std::char_traits<char> {
		typedef std::basic_string<char_type> string_type;

		static int_type vprintf(char_type* const buf, size_t buf_size, const char_type* fmt, va_list args) noexcept {
			if (!fmt || !args) {
				return -1;
			}
			return std::vsnprintf(buf, buf_size, fmt, args);
		}

		//*https://en.cppreference.com/w/cpp/chrono/c/strftime
		static string_type cur_time_to_str(const char_type* const format);
	};

	template<>
	struct strmanip_base_impl<wchar_t> : std::char_traits<wchar_t> {
		typedef std::basic_string<char_type> string_type;

		static int_type vprintf(char_type* const buf, size_t buf_size, const char_type* fmt, va_list args) noexcept {
			if (!fmt || !args) {
				return -1;
			}
			return std::vswprintf(buf, buf_size, fmt, args);
		}
	};


}//namespace impl
	template<typename CharT = TCHAR>
	struct strmanip_base : impl::strmanip_base_impl<CharT> {
	private:
		typedef impl::strmanip_base_impl<CharT> base_type;

	public:
		using typename base_type::int_type;
		using typename base_type::char_type;

		typedef std::basic_string<char_type> string_type;

		static int_type vprintf(string_type& buf, const char_type* fmt, va_list args) noexcept {
			if (!fmt || !args) {
				return -1;
			}

			int_type ret = vprintf(0, 0, fmt, args);
			if (ret < 0) {
				return ret;
			}

#define NULL_TERMINATED_STR_COMPLEMENT 1
			const size_t need_buf_size = (size_t)ret + NULL_TERMINATED_STR_COMPLEMENT;
#undef NULL_TERMINATED_STR_COMPLEMENT
			const size_t old_offset = buf.length();

			buf.resize(old_offset + need_buf_size);
			ret = base_type::vprintf(&buf[old_offset], old_offset + need_buf_size, fmt, args);
			assert(buf.size() == ret);
			return ret;
		}

		static int_type sprintf(char_type* const buf, size_t buf_size, const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			const int_type ret = base_type::vprintf(buf, buf_size, fmt, args);
			va_end(args);
			return ret;
		}

		static string_type vprintf(const char_type* fmt, va_list args) noexcept {
			string_type ret;
			base_type::vprintf(ret, fmt, args);
			return ret;
		}

		static int_type sprintf(string_type& buf, const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			const int_type ret = base_type::vprintf(buf, fmt, args);
			va_end(args);
			return ret;
		}

		static string_type sprintf(const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			string_type buffer = base_type::vprintf(fmt, args);
			va_end(args);
			return buffer;
		}
	};

	tstring current_time_to_string(const char* const format = "%y-%m-%d %H:%M:%S") noexcept;
	tstring current_time_to_wstring(const wchar_t* const format = L"%y-%m-%d %H:%M:%S") noexcept;
	tstring current_time_to_tstring(const TCHAR* const format = TEXT("%y-%m-%d %H:%M:%S")) noexcept;

	int tvprintf(TCHAR* buffer, size_t buf_size, const TCHAR* format, va_list args);
	int tsprintf(TCHAR* buffer, size_t buf_size, const TCHAR* format, ...);
	
	int tsscanf(const TCHAR* const buffer, const TCHAR* format, ...) noexcept;
	int tvscanf(const TCHAR* const buffer, const TCHAR* format, va_list args) noexcept;

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

	inline tstring to_tstring(bool b) noexcept {
		return b ? TEXT("true") : TEXT("false");
	}

	tstring to_tstring(const char* c_str) ;
	tstring to_tstring(const wchar_t* w_str) ;
}

