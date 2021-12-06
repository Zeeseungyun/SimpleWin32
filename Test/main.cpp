#include "core/core.h"
#include "shape/shape.h"
#include "shape/boundings.h"
#include "shape/to_string.h"
#include "math/math.h"
#include "math/to_string.h"
#include <iostream>
using namespace zee;

//�浹
//cf) contain: ���� ���� ������ ���� ������ ���
//�� vs ��
const shape::collide_type intersect(const math::vec2f& begin, const math::vec2f& end, const math::vec2f& pt) noexcept {	//����ã��.
	if (pt.x >= begin.x && pt.x <= end.x
		&& pt.y >= begin.y && pt.y <= end.y) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//�� vs �簢��
const shape::collide_type intersect(const shape::rectf& rect, const math::vec2f& pt) noexcept {
	if ((pt.x >= rect.left && pt.x <= rect.right)
		&& (pt.y <= rect.bottom && pt.y >= rect.top)) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//�� vs ����
const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& pt) noexcept {
	if (circle.origin.distance_sq(pt) <= circle.radius * circle.radius) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//�� vs �� 1. ���� ���� �̿� ���
const int comparator(const math::vec2f& l, const math::vec2f& r) {
	//operator< > �Լ� �ʿ�
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
	//���� ��� �ݽð�, ���� ���� �ð�, ���� 0 ����		==> ��ǥ�谡 y������̶� �ݴ��ε�
	//(x1, y1) ���� (x2, y2) == (x1*y2) - (x2*y1).	
	//�� ������ begin���� �ٸ� ������ begin����, end���� ������ �ϳ��� ���, �ϳ��� ������ ���;� ��. �ٸ� ���� begin�� �ݺ�.

	//�� ������ �� ���� ���� �ְų�, ������ ��ġ�� ���
	if (begin1.ccw(begin2) * begin1.ccw(end2) == 0 && begin2.ccw(begin1) * begin2.ccw(end1) == 0) {
		//���� ����. ���� ���ش� ����
		if (comparator(end1, begin1)) {
			swap(begin1, end1);
		}
		if (comparator(end2, begin2)) {
			swap(begin2, end2);
		}
		//����
		if (comparator(begin2, end1) && comparator(begin1, end2)) {
			//���� ����
			if (begin1.x <= begin2.x && begin1.y <= begin2.y && end1.x >= end2.x && end1.y >= end2.y) {	//operator < > �ʿ�
				return shape::collide_type::contain;	//���Եȴٰ� ����
			}
			return shape::collide_type::intersect;
		}
	}
	//�߰����� ��ġ�� ���
	else if (begin1.ccw(begin2) * begin1.ccw(end2) <= 0 && begin2.ccw(begin1) * begin2.ccw(end1) <= 0) {
		return shape::collide_type::intersect;
	}
	return shape::collide_type::none;
}
//�� vs �� 2.1. ������ pt���� ���� 
const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2, math::vec2f& out_pt) noexcept {
	float t;
	float s;
	//������ ������ ���� ���� 1
	float under = (end2.y - begin2.y) * (end1.x - begin1.x) - (end2.x - begin2.x) * (end1.y - begin1.y);
	if (under == 0) {
		//���� ����: �º� ���� �캯 ���� ������ ������ ���
		if (begin1.x <= begin2.x && begin1.y <= begin2.y && end1.x >= end2.x && end1.y >= end2.y) {	//operator < > �ʿ�
			return shape::collide_type::contain;	//���Եȴٰ� ����
		}
		//�Ϻ� ����
		else if (begin1.x <= begin2.x && end1.x >= begin1.x) {
			return shape::collide_type::intersect;
		}
		//����, ������ ����
		else {
			return shape::collide_type::none;
		}
	}
	//������ ������ ���� ���� 2
	float t_ = (end2.x - begin2.x) * (begin1.y - begin2.y) - (end2.y - begin2.y) * (begin1.x - begin2.x);
	float s_ = (end1.x - begin1.x) * (begin1.y - begin2.y) - (end1.y - begin1.y) * (begin1.x - begin2.x);
	t = t_ / under;
	s = s_ / under;
	//���Ŀ��� t�� s�� 0�� 1 ���̿��� ���� �Ǵ�.
	if (t < 0.0 || t > 1.0 || s < 0.0 || s > 1.0) {
		return shape::collide_type::none;
	}
	//����
	out_pt = begin1 + t * (end1 - begin1);
	return shape::collide_type::intersect;
}
//�� vs ��. 2.2. ������ pt���� ���� Ȱ��
const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2) noexcept {
	math::vec2f tmp = { -10000, -10000 };	//�������� �ǹ� ���� �� �� ����
	return intersect(begin1, end1, begin2, end2, tmp);
}

