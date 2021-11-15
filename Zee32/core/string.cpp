#include "string.h"
#include "core/core.h"
#include <cstdarg>
#include <cstdio>

namespace zee {

	tstring current_time_to_string() {
		time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm tmm;
		if (localtime_s(&tmm, &t) != 0) {
			return _T("wrong time.");
		}

		tstring temp_buffer;
		temp_buffer.resize(32);
#ifdef UNICODE
		while (
			std::wcsftime
#else
			std::strftime
#endif
			(&temp_buffer[0], temp_buffer.size() - 1, _T("%y-%m-%d %H:%M:%S"), &tmm) == 0) {
			temp_buffer.resize(temp_buffer.size() * 2);
		}

		return temp_buffer;
	}

	tstring tstring_format(const TCHAR* format, ...) {
		tstring buffer; 
		buffer.resize(1);

		va_list args;
		va_start(args, format);

		while (
#ifdef UNICODE
			std::vswprintf(
#else
			std::vsprintf(
#endif
			&buffer[0], buffer.size(), format, args) == -1) {
			buffer.resize(buffer.size() * 2, 0);
		}

		va_end(args);
		return buffer;
	}

	void tstring_format(tstring& buffer, const TCHAR* format, ...) {
		buffer.clear();
		if (buffer.empty()) {
			buffer.resize(1);
		}

		va_list args;
		va_start(args, format);

		while (
#ifdef UNICODE
			std::vswprintf(
#else
			std::vsprintf(
#endif
			&buffer[0], buffer.size() - 1, format, args) == -1) {
			buffer.resize(buffer.size() * 2, 0);
		}

		va_end(args);
	}
}