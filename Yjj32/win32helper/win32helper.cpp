#include "win32helper.h"
namespace yjj {
namespace win32helper {
	tstring error_to_tstring(DWORD error_code) noexcept {
		tstring ret;
		HLOCAL hLocal = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr,
			error_code,
			GetLocaleInfo(GetUserDefaultLCID(), LOCALE_ALL, NULL, 0),
			reinterpret_cast<LPTSTR>(&hLocal),
			0,
			NULL
		);

		ret = reinterpret_cast<const LPTSTR>(hLocal);
		LocalFree(hLocal);
		return ret;
	}

	tstring last_error_to_tstring() noexcept {
		return error_to_tstring(GetLastError());
	}

}//namespace yjj::win32helper
}//namespace yjj