//�� vs �簢�� (�簢���� ���� �����ϴ°�)
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
//�� vs ���� (������ ���� �����ϴ°�)
const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept {
	//���� �ڵ�� ���۸� �� ���Ǹ� �� ������.
	const math::vec2f& origin_to_begin = begin - circle.origin;
	const math::vec2f& origin_to_end = end - circle.origin;
	float c_begin = origin_to_begin.dot(origin_to_begin) - circle.radius * circle.radius;
	float c_end = origin_to_end.dot(origin_to_end) - circle.radius * circle.radius;

	//������ ���������� ���� �߽ɱ����� �Ÿ��� ���� ���������� ũ�� �ʴٸ�,
	//������ �������� ���� �ȿ� �����Ƿ� ������.
	//������ ��������, ������ �� �ȿ� �ִٸ� ����
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

	//������ �������� ���� �ۿ� �ְ�, ���� �߽ɿ��� ������ �������� ���ϴ� ���Ϳ� ������ ����
	//���Ͱ� �̷�� ���� 90�� �̸��̶�� �������� ����
	if (b_prime > 0.0f) {
		return shape::collide_type::none;
	}

	//������ b' * b' - a * c�� ����ؾ� ��. �׷��� ������ ���� ���Ͱ� ���� ���͸�,
	//a�� normalized_dir.dot(normalized_dir) = 1
	//�̹Ƿ�, a�� ���� ����.
	float square_root_of_discriminant = sqrt(b_prime * b_prime - c_begin);	//discriminant == �Ǻ���

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
	float d_cb = circle.origin.distance(begin);	//���� �������� �� �߽� ���� �Ÿ�
	float d_ce = circle.origin.distance(end);
	//����
	if (d_ce < circle.radius && d_cb < circle.radius) {
		return shape::collide_type::contain;
	}
	//����: ���� �� ���� �� �ȿ�
	else if ((d_cb < circle.radius && d_ce > circle.radius) || (d_cb > circle.radius && d_ce < circle.radius)) {
		return shape::collide_type::intersect;
	}
	else {
		return shape::collide_type::none;
	}
}
//�簢�� vs �簢��
const shape::collide_type intersect(const shape::rectf& rect1, const shape::rectf& rect2) {
	const shape::rectf collision_rect = {
		math::max(rect1.get_left(), rect2.get_left()),
		math::max(rect1.get_top(), rect2.get_top()),
		math::min(rect1.get_right(), rect2.get_right()),
		math::min(rect1.get_bottom(), rect2.get_bottom())
	};
	if (collision_rect.width() != 0 || collision_rect.height() != 0) {	//collision_rect�� �浹�Ǵ� ����
		if (collision_rect == rect1 || collision_rect == rect2) {
			return shape::collide_type::contain;
		}
		//contain �ƴ� �� �������� ����
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
//���� vs ����
const shape::collide_type intersect(const shape::circlef& circle1, const shape::circlef& circle2) {
	const float rad_minus = math::abs(circle1.radius - circle2.radius);
	const float rad_plus = circle1.radius + circle2.radius;
	const float dist_sq = circle1.origin.distance_sq(circle2.origin);
	//7 ������ ���� ��
	if (circle1.origin == circle2.origin && circle1.radius == circle2.radius) {
		return shape::collide_type::contain;
	}
	//1 �� �� ��, 2 �� �� ����, 3 �� �� ����
	if (dist_sq >= rad_minus * rad_minus && dist_sq <= rad_plus * rad_plus) {
		return shape::collide_type::intersect;
	}
	//4 ������ �ʴ� ���
	if (dist_sq > rad_plus * rad_plus) {
		return shape::collide_type::none;
	}
	//5 ���� ���� ū �� �ȿ� �����鼭 ������ �ʴ� ���, ū ���� �Ű����� �����̸� ����, �ƴϸ� none
	if (dist_sq < rad_minus * rad_minus && circle1.radius > circle2.radius) {
		return shape::collide_type::contain;
	}
	//6 �� ���� �߽��� ���� �������� �ٸ� ���, ū ���� �Ű����� �����̸� ����, �ƴϸ� none
	if (dist_sq == 0 && circle1.radius > circle2.radius) {
		return shape::collide_type::contain;
	}
	return shape::collide_type::none;
}
//�簢�� vs ����
const shape::collide_type intersect(const shape::rectf& rect, const shape::circlef& circle) {
	math::vec2f lt = { rect.left, rect.top };
	math::vec2f lb = { rect.left, rect.bottom };
	math::vec2f rt = { rect.right, rect.top };
	math::vec2f rb = { rect.right, rect.bottom };
	//����: �� �� �� �ϳ��� ��ħ
	if (intersect(circle, lt, lb) == shape::collide_type::intersect
		|| intersect(circle, lb, rb) == shape::collide_type::intersect
		|| intersect(circle, rb, rt) == shape::collide_type::intersect
		|| intersect(circle, rt, lt) == shape::collide_type::intersect) {
		return shape::collide_type::intersect;
	}
	//����
	else if (circle.origin.x > rect.left && circle.origin.x < rect.right
		&& circle.origin.y > rect.top && circle.origin.y < rect.bottom
		&& circle.radius * 2 <= rect.width() && circle.radius * 2 <= rect.height()) {
		return shape::collide_type::contain;
	}
	return shape::collide_type::none;
}
//���� vs �簢��
const shape::collide_type intersect(const shape::circlef& circle, const shape::rectf& rect) {
	math::vec2f lt = { rect.left, rect.top };
	math::vec2f lb = { rect.left, rect.bottom };
	math::vec2f rt = { rect.right, rect.top };
	math::vec2f rb = { rect.right, rect.bottom };
	//����: �� �� �� �ϳ��� ��ħ
	if (intersect(circle, lt, lb) == shape::collide_type::intersect
		|| intersect(circle, lb, rb) == shape::collide_type::intersect
		|| intersect(circle, rb, rt) == shape::collide_type::intersect
		|| intersect(circle, rt, lt) == shape::collide_type::intersect) {
		return shape::collide_type::intersect;
	}
	//���� : �� �� ��� ����
	else if (intersect(circle, lt, lb) == shape::collide_type::contain
		&& intersect(circle, lb, rb) == shape::collide_type::contain
		&& intersect(circle, rb, rt) == shape::collide_type::contain
		&& intersect(circle, rt, lt) == shape::collide_type::contain) {
		return shape::collide_type::contain;
	}
	//�Ʒ� �ڵ嵵 �۵�������, �̹� �ۼ��� �� vs ���� �ڵ� Ȱ��
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
	constexpr zee::math::vec2f v = { 10, 10 };
	constexpr zee::math::impl::vec_impl<2, float> v_parent = { 10, 10 };

	constexpr zee::shape::recti rt = { 1,2,3,4 };
	constexpr zee::shape::circlei cc = { v, 4.0f };
	std::cout << to_string(cc) << std::endl;

	constexpr zee::shape::impl::basic_rect_impl<float> rt2 = { 10, 10, 0 , 0 };
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

	constexpr math::vec2f begin = { 0, 0 };
	constexpr math::vec2f end = { 2, 3 };
	constexpr math::vec2f pt = { 1, 2 };
	//pt.is_normalize();
	constexpr shape::rectf rect = { begin, end };
	constexpr shape::circlef circle = { pt, 3 };
	std::cout << to_string(intersect(begin, end, pt)) << std::endl;
	std::cout << to_string(intersect(rect, pt)) << std::endl;
	*/

	//YJJ �׽�Ʈ
	math::vec2f lt = { 0, 0 };
	math::vec2f rb = { 1, 1 };
	shape::rectf rect = { lt, rb };
	math::vec2f pt = { 1, 2 };
	shape::circlef c = { pt, 3 };
	std::cout << "[����] �簢�� vs �簢��? " << to_string(intersect(rect, rect)) << std::endl;
	std::cout << "[����] ���� vs ����? " << to_string(intersect(c, c)) << std::endl;

	puts("");
	math::vec2f begin1 = { 2, 0 };
	math::vec2f end1 = { 3, 1 };
	math::vec2f begin2 = { 1, 0 };
	math::vec2f end2 = { 1, 1 };
	math::vec2f out_pt = { 10000, 10000 };
	std::cout << "�� vs �� : ������ / ������pt���� / �����̿� ����" << std::endl;
	std::cout << "[�浹 x ���� x]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 0, 0 };
	std::cout << "[�浹 x ���� o]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 2, 0 };
	std::cout << "[����(2, 0) �浹]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(out_pt) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 3, 0 };
	std::cout << "[�߰��� �浹]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(out_pt) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 2, 0 };
	end2 = { 4, 2 };
	std::cout << "[�Ϻ� ����]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	end2 = { 3, 1 };
	std::cout << "[���� ����]? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;

	puts("");
	lt = { -1, -1 };	rb = { 2, 2 };
	shape::rectf rct2 = { lt, rb };
	begin1 = { 4, 4 };	end1 = { 5, 5 };
	std::cout << "[���浹] �� vs �簢��? " << to_string(intersect(rct2, begin1, end1)) << std::endl;
	begin1 = { 1, 1 };	end1 = { 3, 3 };
	std::cout << "[�浹] �� vs �簢��? " << to_string(intersect(rct2, begin1, end1)) << std::endl;
	begin1 = { 0, 0 };	end1 = { 2, 2 };
	std::cout << "[����] �� vs �簢��? " << to_string(intersect(rct2, begin1, end1)) << std::endl;

	puts("");
	math::vec2f ori = { 2, 2 };
	shape::circlef c2 = { ori, 2 };
	begin1 = { 6, 6 };	end1 = { 7, 7 };
	std::cout << "[���浹] �� vs ��? " << to_string(intersect(c2, begin1, end1)) << std::endl;
	begin1 = { 3, 3 };	end1 = { 7, 7 };
	std::cout << "[������or���� ����] �� vs �� �浹? " << to_string(intersect(c2, begin1, end1)) << std::endl;
	begin1 = { 4, 0 };	end1 = { 4, 4 };
	std::cout << "[�߰��� �� �� ����] �� vs �� �浹? " << to_string(intersect(c2, begin1, end1)) << std::endl;
	begin1 = { 1, 1 };	end1 = { 2, 2 };
	std::cout << "[����] �� vs ��? " << to_string(intersect(c2, begin1, end1)) << std::endl;

	puts("");
	lt = { 0, 0 };	rb = { 4, 4 };
	shape::rectf rct3 = { lt, rb };
	ori = { 4, 2 };	shape::circlef c3 = { ori, 1 };
	std::cout << "[�浹] �簢�� vs ����? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[�浹] ���� vs �簢��? " << to_string(intersect(c3, rct3)) << std::endl;
	ori = { 9, 9 };	c3 = { ori, 1 };
	std::cout << "[���浹] �簢�� vs ����? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[���浹] ���� vs �簢��? " << to_string(intersect(c3, rct3)) << std::endl;
	ori = { 2, 2 }; c3 = { ori, 1 };
	std::cout << "[�簢�� ���� ��] �簢�� vs ���� ����? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[�簢�� ���� ��] ���� vs �簢�� ����? " << to_string(intersect(c3, rct3)) << std::endl;
	ori = { 2, 2 };	c3 = { ori, 9 };
	std::cout << "[�� ���� �簢��] �簢�� vs ����? " << to_string(intersect(rct3, c3)) << std::endl;
	std::cout << "[�� ���� �簢��] ���� vs �簢��? " << to_string(intersect(c3, rct3)) << std::endl;
}

/*
//���ĵ�
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

//���� �ʿ��� �Լ���
//���� ��鿡�� ������ ��ȯ.
bool intersectInfPlane(const Vec3& origin, const Vec3& dir, const Plane& plane, Vec3& out_intersectPos);
bool intersectInfPlane(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec3& out_intersectPos);

bool intersectInfPlaneFront(const Vec3& origin, const Vec3& dir, const Plane& plane, Vec3& out_intersectPos);
bool intersectInfPlaneFront(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec3& out_intersectPos);

bool intersectInfPlaneBack(const Vec3& origin, const Vec3& dir, const Plane& plane, Vec3& out_intersectPos);
bool intersectInfPlaneBack(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec3& out_intersectPos);

//�ﰢ���� ��, �޸� �������
bool intersectTriangle(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& dist);
//�ﰢ���� �ո�� �浹
bool intersectTriangleFront(const Vec3& origin, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& dist);
//�ﰢ���� �޸�� �浹
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

void intersectPlaneObb(const Zee::BoundingOrientedBox& obb, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneObb(const Zee::BoundingOrientedBox& obb, const Vec3& axisX, const Vec3& axisY, const Vec3& axisZ, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneObb(const Vec3& center, const Vec3& extents, const Vec3& axisX, const Vec3& axisY, const Vec3& axisZ, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneAabb(const Zee::BoundingBox& aabb, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneAabb(const Vec3& center, const Vec3& extents, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneCorners(const Corners& Corners, const Plane& plane, bool& outSide, bool& inSide);

void intersectPlaneCapsule(const Zee::BoundingCapsule& capsule, const Plane& plane, bool& outSide, bool& inSide);
void intersectPlaneCapsule(const Zee::BoundingCapsule& capsule, const Vec3& bot, const Vec3& top, const Plane& plane, bool& outSide, bool& inSide);
*/