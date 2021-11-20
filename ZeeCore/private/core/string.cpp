#include <cstdarg>
#include <cstdio>
#include "../../public/core/string.h"
#include "../../public/core/core_base.h"

namespace zee {
	
	std::string string_format(const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int need_count = vsnprintf(NULL, 0, format, args);
		std::string buffer((size_t)need_count + 1, 0);
		vsnprintf(&buffer[0], buffer.size(), format, args);
		va_end(args);
		return buffer;
	}

	void string_format(std::string& buffer, const char* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int need_count = vsnprintf(NULL, 0, format, args);
		buffer.clear();
		buffer.resize((size_t)need_count + 1);
		need_count = vsnprintf(&buffer[0], buffer.size(), format, args);
		va_end(args);
	}

	std::wstring wstring_format(const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int need_count = vswprintf(NULL, 0, format, args);
		std::wstring buffer((size_t)need_count + 1, 0);
		vswprintf(&buffer[0], buffer.size(), format, args);
		va_end(args);
		return buffer;
	}

	void wstring_format(std::wstring& buffer, const wchar_t* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int need_count = vswprintf(NULL, 0, format, args);
		buffer.clear();
		buffer.resize((size_t)need_count + 1);
		need_count = vswprintf(&buffer[0], buffer.size(), format, args);
		va_end(args);
	}

#ifdef UNICODE
#define ZEE_TVPRINTF vswprintf
#else 
#define ZEE_TVPRINTF vsnprintf
#endif

	tstring tstring_format(const TCHAR* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int need_count = ZEE_TVPRINTF(NULL, 0, format, args);
		tstring buffer((size_t)need_count + 1, 0);
		ZEE_TVPRINTF(&buffer[0], buffer.size(), format, args);
		va_end(args);
		return buffer;
	}

	void tstring_format(tstring& buffer, const TCHAR* format, ...) noexcept {
		va_list args;
		va_start(args, format);
		int need_count = ZEE_TVPRINTF(NULL, 0, format, args);
		buffer.clear();
		buffer.resize((size_t)need_count + 1);
		need_count = ZEE_TVPRINTF(&buffer[0], buffer.size(), format, args);
		va_end(args);
	}

	tstring current_time_to_tstring() noexcept {
		time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm tmm;
		if (localtime_s(&tmm, &t) != 0) {
			return TEXT("wrong time.");
		}

		tstring buffer;
		buffer.resize(32);
#ifdef UNICODE
		while (
			std::wcsftime
#else
			std::strftime
#endif
			(&buffer[0], buffer.size() - 1, TEXT("%y-%m-%d %H:%M:%S"), &tmm) == 0) {
			buffer.resize(buffer.size() * 2);
		}

		return buffer;
	}

}