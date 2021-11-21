#include <cstdarg>
#include <cstdio>
#include "../../public/core/string.h"
#include "../../public/core/core_base.h"

namespace zee {

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

	std::string string_format(const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		std::string buffer_;
		string_vformat(buffer_, format, args);
		va_end(args);
		return buffer_;
	}

	int string_format(std::string& buffer_, const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int wrote_count = string_vformat(buffer_, format, args);
		va_end(args);
		return wrote_count;
	}

	std::string string_vformat(const char* format, va_list args) noexcept {
		std::string buffer_;
		string_vformat(buffer_, format, args);
		return buffer_;
	}

	int string_vformat(std::string& buffer_, const char* format, va_list args) noexcept {
		int need_count = format_helper::calculate_buffer_size(format, args);
		if (need_count < 0) {
			return need_count;
		}
		buffer_.clear();
		buffer_.resize((size_t)need_count + 1);
		return vsnprintf(&buffer_[0], buffer_.size(), format, args);
	}

	std::wstring wstring_format(const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		std::wstring buffer_;
		wstring_vformat(buffer_, format, args);
		va_end(args);
		return buffer_;
	}

	int wstring_format(std::wstring& buffer_, const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		const int wrote_count = wstring_vformat(buffer_, format, args);
		va_end(args);
		return wrote_count;
	}

	std::wstring wstring_vformat(const wchar_t* format, va_list args) noexcept {
		std::wstring buffer_;
		wstring_vformat(buffer_, format, args);
		return buffer_;
	}

	int wstring_vformat(std::wstring& buffer_, const wchar_t* format, va_list args) noexcept {
		int need_count = format_helper::calculate_buffer_size(format, args);
		if (need_count < 0) {
			return need_count;
		}
		buffer_.clear();
		buffer_.resize((size_t)need_count + 1);
		return vswprintf(&buffer_[0], buffer_.size(), format, args);
	}

	tstring tstring_format(const TCHAR* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		tstring buffer_ = tstring_vformat(format, args);
		va_end(args);
		return buffer_;
	}

	int tstring_vformat(tstring& buffer_, const TCHAR* format, va_list args) noexcept {
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

	int tstring_format(tstring& buffer_, const TCHAR* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		const int wrote_count = tstring_vformat(buffer_, format, args);
		va_end(args);
		return wrote_count;
	}

	tstring tstring_vformat(const TCHAR* format, va_list args) noexcept {
		tstring buffer_;
		tstring_vformat(buffer_, format, args);
		return buffer_;
	}


	tstring current_time_to_tstring() noexcept {
		time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm tmm;
		if (localtime_s(&tmm, &t) != 0) {
			return TEXT("wrong time.");
		}

		tstring buffer_;
		buffer_.resize(32);
#ifdef UNICODE
		while (
			std::wcsftime
#else
			std::strftime
#endif
			(&buffer_[0], buffer_.size() - 1, TEXT("%y-%m-%d %H %M %S"), &tmm) == 0) {
			buffer_.resize(buffer_.size() * 2);
		}

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
}