#include <iostream>
#include "json/json.hpp"
#include "math/vec/vec.h"
#include "math/to_string.h"

int main() {
	char temp2[] = R"(
{
	"id": "asdf",
	"pw" : "asdflkjasdf"
}
	)";
	nlohmann::json temp = nlohmann::json::parse(std::begin(temp2), std::end(temp2));
	//int d = 0;
	std::cout << temp["id"] << std::endl;
	zee::math::vec2f v;
	std::cout << to_string(v * v) << std::endl;

	return 0;
}