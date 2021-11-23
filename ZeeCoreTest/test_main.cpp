#include <iostream>
#include "json/json.hpp"

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


	return 0;
}