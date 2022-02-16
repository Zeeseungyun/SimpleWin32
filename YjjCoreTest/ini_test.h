#pragma once
#include "zconfig/zconfig.h"
#include "zlog/zlog.h"
#include <math/vec/vec.h>
#include <shape/rect.h>
namespace yjj {
namespace test {
	struct test_config1 {
		math::vec2f temp_vec1 = { 1,2 };
		math::vec2f temp_vec2 = { 2,4 };
		math::vec2f temp_vec3 = { 3,4 };
	};
	
	inline void to_json(json& j, const test_config1& c) noexcept {
		j = 
		{
			{"temp_vec1", c.temp_vec1 },
			{"temp_vec2", c.temp_vec2 },
			{"temp_vec3", c.temp_vec3 },
		};
	}

	inline void from_json(const json& j, test_config1& c) noexcept {
		bool is_valid = false;
		auto list = json_helper::safety_get_members(is_valid, j, "temp_vec1", "temp_vec2", "temp_vec3");
		if (is_valid) {
			c.temp_vec1 = list[0].value();
			c.temp_vec2 = list[1].value();
			c.temp_vec3 = list[2].value();
		} else {
			c = test_config1{};
		}
	}

	struct test_config2 {
		static tstring config_name() noexcept {
			return TEXT("my_config");
		}

		static const TCHAR* config_dir() noexcept {
			return TEXT("my_custom_dir");
		}

		math::vec2f val0 = { 1,2 };
		float val1 = 0.2f;
		shape::recti val2 = { 0, 0, 10, 10 };
		tstring val3 = TEXT("hi");
	};

	inline void to_json(json& j, const test_config2& c) noexcept {
		j = 
		{
			{ "val0", c.val0 },
			{ "val1", c.val1 },
			{ "val2", c.val2 },
			{ "val3", to_string(c.val3) }
		};
	}

	inline void from_json(const json& j, test_config2& c) noexcept {
		bool is_valid = false;
		auto list = json_helper::safety_get_members(is_valid, j, "val0", "val1", "val2", "val3");
		if (is_valid) {
			c.val0 = list[0].value();
			c.val1 = list[1].value();
			c.val2 = list[2].value();
			c.val3 = to_tstring(list[3].value());
		} else {
			c = test_config2{};
		}
	}

}//namespace yjj::test
}//namespace yjj