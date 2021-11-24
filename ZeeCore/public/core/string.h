#pragma once
#include "core_base.h"
#include <string>
#include <cstdarg>
#include <chrono>
#include <cwchar>
#include <cstdio>

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
namespace impl {
	template<typename CharT /*= TCHAR */, typename TraitT /*= std::char_traits<CharT>*/>
	struct strmanip_base_impl {	};

	template<typename TraitT>
	struct strmanip_base_impl<char, TraitT> : TraitT {
		using typename TraitT::char_type;
		using typename TraitT::int_type;

		//https://en.cppreference.com/w/c/io/vfprintf
		static int_type vprintf(char_type* const buf, size_t buf_size, const char_type* fmt, va_list args) noexcept {
			if (!fmt || !args) {
				return -1;
			}
			return std::vsnprintf(buf, buf_size, fmt, args);
		}

		static const char_type* get_default_time_format() noexcept { return L"%y-%m-%d %H:%M:%S"; }

		//*https://en.cppreference.com/w/cpp/chrono/c/strftime
		static size_t time_to_str( char_type* buf, size_t buf_size, const time_t& time_v, const char_type* fmt) noexcept {
			if (!fmt || !buf) {
				assert(0 && " buf, fmt must be not nullptr.");
				return 0;
			}

			tm tm_v;
			if (localtime_s(&tm_v, &time_v) != 0) {
				return 0;
			}
			return std::strftime(buf, buf_size, fmt, &tm_v);
		}

		//https://en.cppreference.com/w/c/io/vfscanf
		static int_type vsscanf(const char_type* buf, const char_type* fmt, va_list args) noexcept {
			return ::vsscanf_s(buf, fmt, args);
		}
		
	};

	template<typename TraitT>
	struct strmanip_base_impl<wchar_t, TraitT> : TraitT {
		using typename TraitT::int_type;
		using typename TraitT::char_type;

		//https://en.cppreference.com/w/c/io/vfwprintf
		static int_type vprintf(char_type* const buf, size_t buf_size, const char_type* fmt, va_list args) noexcept {
			if (!fmt) {
				assert(0 && " fmt must be not nullptr.");
				return -1;
			}
			return std::vswprintf(buf, buf_size, fmt, args);
		}

		static const char_type* get_default_time_format() noexcept { return L"%y-%m-%d %H:%M:%S"; }

		//https://en.cppreference.com/w/cpp/chrono/c/wcsftime
		static size_t time_to_str(char_type* buf, size_t buf_size, const time_t& time_v, const char_type* fmt) noexcept {
			if (!fmt || !buf) {
				assert(0 && " buf, fmt must be not nullptr.");
				return 0;
			}

			tm tm_v;
			if (localtime_s(&tm_v, &time_v) != 0) {
				return 0;
			}

			return std::wcsftime(buf, buf_size, fmt, &tm_v);
		}

