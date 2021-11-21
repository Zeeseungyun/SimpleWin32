#pragma once
#include "../core/string.h"
#include <vector>
#include <set>
#include <map>
#include <memory>

namespace zee {
namespace interfaces {
	struct loggable;
}//zee::interfaces

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

		static const TCHAR* verbose_to_raw_str(verbose_type vb);
		enum class logger_result {
			success,
			logger_is_nullptr,
			already_used_tag_name,
			already_turn_off, 
			already_turn_on,
		};

		void add(const tstring& tag, std::shared_ptr<interfaces::loggable> new_logger);
		void remove(std::shared_ptr<interfaces::loggable> new_logger);
		void remove(const tstring& tag);

		std::vector<tstring> get_logger_tag_names() const;

		void printf_detail(verbose_type vb, const TCHAR* category_name, const TCHAR* file_name, int line, const TCHAR* format, ...);
		void printf(verbose_type vb, const TCHAR* category_name, const TCHAR* format, ...);

		void turn_on(const tstring& category_name);
		void turn_off(const tstring& category_name);

		bool is_on(const tstring& category_name) const;
		bool is_off(const tstring& category_name) const;
		
	private:
		void flush_();
		void clear_buffers();

	private:
		size_t number_ = 0;
		size_t header_buffer_size_ = 0;
		size_t buffer_size_ = 0;
		tstring header_buffer_;
		tstring buffer_;

		std::set<tstring> off_categories_;
		std::set<tstring> on_categories_;

		struct tag_logger_pair {
			tstring tag;
			int priority;
			std::shared_ptr<interfaces::loggable> logger;
		};
		
		std::map<tstring, std::shared_ptr<interfaces::loggable>> loggers_;
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
