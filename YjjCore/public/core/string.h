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

namespace yjj {
namespace impl {
	template<typename CharT /*= TCHAR */, typename TraitT /*= std::char_traits<CharT>*/>
	struct basic_strmanip_impl {	};

	template<typename TraitT>
	struct basic_strmanip_impl<char, TraitT> : TraitT {
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
	struct basic_strmanip_impl<wchar_t, TraitT> : TraitT {
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
	struct basic_strmanip : impl::basic_strmanip_impl<CharT, TraitT> {
	private:
		typedef impl::basic_strmanip_impl<CharT, TraitT> base_type;

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
	template struct basic_strmanip<char>;
	template struct basic_strmanip<wchar_t>;
	
	using strmanip = basic_strmanip<char>;
	using wstrmanip = basic_strmanip<wchar_t>;
	using tstrmanip = basic_strmanip<TCHAR>;

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

	template<typename ToCharT, typename FromCharT>
	struct conversion_func_ {
		
	};
	
	template<>
	struct conversion_func_<char, wchar_t> {
		typedef char		to_char_type;
		typedef wchar_t		from_char_type;
		typedef errno_t(*std_func_t)(size_t*, to_char_type*, size_t, const from_char_type*, size_t);
		static constexpr std_func_t conversion = ::wcstombs_s;
	};

	template<>
	struct conversion_func_<wchar_t, char> {
		typedef wchar_t		to_char_type;
		typedef char		from_char_type;
		typedef errno_t(*std_func_t)(size_t*, to_char_type*, size_t, const from_char_type*, size_t);
		static constexpr std_func_t conversion = ::mbstowcs_s;
	};

	//primary template struct
	template<typename ToCharT, typename FromCharT,
		typename ToTraitsT   = std::char_traits<ToCharT>  , typename ToAllocT   = std::allocator<ToCharT>,
		typename FromTraitsT = std::char_traits<FromCharT>, typename FromAllocT = std::allocator<FromCharT>
	>
	struct string_conversion_ {
		typedef FromCharT from_char_type;
		typedef ToCharT to_char_type;

		typedef FromTraitsT from_traits_type;
		typedef FromAllocT from_allocator_type;

		typedef ToTraitsT to_traits_type;
		typedef ToAllocT to_allocator_type;

		typedef std::basic_string<from_char_type, from_traits_type, from_allocator_type> from_string_type;
		typedef std::basic_string<to_char_type, to_traits_type, to_allocator_type> to_string_type;

		static to_string_type convert_raw_with_len(const from_char_type* raw_str, size_t len) {
			size_t buf_size = 0;
			errno_t err = conversion_func_<to_char_type, from_char_type>::conversion(&buf_size, 0, 0, raw_str, len);
			if (err != 0) {
				return {};
			}

			to_string_type buf;
			buf.resize(buf_size);//include null character.
			err = conversion_func_<to_char_type, from_char_type>::conversion(&buf_size, &buf[0], buf.size(), raw_str, len);
			if (err != 0) {
				return {};
			}

			buf.resize(buf_size - 1);//remove null character.
			return buf;
		}

		static to_string_type convert_raw(const from_char_type* raw_str) {
			return convert_raw_with_len(raw_str, from_traits_type::length(raw_str));
		}

		static to_string_type convert_str(const from_string_type& str) {
			return convert_raw_with_len(str.c_str(), str.length());
		}
	};

	template< typename SameT,
		typename FromTraitsT , typename FromAllocT ,
		typename ToTraitsT , typename ToAllocT 
	>
	struct string_conversion_<SameT, SameT,
		FromTraitsT, FromAllocT,
		ToTraitsT, ToAllocT
	> {
		typedef SameT from_char_type;
		typedef SameT to_char_type;

		typedef FromTraitsT from_traits_type;
		typedef FromAllocT from_allocator_type;

		typedef ToTraitsT to_traits_type;
		typedef ToAllocT to_allocator_type;

		typedef std::basic_string<from_char_type, from_traits_type, from_allocator_type> from_string_type;
		typedef std::basic_string<to_char_type, to_traits_type, to_allocator_type> to_string_type;

		static to_string_type convert_raw(const from_char_type* raw_str) {
			return raw_str;
		}

		static to_string_type convert_str(const from_string_type& str) {
			return str;
		}

		static to_string_type convert_raw_with_len(const from_char_type* raw_str, size_t len) {
			return to_string_type(raw_str, len);
		}
	};

}//namespace yjj::impl

