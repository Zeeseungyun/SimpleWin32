#include "zlog.h"
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include "../math/common.h"
#include "../interfaces/loggable.h"

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
		static const TCHAR* const header_format = TEXT("[%6d][%s][%s][%s][line:%d][file:%s]: ");
		static const TCHAR* const content_format = TEXT("%s");
		if (is_off(category_name)) {
			return;
		}

		const auto current_tiem_str = current_time_to_tstring();
		const int header_size = format_helper::calculate_buffer_size(header_format,
			number_,
			current_tiem_str.c_str(),
			verbose_to_raw_str(vb),
			category_name,
			file_name,
			line
		);

		if (header_size < 0) {
			return;
		}

		va_list args;
		va_start(args, format);

		const int content_size = format_helper::calculate_buffer_size(content_format, args);
		if (content_size > 0) {
			size_t new_buffer_size = header_size + content_size;
			buffer_.clear();
			buffer_.resize(new_buffer_size + 1);
			tsprintf(buffer_.data(), buffer_.size(), header_format,
				number_,
				current_tiem_str.c_str(),
				verbose_to_raw_str(vb),
				category_name,
				file_name,
				line
			);

			tvprintf(buffer_.data() + header_size, buffer_.size() - header_size, content_format,
				args
			);

			number_ = math::clamp(number_ + 1, 0, 1000000);
			flush_();
		}

		va_end(args);
	}

	void log::printf(verbose_type vb, const TCHAR* category_name, const TCHAR* format, ...) {
		static const TCHAR* const header_format = TEXT("[%6d][%s][%s][%s]: ");
		static const TCHAR* const content_format = TEXT("%s");
		if (is_off(category_name)) {
			return;
		}

		const auto current_tiem_str = current_time_to_tstring();
		const int header_size = format_helper::calculate_buffer_size(header_format,
			number_,
			current_tiem_str.c_str(),
			verbose_to_raw_str(vb),
			category_name
		);

		if (header_size < 0) {
			return;
		}

		va_list args;
		va_start(args, format);

		const int content_size = format_helper::calculate_buffer_size(content_format, args);
		if (content_size > 0) {
			size_t new_buffer_size = header_size + content_size;
			buffer_.clear();
			buffer_.resize(new_buffer_size + 1);
			tsprintf(buffer_.data(), buffer_.size(), header_format,
				number_,
				current_tiem_str.c_str(),
				verbose_to_raw_str(vb),
				category_name
			);

			tvprintf(buffer_.data() + header_size, buffer_.size() - header_size, content_format,
				args
			);

			number_ = math::clamp(number_ + 1, 0, 1000000);
			flush_();
		}

		va_end(args);
	}

	void log::turn_on(const tstring& category_name) {
		auto found = off_categories_.find(category_name);
		if (found != end(off_categories_)) {
			off_categories_.erase(found);
			on_categories_.insert(category_name);
		}
	}

	void log::turn_off(const tstring& category_name) {
		auto found = on_categories_.find(category_name);
		if (found != end(on_categories_)) {
			on_categories_.erase(found);
			off_categories_.insert(category_name);
		}
	}

	log& get_log() noexcept	{
		static std::unique_ptr<log> log_instance;
		if (!log_instance) {
			log_instance = std::make_unique<log>(new log());
		}
		return *log_instance;
	}

	void log::flush_() {
		for (auto iter = begin(loggers_); iter != end(loggers_);) {
			if (auto logger = iter->lock()) {
				logger->print(header_buffer_.c_str(), header_buffer_size_);
				logger->print(buffer_.c_str(), buffer_size_);
				logger->print(TEXT("\n"), 1);
				++iter;
			} else {
				iter = loggers_.erase(iter);
			}
		}

		clear_buffers();
	}

	void log::clear_buffers() {
		header_buffer_.clear();
		buffer_.clear();
		header_buffer_size_ = 0;
		buffer_size_ = 0;
	}

	bool log::is_on(const tstring& category_name) const {
		return on_categories_.find(category_name) != end(on_categories_);
	}

	bool log::is_off(const tstring& category_name) const {
		return off_categories_.find(category_name) != end(off_categories_);
	}

	void log::add(const tstring& logger_tag, std::shared_ptr<interfaces::loggable> new_logger) {

		loggers_.push_back(new_logger);
		new_logger->on_bind(*this);
	}

	void log::remove(std::shared_ptr<interfaces::loggable> remove_logger) {
		remove_logger->on_bind(*this);
	}

}
