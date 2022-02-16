#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include "interfaces/loggable.h"

#include "zlog/zlog.h"
namespace impl {
    class default_debug_ouput_logger final : public yjj::interfaces::loggable {
        virtual void print(const TCHAR* null_terminated_str, size_t length) final {
            OutputDebugString(null_terminated_str);
        }

        virtual void on_bind(yjj::log& owner_log) final {
            owner_log.printf(yjj::log::verbose_type::normal, TEXT("debug_output"), TEXT("debug_output logger created."));
        }

        virtual void on_unbind() final {

        }
    };

    static struct debug_output_logger_creator_ {
        debug_output_logger_creator_() {
            auto new_logger = std::make_shared<default_debug_ouput_logger>();
            yjj::get_log().add(TEXT("ouput_debug"), std::static_pointer_cast<yjj::interfaces::loggable>(new_logger));
        }
    } creator;
}
