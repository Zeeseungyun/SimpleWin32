#pragma once
#include "core/string.h"
#include "windows_nominmax.h"

namespace zee {
namespace win32helper {
	tstring error_to_tstring(DWORD error_code) noexcept;
	tstring last_error_to_tstring() noexcept;

}//namespace zee::win32helper
}//namespace zee
