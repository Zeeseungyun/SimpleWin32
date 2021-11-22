#pragma once
#include "../core/string.h"
#include "../core/core_base.h"

#include <vector>
#include <set>
#include <map>
#include <memory>

namespace zee {
namespace interfaces {
	struct loggable;
}//zee::interfaces

/*
* TODO
* flush_ 마저구현
* header_buffer_ 삭제
* add 시 로거 tag 관련 로직 작성.
* 및 테스트.
*/
	class log& get_log() noexcept;
	class log {
		friend log& get_log() noexcept;
		log() noexcept;

	public:
		enum class verbose_type {
			normal,
			warning,
			fatal
		};

		enum class status {
			on,
			off,
			not_exists
		};

		static const TCHAR* verbose_to_raw_str(verbose_type vb);

		void add(const tstring& tag, const std::shared_ptr<interfaces::loggable>& new_logger, int32 new_priority = 0);
		void remove(const std::shared_ptr<interfaces::loggable>& remove_logger);
		void remove(const tstring& tag);
		void change_priority(const tstring& tag, int32 new_priority);

		std::vector<tstring> get_logger_tag_names() const;

		void printf_detail(verbose_type vb, const TCHAR* category_name, const TCHAR* file_name, int32 line, const TCHAR* format, ...);
		void printf(verbose_type vb, const TCHAR* category_name, const TCHAR* format, ...);

		void turn_on_category(const tstring& category_name);
		void turn_off_category(const tstring& category_name);

		status category_status(const tstring& category_name) const;
		status logger_status(const tstring& logger_tag) const;

		void turn_on_logger(const tstring& tag_name);
		void turn_off_logger(const tstring& tag_name);

	private:
		void flush_();
		void clear_buffers();

		struct logger_info {
			int32 priority = -1;
			bool is_on = true;
			tstring tag;
			std::shared_ptr<interfaces::loggable> logger;
			bool operator<(const logger_info& other) const noexcept {
				return priority < other.priority;
			}
		};

	private:
		size_t number_ = 0;
		size_t buffer_size_ = 0;
		tstring buffer_;

		std::set<tstring> off_categories_;

		std::vector<logger_info> logger_infos_;
		std::map<tstring, size_t> tag_map_idx_;
	};
	
#define ZEE_LOG_DETAIL(verbose, category_name, log_content, ...) \
		[]() ->void { get_log().printf_detail(log::verbose_type::##verbose, category_name, __FILE__, __LINE__, log_content, __VA_ARGS__); \
		}()

#define ZEE_CLOG_DETAIL(condtion, verbose, category_name, log_content, ...) \
		[]() ->void { if((bool)(condition)) get_log().printf_detail(log::verbose_type::##verbose, category_name, __FILE__, __LINE__, log_content, __VA_ARGS__); \
		}()

#define ZEE_LOG(verbose, category_name, log_content, ...) \
		[]() ->void {  get_log().printf(log::verbose_type::##verbose, category_name, log_content, __VA_ARGS__); \
		}()

#define ZEE_CLOG(condtion, verbose, category_name, log_content, ...) \
		[]() ->void { if((bool)(condition)) get_log().printf(log::verbose_type::##verbose, category_name, log_content, __VA_ARGS__); \
		}()
}//namespace zee
