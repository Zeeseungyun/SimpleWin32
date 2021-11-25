#pragma once
#include "zconfig/zconfig.h"
#include "zlog/zlog.h"
#include <math/vec/vec.h>

namespace zee {
namespace test {
	struct test_config {
		math::vec2f temp_vec1 = { 1,2 };
		math::vec2f temp_vec2 = { 2,4 };
		math::vec2f temp_vec3 = { 3,4 };
	};
	
	inline void to_json(json& j, const test_config& c) {

		j = {
			{"temp_vec1", c.temp_vec1 },
			{"temp_vec2", c.temp_vec2 },
			{"temp_vec3", c.temp_vec3 },
		};
	}

	inline void from_json(const json& j, test_config& c) {
		bool is_valid = false;
		auto list = json_helper::safety_get_members(is_valid, j, "temp_vec1", "temp_vec2", "temp_vec3");
		if (is_valid) {
			
		}
	}

}//namespace zee::test
}//namespace zee