	//
	// raw string conversion ..
	//

	template<typename TraitsT = std::char_traits<char>, typename AllocT = std::allocator<char>>
	std::basic_string<char, TraitsT, AllocT>
		to_string(const wchar_t* str) noexcept {
		return yjj::impl::string_conversion_<char, wchar_t, TraitsT, AllocT>::convert_raw(str);
	}

	template<typename TraitsT = std::char_traits<wchar_t>, typename AllocT = std::allocator<wchar_t>>
	std::basic_string<wchar_t, TraitsT, AllocT>
		to_wstring(const char* str) noexcept {
		return yjj::impl::string_conversion_<wchar_t, char, TraitsT, AllocT>::convert_raw(str);
	}

	template<typename TraitsT = std::char_traits<TCHAR>, typename AllocT = std::allocator<TCHAR>>
	std::basic_string<TCHAR, TraitsT, AllocT>
		to_tstring(const char* str) noexcept {
		return yjj::impl::string_conversion_<TCHAR, char, TraitsT, AllocT>::convert_raw(str);
	}

	template<typename TraitsT = std::char_traits<TCHAR>, typename AllocT = std::allocator<TCHAR>>
	std::basic_string<TCHAR, TraitsT, AllocT>
		to_tstring(const wchar_t* str) noexcept {
		return yjj::impl::string_conversion_<TCHAR, wchar_t, TraitsT, AllocT>::convert_raw(str);
	}

	//
	// basic string conversion. to string
	//

	template<typename FromTraitsT, typename FromAllocT>
	std::basic_string<char, FromTraitsT, FromAllocT>
		to_string(const std::basic_string<char, FromTraitsT, FromAllocT>& str) noexcept {
		return yjj::impl::string_conversion_<char, char, FromTraitsT, FromAllocT, FromTraitsT, FromAllocT>::convert_str(str);
	}

	template<typename ToTraitsT, typename ToAllocT, typename FromTraitsT, typename FromAllocT>
	std::basic_string<char, ToTraitsT, ToAllocT>
		to_string(const std::basic_string<wchar_t, FromTraitsT, FromAllocT>& str) noexcept {
		return yjj::impl::string_conversion_<char, wchar_t, ToTraitsT, ToAllocT, FromTraitsT, FromAllocT>::convert_str(str);
	}

	//
	// basic string conversion. to wstring
	//

	template<typename ToTraitsT, typename ToAllocT, typename FromTraitsT, typename FromAllocT>
	std::basic_string<wchar_t, ToTraitsT, ToAllocT>
		to_wstring(const std::basic_string<char, FromTraitsT, FromAllocT>& str) noexcept {
		return yjj::impl::string_conversion_<wchar_t, char, ToTraitsT, ToAllocT, FromTraitsT, FromAllocT>::convert_str(str);
	}

	template<typename FromTraitsT, typename FromAllocT>
	std::basic_string<wchar_t, FromTraitsT, FromAllocT>
		to_wstring(const std::basic_string<wchar_t, FromTraitsT, FromAllocT>& str) noexcept {
		return yjj::impl::string_conversion_<wchar_t, wchar_t, FromTraitsT, FromAllocT, FromTraitsT, FromAllocT>::convert_str(str);
	}


	//
	// basic string conversion. to tstring
	//

	template<typename ToTraitsT, typename ToAllocT, typename FromTraitsT, typename FromAllocT>
	std::basic_string<TCHAR, ToTraitsT, ToAllocT>
		to_tstring(const std::basic_string<char, FromTraitsT, FromAllocT>& str) noexcept {
		return yjj::impl::string_conversion_<TCHAR, char, ToTraitsT, ToAllocT, FromTraitsT, FromAllocT>::convert_str(str);
	}

	template<typename ToTraitsT, typename ToAllocT, typename FromTraitsT, typename FromAllocT>
	std::basic_string<TCHAR, ToTraitsT, ToAllocT>
		to_tstring(const std::basic_string<wchar_t, FromTraitsT, FromAllocT>& str) noexcept {
		return yjj::impl::string_conversion_<TCHAR, wchar_t, ToTraitsT, ToAllocT, FromTraitsT, FromAllocT>::convert_str(str);
	}

	inline tstring to_tstring(bool b) noexcept {
		return b ? TEXT("true") : TEXT("false");
	}
}

