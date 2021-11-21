#include "zlog.h"
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <Windows.h>

namespace zee {
	//static tstring error_to_str(DWORD error_code) {
	//	tstring ret;
	//	HLOCAL hLocal = NULL;
	//	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
	//		nullptr,
	//		error_code,
	//		GetLocaleInfo(GetUserDefaultLCID(), LOCALE_ALL, NULL, 0),
	//		reinterpret_cast<LPTSTR>(&hLocal),
	//		0,
	//		NULL
	//	);
	//
	//	ret = reinterpret_cast<const LPTSTR>(hLocal);
	//	LocalFree(hLocal);
	//	return ret;
	//}
	//
	//tstring log::last_error_to_string() {
	//	return error_to_str(GetLastError());
	//}

	const TCHAR* log::verbose_to_raw_str(verbose_type  vb) {
		switch (vb)
		{
		case zee::log::verbose_type::normal:
			return TEXT("normal");
			break;
		case zee::log::verbose_type::warning:
			return TEXT("warning");
			break;
		case zee::log::verbose_type::fatal:
			return TEXT("fatal");
			break;
		default:
			break;
		}

		return TEXT("normal");
	}

	void log::printf_detail(verbose_type vb, const TCHAR* category_name, const TCHAR* file_name, int line, const TCHAR* format, ...) {
		int written_size = -1;
		buffer_.clear();
		header_buffer_size_ = 0;
		buffer_size_ = 0;

		written_size = tstring_format(header_buffer_, TEXT("[%6d][%s][%s][%s]: "),
			number_,
			current_time_to_tstring().c_str(),
			verbose_to_raw_str(vb),
			category_name
		);

		if (written_size < 0 ) {
			return;
		}

		header_buffer_size_ = (size_t)written_size;

		va_list args;
		va_start(args, format);
		written_size = tstring_vformat(buffer_, TEXT("[%s]"), args);
		if (written_size < 0) {
			return;
		}

	}

	void log::printf(verbose_type vb, const TCHAR* category_name, const TCHAR* format, ...) {

	}

	void log::turn_on(const tstring& category_name) {

	}

	void log::turn_off(const tstring& category_name) {

	}

	int log::write_header_(verbose_type vb, const TCHAR* category_name) {
		return tstring_format(header_buffer_, TEXT("[%6d][%s][%s][%s]"),
			number_,
			current_time_to_tstring().c_str(),
			verbose_to_raw_str(vb),
			category_name
		);
	}

	int log::write_content_() {

	}

	void log::printf_detail(verbose_type vb, const TCHAR* file_name, int line, const TCHAR* format, ...) const {
		buffer_.clear();
		size_t buffer_size = buffer_.size();
		if (buffer_size == 0) {
			++buffer_size;
			buffer_.resize(buffer_size, 0);
		}

		va_list args;
		va_start(args, format);

		while (
#ifdef UNICODE
			std::vswprintf
#else
			std::vsprintf
#endif
				(&buffer_[0], buffer_size - 1, format, args) == -1) {
			buffer_size *= 2;
			buffer_.resize(buffer_size, 0);
		}

		va_end(args);
		
		header_buffer_.clear();
		tstring_format(header_buffer_, TEXT("[%6d][%s][%s][%s][%4d]line : "),
			number_++,
			current_time_to_tstring().c_str(),
			verbose_to_raw_str(vb),
			file_name,
			line
		);
		
		if (number_ > 1000000) {
			number_ = 0;
		}

		OutputDebugString(header_buffer_.c_str());
		OutputDebugString(buffer_.c_str());
		OutputDebugString(_T("\n"));
	}

	void log::printf(verbose_type vb, const TCHAR* format, ...) const {

		buffer_.clear();
		size_t buffer_size = buffer_.size();
		if (buffer_size == 0) {
			++buffer_size;
			buffer_.resize(buffer_size, 0);
		}

		va_list args;
		va_start(args, format);

		while (
#ifdef UNICODE
			std::vswprintf
#else
			std::vsprintf
#endif
			(&buffer_[0], buffer_size - 1, format, args) == -1) {
			buffer_size *= 2;
			buffer_.resize(buffer_size, 0);
		}

		va_end(args);

		header_buffer_.clear();
		tstring_format(header_buffer_, TEXT("[%6d][%s][%s] : "),
			number_++,
			current_time_to_tstring().c_str(),
			verbose_to_raw_str(vb)
		);

		if (number_ > 1000000) {
			number_ = 0;
		}

		OutputDebugString(header_buffer_.c_str());
		OutputDebugString(buffer_.c_str());
		OutputDebugString(TEXT("\n"));
	}

	log& get_log() noexcept	{
		static std::unique_ptr<log> log_instance;
		if (!log_instance) {
			log_instance = std::make_unique<log>(new log());
		}
		return *log_instance;
	}
}
