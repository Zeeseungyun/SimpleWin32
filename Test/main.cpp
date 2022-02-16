#include "core/core.h"
#include "shape/shape.h"
#include "shape/boundings.h"
#include "shape/to_string.h"
#include "math/math.h"
#include "math/to_string.h"
#include <iostream>
using namespace yjj;

//충돌
//cf) contain: 왼쪽 값이 오른쪽 값을 포함할 경우
//점 vs 선
const shape::collide_type intersect(const math::vec2f& begin, const math::vec2f& end, const math::vec2f& pt) noexcept {	//접점찾기.
	if (pt.x >= begin.x && pt.x <= end.x
		&& pt.y >= begin.y && pt.y <= end.y) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//점 vs 사각형
const shape::collide_type intersect(const shape::rectf& rect, const math::vec2f& pt) noexcept {
	if ((pt.x >= rect.left && pt.x <= rect.right)
		&& (pt.y <= rect.bottom && pt.y >= rect.top)) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//점 vs 원형
const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& pt) noexcept {
	if (circle.origin.distance_sq(pt) <= circle.radius * circle.radius) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//선 vs 선 1. 외적 벡터 이용 방법
const int comparator(const math::vec2f& l, const math::vec2f& r) {
	//operator< > 함수 필요
	int ret;
	if (l.x == r.x) {
		ret = (l.y <= r.y);
	}
	else {
		ret = (l.x <= r.x);
	}
	return ret;
}
void swap(math::vec2f& l, math::vec2f& r) {
	math::vec2f tmp = l;
	l = r;
	r = tmp;
}
const shape::collide_type intersect_with_vector(math::vec2f begin1, math::vec2f end1, math::vec2f begin2, math::vec2f end2) noexcept {
	//외적 양수 반시계, 외적 음수 시계, 외적 0 평행		==> 좌표계가 y축반전이라 반대인듯
	//(x1, y1) 외적 (x2, y2) == (x1*y2) - (x2*y1).	
	//각 선분의 begin에서 다른 선분의 begin까지, end까지 외적이 하나는 양수, 하나는 음수로 나와야 함. 다른 선의 begin도 반복.

	//두 선분이 한 직선 위에 있거나, 끝점이 겹치는 경우
	if (begin1.ccw(begin2) * begin1.ccw(end2) == 0 && begin2.ccw(begin1) * begin2.ccw(end1) == 0) {
		//공식 참고. 원리 이해는 못함
		if (comparator(end1, begin1)) {
			swap(begin1, end1);
		}
		if (comparator(end2, begin2)) {
			swap(begin2, end2);
		}
		//포함
		if (comparator(begin2, end1) && comparator(begin1, end2)) {
			//완전 포함
			if (begin1.x <= begin2.x && begin1.y <= begin2.y && end1.x >= end2.x && end1.y >= end2.y) {	//operator < > 필요
				return shape::collide_type::contain;	//포함된다고 정의
			}
			return shape::collide_type::intersect;
		}
	}
	//중간점이 겹치는 경우
	else if (begin1.ccw(begin2) * begin1.ccw(end2) <= 0 && begin2.ccw(begin1) * begin2.ccw(end1) <= 0) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//선 vs 선 2.1. 방정식 pt대입 버전 
const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2, math::vec2f& out_pt) noexcept {
	float t;
	float s;
	//직선의 방정식 공식 참고 1
	float under = (end2.y - begin2.y) * (end1.x - begin1.x) - (end2.x - begin2.x) * (end1.y - begin1.y);
	if (under == 0) {
		//완전 내접: 좌변 선이 우변 선을 완전히 포함할 경우
		if (begin1.x <= begin2.x && begin1.y <= begin2.y && end1.x >= end2.x && end1.y >= end2.y) {	//operator < > 필요
			return shape::collide_type::contain;	//포함된다고 정의
		}
		//일부 내접
		else if (begin1.x <= begin2.x && end1.x >= begin1.x) {
			return shape::collide_type::intersect;
		}
		//평행, 만나지 않음
		else {
			return shape::collide_type::none;
		}
	}
	//직선의 방정식 공식 참고 2
	float t_ = (end2.x - begin2.x) * (begin1.y - begin2.y) - (end2.y - begin2.y) * (begin1.x - begin2.x);
	float s_ = (end1.x - begin1.x) * (begin1.y - begin2.y) - (end1.y - begin1.y) * (begin1.x - begin2.x);
	t = t_ / under;
	s = s_ / under;
	//공식에선 t와 s가 0과 1 사이여야 교차 판단.
	if (t < 0.0 || t > 1.0 || s < 0.0 || s > 1.0) {
		return shape::collide_type::none;
	}
	//교차
	out_pt = begin1 + t * (end1 - begin1);
	return shape::collide_type::intersect;
}
//선 vs 선. 2.2. 방정식 pt대입 버전 활용
const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2) noexcept {
	math::vec2f tmp = { -10000, -10000 };	//교차점에 의미 없는 값 선 세팅
	return intersect(begin1, end1, begin2, end2, tmp);
}

//선 vs 사각형 (사각형이 선을 포함하는가)
const shape::collide_type intersect(const shape::rectf rect, const math::vec2f begin, const math::vec2f end) noexcept {
	math::vec2f lt = { rect.left, rect.top };
	math::vec2f lb = { rect.left, rect.bottom };
	math::vec2f rt = { rect.right, rect.top };
	math::vec2f rb = { rect.right, rect.bottom };

	if (intersect(rect, begin) == shape::collide_type::intersect
		&& intersect(rect, end) == shape::collide_type::intersect) {
		return shape::collide_type::contain;
	}
	else if (intersect(begin, end, lt, lb) == shape::collide_type::intersect
		|| intersect(begin, end, lb, rb) == shape::collide_type::intersect
		|| intersect(begin, end, rb, rt) == shape::collide_type::intersect
		|| intersect(begin, end, rt, lt) == shape::collide_type::intersect) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//선 vs 원형 (원형이 선을 포함하는가)
const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept {
	//이하 코드는 구글링 후 조건만 좀 변경함.
	const math::vec2f& origin_to_begin = begin - circle.origin;
	const math::vec2f& origin_to_end = end - circle.origin;
	float c_begin = origin_to_begin.dot(origin_to_begin) - circle.radius * circle.radius;
	float c_end = origin_to_end.dot(origin_to_end) - circle.radius * circle.radius;

	//선분의 시작점부터 구의 중심까지의 거리가 구의 반지름보다 크지 않다면,
	//선분의 시작점이 구의 안에 있으므로 교차함.
	//선분의 시작점도, 끝점도 구 안에 있다면 포함
	if (c_begin <= 0.0f && c_end <= 0.0f) {
		return shape::collide_type::contain;
	}
	else if (c_begin <= 0.0f || c_end <= 0.0f) {
		return shape::collide_type::intersect;
	}

	const math::vec2f dir = end - begin;
	float length = dir.length();
	if (length == 0.0f) {
		return shape::collide_type::none;
	}
	const math::vec2f normalized_dir = dir / length;
	float b_prime = origin_to_begin.dot(normalized_dir);

	//선분의 시작점이 구의 밖에 있고, 구의 중심에서 선분의 시작점을 향하는 벡터와 선분의 방향
	//벡터가 이루는 각이 90도 미만이라면 교차하지 않음
	if (b_prime > 0.0f) {
		return shape::collide_type::none;
	}

	//원래는 b' * b' - a * c를 사용해야 함. 그런데 선분의 방향 벡터가 단위 벡터면,
	//a는 normalized_dir.dot(normalized_dir) = 1
	//이므로, a를 생략 가능.
	float square_root_of_discriminant = sqrt(b_prime * b_prime - c_begin);	//discriminant == 판별식

	float t1 = -b_prime + square_root_of_discriminant;
	if (t1 >= 0.0f && t1 <= length) {
		return shape::collide_type::intersect;
	}
	float t2 = -b_prime + square_root_of_discriminant;
	if (t2 >= 0.0f && t2 <= length) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
const shape::collide_type intersect2(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept {
	float d_cb = circle.origin.distance(begin);	//선분 시작점과 원 중심 사이 거리
	float d_ce = circle.origin.distance(end);
	//포함
	if (d_ce < circle.radius && d_cb < circle.radius) {
		return shape::collide_type::contain;
	}
	//교차: 선분 한 점이 원 안에
	else if ((d_cb < circle.radius && d_ce > circle.radius) || (d_cb > circle.radius && d_ce < circle.radius)) {
		return shape::collide_type::intersect;
	}
	else {
		return shape::collide_type::none;
	}
}
//사각형 vs 사각형
const shape::collide_type intersect(const shape::rectf& rect1, const shape::rectf& rect2) {
	const shape::rectf collision_rect = {
		math::max(rect1.get_left(), rect2.get_left()),
		math::max(rect1.get_top(), rect2.get_top()),
		math::min(rect1.get_right(), rect2.get_right()),
		math::min(rect1.get_bottom(), rect2.get_bottom())
	};
	if (collision_rect.width() != 0 || collision_rect.height() != 0) {	//collision_rect는 충돌되는 범위
		if (collision_rect == rect1 || collision_rect == rect2) {
			return shape::collide_type::contain;
		}
		//contain 아닐 때 조건으로 변경
		else if (!(rect1.get_right() < rect2.get_left()
			|| rect1.get_left() > rect2.get_right()
			|| rect1.get_bottom() < rect2.get_top()
			|| rect1.get_top() > rect2.get_bottom()
			)) {
			return shape::collide_type::intersect;
		}
	}
	return shape::collide_type::none;
}
//원형 vs 원형
const shape::collide_type intersect(const shape::circlef& circle1, const shape::circlef& circle2) {
	const float rad_minus = math::abs(circle1.radius - circle2.radius);
	const float rad_plus = circle1.radius + circle2.radius;
	const float dist_sq = circle1.origin.distance_sq(circle2.origin);
	//7 완전히 같은 원
	if (circle1.origin == circle2.origin && circle1.radius == circle2.radius) {
		return shape::collide_type::contain;
	}
	//1 두 점 접, 2 한 점 외접, 3 한 점 내접
	if (dist_sq >= rad_minus * rad_minus && dist_sq <= rad_plus * rad_plus) {
		return shape::collide_type::intersect;
	}
	//4 만나지 않는 경우
	if (dist_sq > rad_plus * rad_plus) {
		return shape::collide_type::none;
	}
	//5 작은 원이 큰 원 안에 있으면서 만나지 않는 경우, 큰 원이 매개변수 좌측이면 포함, 아니면 none
	if (dist_sq < rad_minus * rad_minus && circle1.radius > circle2.radius) {
		return shape::collide_type::contain;
	}
	//6 두 원의 중심이 같고 반지름이 다른 경우, 큰 원이 매개변수 좌측이면 포함, 아니면 none
	if (dist_sq == 0 && circle1.radius > circle2.radius) {
		return shape::collide_type::contain;
	}
	return shape::collide_type::none;
}
//사각형 vs 원형
const shape::collide_type intersect(const shape::rectf& rect, const shape::circlef& circle) {
	math::vec2f lt = { rect.left, rect.top };
	math::vec2f lb = { rect.left, rect.bottom };
	math::vec2f rt = { rect.right, rect.top };
	math::vec2f rb = { rect.right, rect.bottom };
	//교차: 네 선 중 하나라도 겹침
	if (intersect(circle, lt, lb) == shape::collide_type::intersect
		|| intersect(circle, lb, rb) == shape::collide_type::intersect
		|| intersect(circle, rb, rt) == shape::collide_type::intersect
		|| intersect(circle, rt, lt) == shape::collide_type::intersect) {
		return shape::collide_type::intersect;
	}
	//포함
	else if (circle.origin.x > rect.left && circle.origin.x < rect.right
		&& circle.origin.y > rect.top && circle.origin.y < rect.bottom
		&& circle.radius * 2 <= rect.width() && circle.radius * 2 <= rect.height()) {
		return shape::collide_type::contain;
	}
	return shape::collide_type::none;
}
//원형 vs 사각형
const shape::collide_type intersect(const shape::circlef& circle, const shape::rectf& rect) {
	math::vec2f lt = { rect.left, rect.top };
	math::vec2f lb = { rect.left, rect.bottom };
	math::vec2f rt = { rect.right, rect.top };
	math::vec2f rb = { rect.right, rect.bottom };
	//교차: 네 선 중 하나라도 겹침
	if (intersect(circle, lt, lb) == shape::collide_type::intersect
		|| intersect(circle, lb, rb) == shape::collide_type::intersect
		|| intersect(circle, rb, rt) == shape::collide_type::intersect
		|| intersect(circle, rt, lt) == shape::collide_type::intersect) {
		return shape::collide_type::intersect;
	}
	//포함 : 네 선 모두 포함
	else if (intersect(circle, lt, lb) == shape::collide_type::contain
		&& intersect(circle, lb, rb) == shape::collide_type::contain
		&& intersect(circle, rb, rt) == shape::collide_type::contain
		&& intersect(circle, rt, lt) == shape::collide_type::contain) {
		return shape::collide_type::contain;
	}
	//아래 코드도 작동하지만, 이미 작성한 선 vs 원형 코드 활용
	/*
	else if (rect.left > circle.origin.x - circle.radius
		&& rect.right < circle.origin.x + circle.radius
		&& rect.bottom < circle.origin.x + circle.radius
		&& rect.top > circle.origin.x - circle.radius) {
		return shape::collide_type::contain;
	}
	*/
	return shape::collide_type::none;
}

int main() {
	std::cout << std::boolalpha;
	/*
	constexpr yjj::math::vec2f v = { 10, 10 };
	constexpr yjj::math::impl::vec_impl<2, float> v_parent = { 10, 10 };

	constexpr yjj::shape::recti rt = { 1,2,3,4 };
	constexpr yjj::shape::circlei cc = { v, 4.0f };
	std::cout << to_string(cc) << std::endl;

	constexpr yjj::shape::impl::basic_rect_impl<float> rt2 = { 10, 10, 0 , 0 };
	constexpr auto dd1  = rt.intersect_rect_area_with_normalize(rt2);
	constexpr auto dd2  = dd1 == rt;
	constexpr auto dd3  = rt + v;
	constexpr auto temp2 = v == v;
	//yjj::shape::circlef cc = { yjj::math::vec2i(0,1), 10.0f };
	yjj::math::vec2 temp = { 1,2 };
	std::cout << yjj::rand(100) << std::endl;
	std::cout << to_string(temp) << std::endl;
	//std::cout << to_string(rt) << std::endl;
	//std::wcout << to_tstring(cc) << std::endl;
	//std::cout << to_string(rt) << std::endl;
	std::cout << to_string(v) << std::endl;
	std::cout << to_string(v + v) << std::endl;
	//std::cout << to_string(rt + v) << std::endl;

	constexpr math::vec2f begin = { 0, 0 };
	constexpr math::vec2f end = { 2, 3 };
	constexpr math::vec2f pt = { 1, 2 };
	//pt.is_normalize();
	constexpr shape::rectf rect = { begin, end };
	constexpr shape::circlef circle = { pt, 3 };
	std::cout << to_string(intersect(begin, end, pt)) << std::endl;
	std::cout << to_string(intersect(rect, pt)) << std::endl;
	*/

	//YJJ 테스트
	math::vec2f lt = { 0, 0 };
	math::vec2f rb = { 1, 1 };
	shape::rectf rect = { lt, rb };
	math::vec2f pt = { 1, 2 };
	shape::circlef c = { pt, 3 };
	std::cout << "[포함] 사각형 vs 사각형? " << to_string(intersect(rect, rect)) << std::endl;
	std::cout << "[같음] 원형 vs 원형? " << to_string(intersect(c, c)) << std::endl;

	puts("");
	math::vec2f begin1 = { 2, 0 };
	math::vec2f end1 = { 3, 1 };
	math::vec2f begin2 = { 1, 0 };
	math::vec2f end2 = { 1, 1 };
	math::vec2f out_pt = { 10000, 10000 };
	std::cout << "선 vs 선 : 방정식 / 방정식pt대입 / 벡터이용 순서" << std::endl;
	std::cout << "[충돌 x 평행 x]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 0, 0 };
	std::cout << "[충돌 x 평행 o]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 2, 0 };
	std::cout << "[끝점(2, 0) 충돌]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(out_pt) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 3, 0 };
	std::cout << "[중간점 충돌]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(out_pt) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 2, 0 };
	end2 = { 4, 2 };
	std::cout << "[일부 내접]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	end2 = { 3, 1 };
	std::cout << "[완전 내접]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;

	puts("");
	lt = { -1, -1 };	rb = { 2, 2 };
	shape::rectf rct2 = { lt, rb };
	begin1 = { 4, 4 };	end1 = { 5, 5 };
	std::cout << "[비충돌] 선 vs 사각형? " << to_string(intersect(rct2, begin1, end1)) << std::endl;
	begin1 = { 1, 1 };	end1 = { 3, 3 };
	std::cout << "[충돌] 선 vs 사각형? " << to_string(intersect(rct2, begin1, end1)) << std::endl;
	begin1 = { 0, 0 };	end1 = { 2, 2 };
	std::cout << "[포함] 선 vs 사각형? " << to_string(intersect(rct2, begin1, end1)) << std::endl;

	puts("");
	math::vec2f ori = { 2, 2 };
	shape::circlef c2 = { ori, 2 };
	begin1 = { 6, 6 };	end1 = { 7, 7 };
	std::cout << "[비충돌] 선 vs 원? " << to_string(intersect(c2, begin1, end1)) << std::endl;
	begin1 = { 3, 3 };	end1 = { 7, 7 };
	std::cout << "[시작점or끝점 내접] 선 vs 원 충돌? " << to_string(intersect(c2, begin1, end1)) << std::endl;
	begin1 = { 4, 0 };	end1 = { 4, 4 };
	std::cout << "[중간점 한 점 내접] 선 vs 원 충돌? " << to_string(intersect(c2, begin1, end1)) << std::endl;
	begin1 = { 1, 1 };	end1 = { 2, 2 };
	std::cout << "[포함] 선 vs 원? " << to_string(intersect(c2, begin1, end1)) << std::endl;

	puts("");
	lt = { 0, 0 };	rb = { 4, 4 };
	shape::rectf rct3 = { lt, rb };
	ori = { 4, 2 };	shape::circlef c3 = { ori, 1 };
	std::cout << "[충돌] 사각형 vs 원형? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[충돌] 원형 vs 사각형? " << to_string(intersect(c3, rct3)) << std::endl;
	ori = { 9, 9 };	c3 = { ori, 1 };
	std::cout << "[비충돌] 사각형 vs 원형? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[비충돌] 원형 vs 사각형? " << to_string(intersect(c3, rct3)) << std::endl;
	ori = { 2, 2 }; c3 = { ori, 1 };
	std::cout << "[사각형 내부 원] 사각형 vs 원형 포함? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[사각형 내부 원] 원형 vs 사각형 포함? " << to_string(intersect(c3, rct3)) << std::endl;
	ori = { 2, 2 };	c3 = { ori, 9 };
	std::cout << "[원 내부 사각형] 사각형 vs 원형? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[원 내부 사각형] 원형 vs 사각형? " << to_string(intersect(c3, rct3)) << std::endl;
}

/*
//공식들
constexpr math::vec2f a0 = { 2,-3 };
constexpr math::vec2f a1 = { 2,3 };
constexpr math::vec2f b0 = { 1,0 };
constexpr math::vec2f b1 = { 3,0 };
constexpr auto va = a1 - a0;
constexpr auto vb = b1 - b0;
constexpr auto a0_b0 = a0 - b0;
constexpr auto b0_a0 = -a0_b0;
constexpr auto vb_dot_a0_b0 = vb.dot(a0_b0);
constexpr auto va_dot_b0_a0 = va.dot(b0_a0);
constexpr auto va_dot_vb = va.dot(vb);
constexpr auto denom = va.length_sq() * vb.length_sq() - va_dot_vb * va_dot_vb;
constexpr auto fa = (va_dot_vb * vb_dot_a0_b0 + va_dot_b0_a0 * vb.length_sq()) / denom;
constexpr auto fb = (va_dot_vb * math::saturate(fa) + vb_dot_a0_b0) / vb.length_sq();
constexpr bool is_intersect = fa >= 0 && fa <= 1.0f;

//구현 필요한 함수들
//무한 평면에서 교점을 반환.
bool intersectInfPlane(const Vec3& origin, const Vec3& dir, const Plane& plane, Vec3& out_intersectPos);
bool intersectInfPlane(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec3& out_intersectPos);

bool intersectInfPlaneFront(const Vec3& origin, const Vec3& dir, const Plane& plane, Vec3& out_intersectPos);
bool intersectInfPlaneFront(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec3& out_intersectPos);

bool intersectInfPlaneBack(const Vec3& origin, const Vec3& dir, const Plane& plane, Vec3& out_intersectPos);
bool intersectInfPlaneBack(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec3& out_intersectPos);

//삼각형의 앞, 뒷면 상관없이
bool intersectTriangle(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& dist);
//삼각형의 앞면과 충돌
bool intersectTriangleFront(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& dist);
//삼각형의 뒷면과 충돌
bool intersectTriangleBack(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& dist);

float closetPointFromLineToLine(const Vec3& A0, const Vec3& A1, const Vec3& B0, const Vec3& B1, Vec3& AP, Vec3& BP);
float closetPointFromPointToLine(const Vec3& pt, const Vec3& l0, const Vec3& l1, Vec3& lp);

float distanceSqPtToLine(const Vec3& pt, const Vec3& l0, const Vec3& l1);
float distanceSqLineToLine(const Vec3& A0, const Vec3& A1, const Vec3& B0, const Vec3& B1);

bool pointOnPlaneInsideTriangle(const Vec3& pt, const Vec3& v0, const Vec3& v1, const Vec3& v2);
void intersectPlaneTriangle(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Plane& plane, bool& outSide, bool& inSide);
collide triangleContainedByPlane(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Planes& planes);

void intersectPlaneSphere(const BoundingSphere& sp, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneSphere(const Vec3& center, float radius, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneCorners(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4, const Vec3& p5, const Vec3& p6, const Vec3& p7, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneCorners(const Vec3(&corners)[8], const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneObb(const Yjj::BoundingOrientedBox& obb, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneObb(const Yjj::BoundingOrientedBox& obb, const Vec3& axisX, const Vec3& axisY, const Vec3& axisZ, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneObb(const Vec3& center, const Vec3& extents, const Vec3& axisX, const Vec3& axisY, const Vec3& axisZ, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneAabb(const Yjj::BoundingBox& aabb, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneAabb(const Vec3& center, const Vec3& extents, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneCorners(const Corners& Corners, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneCapsule(const Yjj::BoundingCapsule& capsule, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneCapsule(const Yjj::BoundingCapsule& capsule, const Vec3& bot, const Vec3& top, const Plane& plane, bool& outSide, bool& inSide);
*/