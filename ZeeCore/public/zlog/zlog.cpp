#include "zlog.h"
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <cassert>
#include "../math/common.h"
#include "../interfaces/loggable.h"
#include <algorithm>
#include "../util/alogorithm.h"

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

	void log::printf_detail(verbose_type vb, const TCHAR* category_name, const TCHAR* file_name, int32 line, const TCHAR* format, ...) {
		static const TCHAR* const header_format = TEXT("[%6d][%s][%s][%s][line:%d][file:%s]: ");
		static const TCHAR* const content_format = TEXT("%s");
		if (category_status(category_name) == status::off) {
			return;
		}

		const auto current_tiem_str = current_time_to_tstring();
		const int32 header_size = format_helper::calculate_buffer_size(header_format,
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

		const int32 content_size = format_helper::calculate_buffer_size(content_format, args);
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
		if (category_status(category_name) == status::off) {
			return;
		}

		const auto current_tiem_str = current_time_to_tstring();
		const int32 header_size = format_helper::calculate_buffer_size(header_format,
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

		const int32 content_size = format_helper::calculate_buffer_size(content_format, args);
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

	void log::turn_on_category(const tstring& category_name) {
		auto found = off_categories_.find(category_name);
		if (found != end(off_categories_)) {
			off_categories_.erase(found);
		}
	}

	void log::turn_off_category(const tstring& category_name) {
		off_categories_.insert(category_name);
	}

	log& get_log() noexcept	{
		static std::unique_ptr<log> log_instance;
		if (!log_instance) {
			log_instance = std::unique_ptr<log>(new log());
		}
		return *log_instance;
	}

	void log::flush_() {
		for (const auto& logger_info : logger_infos_) {
			logger_info.logger->print(buffer_.c_str(), buffer_size_);
			logger_info.logger->print(TEXT("\n"), 1);
		}

		clear_buffers();
	}

	void log::clear_buffers() {
		buffer_.clear();
		buffer_size_ = 0;
	}

	log::status log::category_status(const tstring& category_name) const {
		if (off_categories_.find(category_name) != end(off_categories_)) {
			return status::off;
		}
		return status::on;
	}

	void log::turn_on_logger(const tstring& tag_name) {

	}

	void log::turn_off_logger(const tstring& tag_name) {

	}

	log::status log::logger_status(const tstring& logger_tag) const {
		auto found = tag_map_idx_.find(logger_tag);
		if (found == end(tag_map_idx_)) {
			return status::not_exists;
		}
		return logger_infos_[found->second].is_on ? status::on : status::off;
	}

	void log::add(const tstring& new_tag_name, const std::shared_ptr<interfaces::loggable>& new_logger, int32 new_priority) {
		logger_info new_item = { new_priority, true, new_tag_name, new_logger };
		algo::remove_all(logger_infos_, begin(logger_infos_), end(logger_infos_), [&](const logger_info& old_item){
			if (old_item.tag == new_item.tag || old_item.logger == new_item.logger) {
				assert(old_item.tag == new_item.tag && "new_logger was previously inserted. new_logger must be unique.");
				return true;
			}
			return false;
		});

		auto inserted_iter = logger_infos_.insert(upper_bound(begin(logger_infos_), end(logger_infos_), new_item), std::move(new_item));
		tag_map_idx_[new_tag_name] = (size_t)distance(inserted_iter, begin(logger_infos_));
		new_logger->on_bind(*this);
	}

	void log::remove(const std::shared_ptr<interfaces::loggable>& remove_logger) {
		tstring logger_tag;
		algo::remove_single(logger_infos_, begin(logger_infos_), end(logger_infos_), [&](const logger_info& old_item){
			if (old_item.logger == remove_logger) {
				logger_tag = old_item.tag;
				return true;
			}
			return false;
		});

		tag_map_idx_.erase(logger_tag);
	}

	void log::remove(const tstring& tag) {
		auto found = tag_map_idx_.find(tag);
		if (end(tag_map_idx_) != found) {
			logger_infos_.erase(begin(logger_infos_) + found->second);
			tag_map_idx_.erase(found);
		}
	}

	void log::change_priority(const tstring& tag, int32 new_priority) {
		auto found = tag_map_idx_.find(tag);
		if (end(tag_map_idx_) != found) {
			logger_info target_info = std::move(logger_infos_[found->second]);
			logger_infos_.erase(begin(logger_infos_) + found->second);
			tag_map_idx_.erase(found);
			add(target_info.tag, target_info.logger, target_info.priority);
		}
	}

	std::vector<tstring> log::get_logger_tag_names() const {
		std::vector<tstring> ret;
		ret.reserve(logger_infos_.size());
		for (const auto& info : logger_infos_) {
			ret.push_back(info.tag);
		}
		return ret;
	}
}
