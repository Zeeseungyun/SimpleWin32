#include <iostream>
#include "json/json.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "ini_test.h"
#include "zlog/zlog.h"

int main() {
	ZEE_LOG(normal, TEXT("test"), TEXT("logtest"));

	zee::test::test_config config;
	std::cout << config << std::endl;
	int d12 = 0;
	zee::json temp;
	temp["id"] = 1;
	auto temp22 = temp["id"].get<int>();

	int d = 0;
	std::cout << temp["id"] << std::endl;
	zee::math::vec2f v;
	std::cout << to_string(v * v) << std::endl;

	return 0;
}