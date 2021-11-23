#pragma once
#include "interfaces/loggable.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

class default_debug_ouput_logger : zee::interfaces::loggable {
    virtual void print(const TCHAR* null_terminated_str, size_t length) {
        OutputDebugString(null_terminated_str);
    }

    virtual void on_bind(zee::log& owner_log) {

    }

    virtual void on_unbind() {

    }
};
