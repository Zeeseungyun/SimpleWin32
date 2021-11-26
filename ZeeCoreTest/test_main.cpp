#include <iostream>
#include "json/json.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
#include "ini_test.h"
#include "zlog/zlog.h"
#include "shape/rect.h"
#include "shape/to_string.h"
//#include <experimental/vector>
struct temp_struct {
	std::string str1 = "str1";
	int value = 2;
	std::string str2 = "str2";
};

template<>
struct nlohmann::adl_serializer<temp_struct> {
	typedef temp_struct data_type;

	static void to_json(json& j, const data_type& c) {
		j =
		{
			{"str1", c.str1},
			{"value", c.value},
			{"str2", c.str2}
		};
	}

	static void from_json(const json& j, data_type& c) {
		using namespace zee;
		bool is_valid = false;
		auto list = zee::json_helper::safety_get_members(is_valid, j, "str1", "value", "str2");
		if (is_valid) {
			c.str1 = to_string(list[0].value());
			c.value = list[1].value();
			c.str2 = list[2].value();
		}
		else {
			c = data_type{};
		}
	}
};

#include <array>
#include <stack>
#include <type_traits>
template<typename T>
void print(T v) {
	std::cout << v;
}

struct pack_param {
	std::string name;

	pack_param func(const pack_param& other) const {
		pack_param ret = { name + other.name };
		std::cout << name << "+" << other.name << std::endl;
		return ret;
	}

	pack_param operator+(const pack_param& other) const {
		return func(other);
	}

	pack_param operator-(pack_param) {
		printf("2");
		return *this;
	}
};

pack_param g_param = { "global" };

constexpr bool my_all(bool b0) {
	return b0;
}

template<typename... Args>
constexpr bool my_all(bool b, Args... args) {
	return b && my_all(args...);
}

int main() {
	constexpr bool b0 = my_all(true);
	constexpr bool b1 = my_all(true, true, true, true, true);
	constexpr bool b2 = my_all(true, true, true, true, false);

	//unary_prefix(pakc_param{}, pakc_param{}, pakc_param{}, pakc_param{});
	std::cout << std::endl;
	pack_param _1{ "1" }, _2{ "2" }, _3{ "3" }, _4{ "4" };
	_1.func(_2.func(_3.func(_4)));
	std::cout << std::endl;
	//unary_right_fold(_1, _2, _3, _4);
	std::cout << std::endl;
	//_1.func(_2).func(_3).func(_4);
	std::cout << std::endl;
	//unary_left_fold(_1, _2, _3, _4);
	std::cout << std::endl;
	return 0;
}