		//https://en.cppreference.com/w/c/io/vfwscanf
		static int_type vsscanf(const char_type* buf, const char_type* fmt, va_list args) noexcept {
			return ::vswscanf_s(buf, fmt, args);
		}
	};

}//namespace impl

	template<typename CharT = TCHAR, typename TraitT = std::char_traits<CharT>>
	struct strmanip_base : impl::strmanip_base_impl<CharT, TraitT> {
	private:
		typedef impl::strmanip_base_impl<CharT, TraitT> base_type;

	public:
		using typename base_type::int_type;
		using typename base_type::char_type;

		template<typename TraitU, typename AllocU>
		static int_type vprintf(std::basic_string<char_type, TraitU, AllocU>& buf, const char_type* fmt, va_list args) noexcept {
			if (!fmt || !args) {
				assert(0 && "invalid arguments.");
				return -1;
			}

			int_type ret = base_type::vprintf(0, 0, fmt, args);
			if (ret < 0) {
				assert(0 && "something wrong.");
				return ret;
			}

			const size_t add_buf_size = (size_t)ret + 1;
			const size_t old_length = buf.length();

			//include null char
			buf.resize(old_length + add_buf_size);
			ret = base_type::vprintf(&buf[0] + old_length, old_length + add_buf_size, fmt, args);
			assert((size_t)ret == add_buf_size - 1 && "something wrong.");
			//remove null char
			buf.resize(old_length + add_buf_size - 1); 
			return (int_type)buf.length();
		}

		static int_type sprintf(char_type* const buf, size_t buf_size, const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			const int_type ret = base_type::vprintf(buf, buf_size, fmt, args);
			va_end(args);
			return ret;
		}

		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static std::basic_string<char_type, TraitU, AllocU> vprintf(const char_type* fmt, va_list args) noexcept {
			std::basic_string<char_type, TraitU, AllocU> buf;
			vprintf(buf, fmt, args);
			return buf;
		}

		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static int_type sprintf(std::basic_string<char_type, TraitU, AllocU>& buf, const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			const int_type ret = vprintf(buf, fmt, args);
			va_end(args);
			return ret;
		}

		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static std::basic_string<char_type, TraitU, AllocU> sprintf(const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			std::basic_string<char_type, TraitU, AllocU> buf;
			vprintf(buf, fmt, args);
			va_end(args);
			return buf;
		}

		//*https://en.cppreference.com/w/cpp/chrono/c/strftime
		//성공하면 0 아닌 값, 실패하면 0.
		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static size_t
			time_to_str(std::basic_string<char_type, TraitU, AllocU>& buf, const time_t& time_v, const char_type* fmt) noexcept {
			const size_t old_length = buf.length();
			size_t add_buf_size = 32;
			buf.resize(old_length + add_buf_size);
			size_t written_size = 0;
			while ((written_size = base_type::time_to_str(&buf[0] + old_length, buf.size(), time_v, fmt)) == 0) {
				add_buf_size *= 2;
				buf.resize(old_length + add_buf_size);
			}
			buf.resize(old_length + written_size);
			return buf.length();
		}

		//성공하면 0 아닌 값, 실패하면 0.
		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static std::basic_string<char_type, TraitU, AllocU>
			time_to_str(const time_t& time_v, const char_type* fmt) noexcept {
			std::basic_string<char_type, TraitU, AllocU> buf;
			time_to_str(buf, time_v, fmt);
			return buf;
		}

		//*https://en.cppreference.com/w/cpp/chrono/c/strftime
		//성공하면 0 아닌 값, 실패하면 0.
		static size_t 
			cur_time_to_str(char_type* buf, size_t buf_size, const char_type* fmt = base_type::get_default_time_format()) noexcept {
			return base_type::time_to_str(buf, buf_size, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), fmt);
		}

		//*https://en.cppreference.com/w/cpp/chrono/c/strftime
		//성공하면 0 아닌 값, 실패하면 0.
		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static size_t
			cur_time_to_str(std::basic_string<char_type, TraitU, AllocU>& buf, const char_type* fmt = base_type::get_default_time_format()) noexcept {
			return time_to_str(buf, std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), fmt);
		}

		//성공하면 0 아닌 값, 실패하면 0.
		template<typename TraitU = TraitT, typename AllocU = std::allocator<char_type>>
		static std::basic_string<char_type, TraitU, AllocU>
			cur_time_to_str(const char_type* fmt = base_type::get_default_time_format()) noexcept {
			std::basic_string<char_type, TraitU, AllocU> buf;
			cur_time_to_str(buf, fmt);
			return buf;
		}

		//성공 시, 적용된 인자의 개수
		//실패 시, EOF
		static int_type sscanf(const char_type* buf, const char_type* fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			const int_type ret = base_type::vsscanf(buf, fmt, args);
			va_end(args);
			return ret;
		}
	};

	typedef std::basic_string<TCHAR> tstring;

	//template<typename CharT = TCHAR, typename TraitT = std::char_traits<CharT>>
	template struct strmanip_base<char>;
	template struct strmanip_base<wchar_t>;
	
	using strmanip = strmanip_base<char>;
	using wstrmanip = strmanip_base<wchar_t>;
	using tstrmanip = strmanip_base<TCHAR>;

	using std::to_string;
	using std::to_wstring;

