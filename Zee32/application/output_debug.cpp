#include <zlog/zlog.h>
#include <interfaces/loggable.h>
#include "../win32helper/win32helper.h"

namespace impl {
    class default_debug_ouput_logger final : public zee::interfaces::loggable {
        virtual void print(const TCHAR* null_terminated_str, size_t length) final {
            OutputDebugString(null_terminated_str);
        }

        virtual void on_bind(zee::log& owner_log) final {
            owner_log.printf(zee::log::verbose_type::normal, TEXT("debug_output"), TEXT("debug_output logger created."));
        }

        virtual void on_unbind() final {

        }
    };

    static struct debug_output_logger_creator_ {
        debug_output_logger_creator_() {
            auto new_logger = std::make_shared<default_debug_ouput_logger>();
            zee::get_log().add(TEXT("ouput_debug"), std::static_pointer_cast<zee::interfaces::loggable>(new_logger));
        }
    } creator;
}
