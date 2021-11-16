#pragma once
#include "vec_base.h"
#include "vec_common_funcs.h"

void test() {
	constexpr zee::math::vec2 v1 = { -1, 0 };
	constexpr zee::math::vec2 v2 = { 0, 1 };
	constexpr zee::math::vec3 v3 = { -1, 0, 0 };
	constexpr zee::math::vec3 v4 = { 0, 1, 0 };
	constexpr zee::math::vec2f v5 = { 10.0, -2.5 };
	constexpr zee::math::vec3f v6 = { 10.0, -2.5 , 0};
	constexpr auto t1 = v1.dot(v2);
	constexpr auto t2 = v1.ccw(v2);
	constexpr auto t3 = min(v1, v2);
	constexpr auto t4 = max(v1, v2, v3);
	constexpr auto t5 = v3.cross(v4);
	constexpr auto t6 = v3 + v4;
	constexpr auto t7 = v3 * v4;
	constexpr auto t8 = v3 * 2.0;
	constexpr auto t9 = v1 - 2 * dot(v1, v2) * v2;
	constexpr auto t10 = v1.reflect(v2);
	constexpr auto t11 = reflect(v1, v2);
}
