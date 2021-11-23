#include "../../public/zlog/zlog.h"
#include "../../public/math/common.h"
#include "../../public/interfaces/loggable.h"
#include "../../public/util/alogorithm.h"
#include "../../public/file/path.h"

#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <filesystem>

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

namespace impl {
	namespace fs = std::filesystem;
	struct default_logger : interfaces::loggable {
		friend class zee::log;
		void print(const TCHAR* null_terminated_str, size_t length = tstring::npos) final {
			length = length == tstring::npos ? tstrmanip::length(null_terminated_str) : length;
			out.write(null_terminated_str, length);
			out.flush();
		}
		
		void on_bind(log& owner_log) final {
		}
		
		void on_unbind() final {
		}

	private:
		default_logger() {
			
			tstring log_file_path = zee::file::paths::log_dir() + TEXT("log_") + tstrmanip::cur_time_to_str(TEXT("%y_%m_%d_%H_%M_%S")) + TEXT(".txt");
			if (!fs::exists(file::paths::log_dir())) {
				fs::create_directories(file::paths::log_dir());
			}

			std::set<fs::path> paths;
			for (const auto& dir_entry : fs::directory_iterator(file::paths::log_dir())) {
				if (dir_entry.is_regular_file()) {
					paths.insert(dir_entry.path());
				}
			}

			constexpr size_t limit_log_file_count = 10;
			while (size(paths) > limit_log_file_count) {
				fs::remove(*begin(paths));
				paths.erase(begin(paths));
			}

			out.open(log_file_path);
		}

		std::basic_ofstream<TCHAR> out;
	};

}//namespace impl

	log::log() noexcept {
		std::shared_ptr<impl::default_logger> default_logger(new impl::default_logger);
		add(TEXT("default"), default_logger);
		printf(log::verbose_type::normal, TEXT("core"), TEXT("default_logger created."));
	}

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
		if (category_status(category_name) == status::off) {
			return;
		}

		buffer_.clear();
		tstrmanip::sprintf(buffer_, TEXT("[%6d][%s][%s][%s][line:%d][file:%s]: "), 
			number_,
			tstrmanip::cur_time_to_str().c_str(),
			verbose_to_raw_str(vb),
			category_name,
			line,
			file_name
		);

		va_list args;
		va_start(args, format);
		tstrmanip::vprintf(buffer_, format, args);
		va_end(args);

		buffer_ += TEXT("\n");
		number_ = math::clamp(number_ + 1, 0, 1000000);
		flush_();
	}

	void log::printf(verbose_type vb, const TCHAR* category_name, const TCHAR* format, ...) {
		if (category_status(category_name) == status::off) {
			return;
		}

		buffer_.clear();
		tstrmanip::sprintf(buffer_, TEXT("[%6d][%s][%s][%s]: "),
			number_,
			tstrmanip::cur_time_to_str().c_str(),
			verbose_to_raw_str(vb),
			category_name
		);

		va_list args;
		va_start(args, format);
		tstrmanip::vprintf(buffer_, format, args);
		va_end(args);

		buffer_ += TEXT("\n");
		number_ = math::clamp(number_ + 1, 0, 1000000);
		flush_();
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
			if (logger_info.is_on) {
				logger_info.logger->print(buffer_.c_str(), buffer_.size());
			}
		}

		buffer_.clear();
	}

	log::status log::category_status(const tstring& category_name) const {
		if (off_categories_.find(category_name) != end(off_categories_)) {
			return status::off;
		}
		return status::on;
	}

	void log::turn_on_logger(const tstring& tag_name) {
		auto found = tag_map_idx_.find(tag_name);
		if (found != end(tag_map_idx_)) {
			logger_infos_[found->second].is_on = true;
		}
	}

	void log::turn_off_logger(const tstring& tag_name) {
		auto found = tag_map_idx_.find(tag_name);
		if (found != end(tag_map_idx_)) {
			logger_infos_[found->second].is_on = false;
		}
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
		if (algo::remove_single(logger_infos_, begin(logger_infos_), end(logger_infos_), 
			[&](const logger_info& old_item) {
				if (old_item.logger == remove_logger) {
					logger_tag = old_item.tag;
					return true;
				}
				return false;
			})) {
			tag_map_idx_.erase(logger_tag);
		}
	}

	void log::remove(const tstring& remove_tag) {
		auto found = tag_map_idx_.find(remove_tag);
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
