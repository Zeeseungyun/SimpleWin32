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
	using namespace zee;
	std::string temp = "zee/test/test_config";
	auto off = temp.find("::");
	off = temp.find_last_of("/");
	temp.erase(off);
	int d = 0;
	
	config::config<test::test_config>::get().load();
	std::cout << to_string(config::config<test::test_config>::get().config().temp_vec1) << std::endl;
	//config::config<test::test_config>::get().config().temp_vec1.x = 10.0f;
	//config::config<test::test_config>::get().save();
	//config::config<test_config>::get().config()std::wcout << ZEE_GET_CONFIG(zee::test::test_config).config_name() << std::endl;
	
	return 0;
}