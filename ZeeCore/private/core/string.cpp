#include "../../public/core/string.h"
#include "../../public/core/core_base.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>

namespace zee {
namespace impl {

}//namespace zee::impl
	int format_helper::calculate_buffer_size(const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int ret = calculate_buffer_size(format, args);
		va_end(args);
		return ret;
	}

	int format_helper::calculate_buffer_size(const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int ret = calculate_buffer_size(format, args);
		va_end(args);
		return ret;
	}

	int format_helper::calculate_buffer_size(const wchar_t* format, va_list args) noexcept {
		return vswprintf(NULL, 0, format, args);
	}

	int format_helper::calculate_buffer_size(const char* format, va_list args) noexcept {
		return vsnprintf(NULL, 0, format, args);
	}

	std::string string_sprintf(const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		std::string buffer_;
		string_vprintf(buffer_, format, args);
		va_end(args);
		return buffer_;
	}

	int string_sprintf(std::string& buffer_, const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int wrote_count = string_vprintf(buffer_, format, args);
		va_end(args);
		return wrote_count;
	}

	std::string string_vprintf(const char* format, va_list args) noexcept {
		std::string buffer_;
		string_vprintf(buffer_, format, args);
		return buffer_;
	}

	int string_vprintf(std::string& buffer_, const char* format, va_list args) noexcept {
		int need_count = format_helper::calculate_buffer_size(format, args);
		if (need_count < 0) {
			return need_count;
		}
		buffer_.clear();
		buffer_.resize((size_t)need_count + 1);
		return vsnprintf(&buffer_[0], buffer_.size(), format, args);
	}

	std::wstring wstring_sprintf(const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		std::wstring buffer_;
		wstring_vprintf(buffer_, format, args);
		va_end(args);
		return buffer_;
	}

	int wstring_sprintf(std::wstring& buffer_, const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		const int wrote_count = wstring_vprintf(buffer_, format, args);
		va_end(args);
		return wrote_count;
	}

	std::wstring wstring_vprintf(const wchar_t* format, va_list args) noexcept {
		std::wstring buffer_;
		wstring_vprintf(buffer_, format, args);
		return buffer_;
	}

	int wstring_vprintf(std::wstring& buffer_, const wchar_t* format, va_list args) noexcept {
		int need_count = format_helper::calculate_buffer_size(format, args);
		if (need_count < 0) {
			return need_count;
		}
		buffer_.clear();
		buffer_.resize((size_t)need_count + 1);
		return vswprintf(&buffer_[0], buffer_.size(), format, args);
	}

	tstring tstring_sprintf(const TCHAR* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		tstring buffer_ = tstring_vprintf(format, args);
		va_end(args);
		return buffer_;
	}

	int tstring_vprintf(tstring& buffer_, const TCHAR* format, va_list args) noexcept {
		int need_count = format_helper::calculate_buffer_size(format, args);
		if (need_count < 0) {
			return need_count;
		}

		buffer_.clear();
		if (buffer_.size() < (size_t)need_count + 1) {
			buffer_.resize((size_t)need_count + 1);
		}

		return tvprintf(&buffer_[0], buffer_.size(), format, args);
	}

	int tstring_sprintf(tstring& buffer_, const TCHAR* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		const int wrote_count = tstring_vprintf(buffer_, format, args);
		va_end(args);
		return wrote_count;
	}

	tstring tstring_vprintf(const TCHAR* format, va_list args) noexcept {
		tstring buffer_;
		tstring_vprintf(buffer_, format, args);
		return buffer_;
	}


	tstring current_time_to_tstring(const TCHAR* const format) noexcept {
		time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm tmm;
		if (localtime_s(&tmm, &t) != 0) {
			return TEXT("wrong time.");
		}
		
		tstring buffer_;
		buffer_.resize(32);
		size_t written_size = 0;
		while (
			(written_size = 
#ifdef UNICODE
			std::wcsftime
#else
			std::strftime
#endif
			(&buffer_[0], buffer_.size() - 1, format, &tmm)) == 0) {
			buffer_.resize(buffer_.size() * 2);
		}
		buffer_.resize((size_t)written_size);
		return buffer_;
	}

#ifdef UNICODE
#define ZEE_TVPRINTF vswprintf
#else 
#define ZEE_TVPRINTF vsnprintf
#endif

	int tvprintf(TCHAR* buffer, size_t buf_size, const TCHAR* format, va_list args)	{
		return ZEE_TVPRINTF(buffer, buf_size, format, args);
	}

	int tsprintf(TCHAR* buffer, size_t buf_size, const TCHAR* format, ...) {
		va_list args;
		va_start(args, format);
		int ret = tvprintf(buffer, buf_size, format, args);
		va_end(args);
		return ret;
	}

	int tsscanf(const TCHAR* const buffer, const TCHAR* format, ...) noexcept {
		return 0;
	}

	int tvscanf(const TCHAR* const buffer, const TCHAR* format, va_list args) noexcept {
		return 0;
	}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

//https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/
	tstring to_tstring(const char* c_str) {
#ifdef UNICODE

		std::wstring buffer(std::strlen(c_str), 0);
		buffer.resize(std::mbstowcs(0, c_str, 0) + 1);
		std::mbstowcs(buffer.data(), c_str, buffer.size());
		return buffer;
#else
		return c_str;
#endif
	}

	tstring to_tstring(const wchar_t* c_str) {
#ifdef UNICODE
		return c_str;
#else
		std::string buffer(std::wcslen(c_str), 0);
		buffer.resize(std::wcstombs(0, c_str, 0) + 1);
		std::wcstombs(buffer.data(), c_str, buffer.size());
		return buffer;
#endif
	}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
}