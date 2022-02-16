#include <iostream>
#include "core/core.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "zlog/zlog.h"
#include "interfaces/loggable.h"

using namespace yjj;
using namespace std;

int string_header_test() {
    char buffer[5] = {}; 
    int nums[6];
    auto ret = sscanf_s("log_21_11_23_11_38_49", "log_%d_%d_%d_%d_%d_%d", nums,nums+1, nums +2, nums +3, nums +4, nums +5);
    cout << buffer << endl;
    std::string temp = "asdf1";
    yjj::basic_strmanip<char>::sprintf(temp, ", asdf%d", 123123);
    yjj::basic_strmanip<char>::cur_time_to_str(temp, "%Y%m");
    int asdfasdf = 0;
    std::cout << yjj::basic_strmanip<char>::cur_time_to_str("%Y") << std::endl;
    std::cout << temp << std::endl;

    std::wstring temp2 = L"asdf1";
    yjj::basic_strmanip<wchar_t>::sprintf(temp2, L", asdf%d", 123123);
    yjj::basic_strmanip<wchar_t>::cur_time_to_str(temp2, L"%Y%m");
    std::wcout << yjj::basic_strmanip<wchar_t>::cur_time_to_str(L"%Y") <<std::endl;
    std::wcout << temp2 << std::endl;
    {
        wchar_t buf[17];
        setlocale(LC_ALL, "ko-KR");
        size_t out_size = 0;
        errno_t dd = mbstowcs_s(&out_size, buf, 17, "한글이등장한다면", 17);
#pragma warning( disable : 4996)
        cout << strerror(dd) <<endl;
        int d = 0;
    }
    {
        wcout << to_tstring("asdfasdf") <<endl;
        char buf[17];
        size_t out_size = 0;
        errno_t dd = wcstombs_s(&out_size, buf, 17, L"한글이등장한다면", 17);
        cout << strerror(dd) << endl;
        cout << buf << endl;
        int d = 0;
    }
    return 0;
}