namespace impl {
	template<typename TraitsT = std::char_traits<char>, typename AllocT = std::allocator<char>>
	std::basic_string<char, TraitsT, AllocT> 
		wstr_to_str_impl(const wchar_t* const c_str, const size_t len) {
		size_t buf_size = 0;
		errno_t err = ::wcstombs_s(&buf_size, 0, 0, c_str, len);
		if (err != 0) {
			return {};
		}

		std::basic_string<char, TraitsT, AllocT> buf;
		buf.resize(buf_size);//include null character.
		err = ::wcstombs_s(&buf_size, &buf[0], buf.size(), c_str, len);
		if (err != 0) {
			return {};
		}

		buf.resize(buf_size - 1);//remove null character.
		return buf;
	}

	template<typename TraitsT = std::char_traits<wchar_t>, typename AllocT = std::allocator<wchar_t>>
	std::basic_string<wchar_t, TraitsT, AllocT>
		str_to_wstr_impl(const char* const c_str, const size_t len) {
		size_t buf_size = 0;
		errno_t err = ::mbstowcs_s(&buf_size, 0, 0, c_str, len);
		if (err != 0) {
			return {};
		}

		std::basic_string<wchar_t, TraitsT, AllocT> buf;
		buf.resize(buf_size);//include null character.
		err = ::mbstowcs_s(&buf_size, &buf[0], buf.size(), c_str, len);
		if (err != 0) {
			return {};
		}

		buf.resize(buf_size - 1);//remove null character.
		return buf;
	}

}//namespace zee::impl

	template<typename TraitsT = std::char_traits<wchar_t>, typename AllocT = std::allocator<wchar_t>>
	std::basic_string<wchar_t, TraitsT, AllocT>
		to_wstring(const char* str) {
		return impl::str_to_wstr_impl<TraitsT, AllocT>(str, std::char_traits<char>::length(str));
	}

	template<typename TraitsT = std::char_traits<char>, typename AllocT = std::allocator<char>>
	std::basic_string<char, TraitsT, AllocT>
		to_string(const wchar_t* str) {
		return impl::wstr_to_str_impl<TraitsT, AllocT>(str, std::char_traits<wchar_t>::length(str));
	}

	template<typename TraitsU, typename AllocU, typename TraitsT = std::char_traits<TCHAR>, typename AllocT = std::allocator<TCHAR>>
#ifdef UNICODE
		std::basic_string<TCHAR, TraitsT, AllocT>
#else
		const std::basic_string<TCHAR, TraitsT, AllocT>&
#endif
		to_tstring(const std::basic_string<char, TraitsU, AllocU>& str) {
#ifdef UNICODE
		return impl::str_to_wstr_impl<TraitsT, AllocT>(str.c_str(), str.size());
#else
		return str;
#endif
	}

		template<typename TraitsU, typename AllocU, typename TraitsT = std::char_traits<TCHAR>, typename AllocT = std::allocator<TCHAR>>
#ifdef UNICODE
		const std::basic_string<TCHAR, TraitsT, AllocT>&
#else
		std::basic_string<TCHAR, TraitsT, AllocT>
#endif
		to_tstring(const std::basic_string<wchar_t, TraitsU, AllocU>& str) {
#ifdef UNICODE
		return str;
#else
		return impl::wstr_to_str_impl<TraitsT, AllocT>(str.c_str(), str.size());
#endif
	}

	template<typename TraitsT = std::char_traits<TCHAR>, typename AllocT = std::allocator<TCHAR>>
	std::basic_string<TCHAR, TraitsT, AllocT> to_tstring(const char* c_str) {
#ifdef UNICODE
		return impl::str_to_wstr_impl<TraitsT, AllocT>(c_str, strmanip::length(c_str));
#else
		return c_str;
#endif
	}

	template<typename TraitsT = std::char_traits<TCHAR>, typename AllocT = std::allocator<TCHAR>>
	std::basic_string<TCHAR, TraitsT, AllocT> to_tstring(const wchar_t* c_str) {
#ifdef UNICODE
		return c_str;
#else
		return impl::wstr_to_str_impl<TraitsT, AllocT>(c_str, wstrmanip::length(c_str));
#endif
	}

	template<typename T>
	tstring to_tstring(const T& v) noexcept {
#ifdef UNICODE
		return std::to_wstring(v);
#else
		return std::to_string(v);
#endif 
	}

	inline tstring to_tstring(bool b) noexcept {
		return b ? TEXT("true") : TEXT("false");
	}

}

