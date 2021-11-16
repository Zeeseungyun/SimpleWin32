#pragma once
#include <string>
#include <tchar.h> //msvc extention header. windows only.

namespace zee {
	typedef std::basic_string<TCHAR> tstring;
	
	tstring current_time_to_string();
	tstring tstring_format(const TCHAR* format, ...);
	void tstring_format(tstring& buffer, const TCHAR* format, ...);
}
