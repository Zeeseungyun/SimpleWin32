#include "../ZeeCore/public/core/core.h"
#include "../ZeeCore/public/shape/shape.h"
#include "../ZeeCore/public/math/math.h"
#include "../ZeeCore/public/math/to_string.h"
#include <iostream>

int main() {
	constexpr zee::math::vec2f v = { 10, 10 };
	constexpr zee::shape::recti rt = { 1,2,3,4 };
	constexpr zee::shape::impl::rect_base_impl<float> rt2 = { 10, 10, 0 , 0 };
	constexpr auto dd1  = rt.intersect_rect_area_with_normalize(rt2);
	constexpr auto dd2  = dd1 == rt;
	constexpr auto dd3  = rt + v;

	zee::shape::circlef cc = { zee::math::vec2i(0,1), 10.0f };
	zee::math::vec2 temp = { 1,2 };
	std::cout << zee::rand(100) << std::endl;
	std::wcout << to_string(temp) << std::endl;
	std::wcout << to_string(rt) << std::endl;
	std::wcout << to_string(cc) << std::endl;
	std::wcout << to_string(rt) << std::endl;
	std::wcout << to_string(v) << std::endl;
	std::wcout << to_string(v + v) << std::endl;
	std::wcout << to_string(rt + v) << std::endl;
}
