#include <iostream>
#include "json/json.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "ini_test.h"
#include "zlog/zlog.h"
#include "shape/rect.h"
#include "shape/to_string.h"
#include <core/core.h>
#include <zconfig/zconfig.h>
#include <util/helper_macros.h>
#include <chrono>
#include <thread>

struct temptemp {
	static const TCHAR* config_dir() {
		return TEXT("");
	}

	static const TCHAR* config_name() {
		return TEXT("sinksink");
	}

	std::string value1 = "asdf";
	float value2 = 2.0f;
};

void to_json(zee::json& j, const temptemp& d) {
	j = {
		{ "value1", d.value1 },
		{ "value2", d.value2 },
	};
}

void from_json(const zee::json& j, temptemp& d) {
	d.value1 = j["value1"];
	d.value2 = j["value2"];
}

#include <cuchar>
void print_mb(const char* ptr)
{
	mbtowc(NULL, 0, 0); // reset the conversion state
	const char* end = ptr + strlen(ptr);
	int ret;
	for (wchar_t wc; (ret = mbtowc(&wc, ptr, end - ptr)) > 0; ptr += ret) {
		wprintf(L"%lc", wc);
	}
	wprintf(L"\n");
}

void print_wide(const wchar_t* wstr)
{
	std::mbstate_t state = std::mbstate_t();
	size_t out_s = {};
	size_t total_size = {};
	auto wstrlen = std::char_traits<wchar_t>::length(wstr);
	::wcsrtombs_s(&out_s,0, 0, &wstr, wstrlen, &state);
	out_s += 1;
	std::vector<char> mbstr(out_s);
	mbstr.resize(512);
	::wcsrtombs_s(&out_s, &mbstr[0], mbstr.size(), &wstr, wstrlen, &state);
	::wcsrtombs_s(&out_s, &mbstr[out_s], mbstr.size(), &wstr + 1, wstrlen, &state);
	
	std::cout << "multibyte string: " << &mbstr[0] << '\n'
		<< "Length, including '\\0': " << mbstr.size() << '\n';
}

int main()
{
	std::wstring str1 = L"한글";
	std::u8string str3;

	setlocale(LC_ALL, "ja_jp.utf8");
	//std::cout << localeconv()->currency_symbol;
	std::locale loc(std::locale(), new std::ctype<char>);
	std::wstring str2 = L"한글";
	int d = 0;
	//print_wide(L"한글");
	return 0;
}
