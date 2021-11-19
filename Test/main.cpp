#include "../ZeeCore/public/core/core.h"
#include "../ZeeCore/public/shape/shape.h"
#include "../ZeeCore/public/shape/to_string.h"
#include "../ZeeCore/public/math/math.h"
#include "../ZeeCore/public/math/to_string.h"
#include <iostream>
using namespace zee;

const bool contain(const shape::rectf& rect1, const shape::rectf& rect2) {
	const auto tmp_rect = rect1.intersect_rect_area(rect2);
	return tmp_rect == rect1 || tmp_rect == rect2;
}

const bool contain(const shape::circlef& circle1, const shape::circlef& circle2) {
	const auto tmp_radius_diff = math::abs(circle1.radius - circle2.radius);
	return circle1.intersect(circle2) && tmp_radius_diff >= circle1.origin.distance(circle2.origin);
}

int main() {
	constexpr zee::math::vec2f v = { 10, 10 };
	constexpr zee::math::impl::vec_impl<2, float> v_parent = { 10, 10 }; 
	
	constexpr zee::shape::recti rt = { 1,2,3,4 };
	constexpr zee::shape::circlei cc = { v, 4.0f };
	std::cout << to_string(cc) << std::endl;

	constexpr zee::shape::impl::rect_base_impl<float> rt2 = { 10, 10, 0 , 0 };
	constexpr auto dd1  = rt.intersect_rect_area_with_normalize(rt2);
	constexpr auto dd2  = dd1 == rt;
	constexpr auto dd3  = rt + v;
	constexpr auto temp2 = v == v;
	//zee::shape::circlef cc = { zee::math::vec2i(0,1), 10.0f };
	zee::math::vec2 temp = { 1,2 };
	std::cout << zee::rand(100) << std::endl;
	std::cout << to_string(temp) << std::endl;
	//std::cout << to_string(rt) << std::endl;
	//std::wcout << to_tstring(cc) << std::endl;
	//std::cout << to_string(rt) << std::endl;
	std::cout << to_string(v) << std::endl;
	std::cout << to_string(v + v) << std::endl;
	//std::cout << to_string(rt + v) << std::endl;
}
