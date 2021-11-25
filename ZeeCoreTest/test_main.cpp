#include <iostream>
#include "json/json.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "ini_test.h"
#include "zlog/zlog.h"
#include "shape/rect.h"
#include "shape/to_string.h"

struct temp_struct {
	std::string str1 = "str1";
	int value = 2;
	std::string str2 = "str2";
};

int main() {
	ZEE_LOG(normal, TEXT("test"), TEXT("logtest"));
	std::string temp = "zee/test/test_config";
	auto off = temp.find("::");
	off = temp.find_last_of("/");
	temp.erase(off);
	int d = 0;

	std::wcout << ZEE_GET_CONFIG(zee::test::test_config).config_name() << std::endl;
	
	return 0;
}