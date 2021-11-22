#include <iostream>
#include "core/core.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "zlog/zlog.h"
#include "interfaces/loggable.h"

using namespace zee;
using namespace std;
#include <thread>
#ifdef TEXT
#undef TEXT
#endif
#include <Windows.h>

struct debug_output_logger : interfaces::loggable {

    virtual void print(const TCHAR* null_terminated_str, size_t length ) {
        OutputDebugString(null_terminated_str);
    }

    virtual void on_bind(zee::log& owner_log) {

    }

    virtual void on_unbind() {

    }
};

struct debug_output_logger_creator {
    debug_output_logger_creator() {
        zee::get_log().add(TEXT("ouput_debug"), std::make_shared<debug_output_logger>());
    }
}temp;

int main() {
    constexpr math::vec2f a0 = { 2,-3 };
    constexpr math::vec2f a1 = { 2,3 };
    constexpr math::vec2f b0 = { 1,0 };
    constexpr math::vec2f b1 = { 3,0 };
    constexpr auto va = a1 - a0;
    constexpr auto vb = b1 - b0;
    constexpr auto a0_b0 = a0 - b0;
    constexpr auto b0_a0 = -a0_b0;
    constexpr auto vb_dot_a0_b0 = vb.dot(a0_b0);
    constexpr auto va_dot_b0_a0 = va.dot(b0_a0);
    constexpr auto va_dot_vb = va.dot(vb);
    constexpr auto denom = va.length_sq() * vb.length_sq() - va_dot_vb * va_dot_vb;
    constexpr auto fa = (va_dot_vb * vb_dot_a0_b0 + va_dot_b0_a0 * vb.length_sq()) / denom;
    constexpr auto fb = (va_dot_vb * math::saturate(fa) + vb_dot_a0_b0) / vb.length_sq();
    constexpr bool is_intersect = fa >= 0 && fa <= 1.0f;

    ZEE_LOG_DETAIL(normal, TEXT("default"), TEXT("test1"));
    std::this_thread::sleep_for(3s);
    ZEE_LOG(normal, TEXT("default"), TEXT("test2"));
    std::this_thread::sleep_for(3s);
    ZEE_LOG(normal, TEXT("default"), TEXT("test3"));
    std::this_thread::sleep_for(3s);
    ZEE_LOG(normal, TEXT("default"), TEXT("test3"));
    std::this_thread::sleep_for(3s);
    ZEE_LOG(normal, TEXT("default"), TEXT("test3"));
    std::this_thread::sleep_for(3s);
    ZEE_LOG(normal, TEXT("default"), TEXT("test3"));
    std::this_thread::sleep_for(3s);
    std::system("pause");
    return 0;
}
