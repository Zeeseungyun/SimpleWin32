#include "core/core.h"
#include "shape/shape.h"
#include "shape/boundings.h"
#include "shape/to_string.h"
#include "math/math.h"
#include "math/to_string.h"
#include <iostream>
using namespace zee;

//�簢�� vs �簢�� ����
const shape::collide_type contain(const shape::rectf& rect1, const shape::rectf& rect2) {
	const shape::rectf tmp_rect = rect1.intersect_rect_area(rect2);
	if (tmp_rect == rect1 || tmp_rect == rect2) {
		return shape::collide_type::contain;
	}
	return shape::collide_type::none;
}
//���� vs ���� ����
const shape::collide_type contain(const shape::circlef& circle1, const shape::circlef& circle2) {
	const auto tmp_radius_diff = math::abs(circle1.radius - circle2.radius);
	if (circle1.intersect(circle2)
		&& tmp_radius_diff >= circle1.origin.distance(circle2.origin)) { //�� ���� ������ �� >= �� �� ���� ���� �Ÿ�
		return shape::collide_type::contain;
	}
	return shape::collide_type::none;
}
//���� ���� ���� ������ ���� ���� ��� �� ���
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
//�� vs ��
//������ �ڵ�
/*
bool check(math::vec2f b0, math::vec2f e0, math::vec2f b1, math::vec2f e1) {
	math::vec2f b0_e0 = b0 - e0;
	math::vec2f b1_e1 = b1 - e1;
	math::vec2f b0_b1 = b0 - b1;
	//������ �ƴ� ���
	//xx near xx : ���� ���� �� ����ϴ� �Լ�
	//���Ƿ�. ���� ��������
	//�ϴ� ǥ�� ������ 10�� ���Ѱ� �ʹ� Ÿ��Ʈ�ؼ� ��縮��. �׷��� 10��������.
	if (math::is_near_not_zero(ccw(b0_e0, b1_e1), math::epsilon<float>() * 10)) {	
		math::vec2f e0_b1 = e0 - b1;
		const auto n1_ccw_b0 = b1_e1.ccw(b0_b1);
		const auto n1_ccw_e0 = b1_e1.ccw(e0_b1);
		if ((n1_ccw_b0 <= 0 && n1_ccw_e0 >= 0) || (n1_ccw_e0 <= 0 && n1_ccw_b0 >= 0)) {
			math::vec2f b1_b0 = b1 - b0;
			math::vec2f e1_b0 = e1 - b0;
			const auto n0_ccw_b1 = b0_e0.ccw(b1_b0);
			const auto n0_ccw_e1 = b0_e0.ccw(e1_b0);
			if ((n0_ccw_b1 <= 0 && n0_ccw_e1 >= 0) || (n0_ccw_e1 <= 0 && n0_ccw_b1 >= 0)) {
				return true; //���ڵ���.
			}
		}

	}
	else { //������ ���
		if (math::is_near_zero(b0_b1.ccw(b0_e0), math::epsilon<float>() * 10)) {
			const auto len = (b0 - e0).length_sq();
			return len > (b0 - b1).length_sq() || len > (b0 - e1).length_sq();
		}
	}

	return false;
}*/
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
	//P(t) = (1 - t)P1 + tP2	P(s) = (1 - s)P1 + tP2
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
	out_pt.x = begin1.x + t * static_cast<float>(end1.x - begin1.x);
	out_pt.y = begin1.y + t * (end1.y - begin1.y);
	return shape::collide_type::intersect;
}
//�� vs ��. 2.2. ������
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
//�� vs ����
const shape::collide_type intersect(const shape::circlef circle, const math::vec2f begin, const math::vec2f end) noexcept {
	//������ x = x1 + u(x2 - x1)		���� �߽�(x3, y3)	u = ( (x3 - x1)(x2 - x1) + (y3 - y1)(y2 - y1) ) / (p2 - p1)^2 
	//�Ÿ� <= ������
	//������ u�� 0�� 1 ���̿� �ִ��� �׽�Ʈ
	math::vec2f distVector = circle.origin - begin;
	if (distVector.length() < circle.radius) {
		double distance = circle.radius - distVector.length();
		math::vec2f moveVector = distVector.normalize() * distance;
	}
	math::vec2f out_pt;
}
//�簢�� vs ����
/*
const shape::collide_type intersect(const shape::rectf, const shape::circlef) {

}
const shape::collide_type intersect(const shape::circlef, const shape::rectf) {

}
*/
int main() {
	std::cout << std::boolalpha;
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

	constexpr math::vec2f begin = { 0, 0 };
	constexpr math::vec2f end = { 2, 3 };
	constexpr math::vec2f pt = { 1, 2 };
	//pt.is_normalize();
	constexpr shape::rectf rect = { begin, end };
	constexpr shape::circlef circle = { pt, 3 };
	std::cout << to_string(intersect(begin, end, pt)) << std::endl;
	std::cout << to_string(intersect(rect, pt)) << std::endl;


	//YJJ �׽�Ʈ
	math::vec2f rx = { 0, 0 };
	math::vec2f ry = { 1, 1 };
	shape::rectf rct = { rx, ry };
	constexpr math::vec2f pot = { 1, 2 };
	constexpr shape::circlef c = { pot, 3 };
	std::cout << "���а� �� ����? " << to_string(contain(rt, rt)) << std::endl;
	std::cout << "�簢���� �� ����? " << to_string(contain(c, c)) << std::endl;

	puts("");
	math::vec2f begin1 = { 2, 0 };
	math::vec2f end1 = { 3, 1 };
	math::vec2f begin2 = { 1, 0 };
	math::vec2f end2 = { 1, 1 };
	math::vec2f out_pt = { 10000, 10000 };
	std::cout << "������ / ������pt���� / �����̿� ����" << std::endl;
	std::cout << "�浹 x ���� x? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 0, 0 };
	std::cout << "�浹 x ���� o? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 2, 0 };
	std::cout << "����(2, 0)? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(out_pt) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 3, 0 };
	std::cout << "�߰��� �浹? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(out_pt) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	begin2 = { 2, 0 };
	end2 = { 4, 2 };
	std::cout << "�Ϻ� ����? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;
	end2 = { 3, 1 };
	std::cout << "���� ����? " << to_string(intersect(begin1, end1, begin2, end2)) << ' ';
	std::cout << to_string(intersect(begin1, end1, begin2, end2, out_pt)) << ' ';
	std::cout << to_string(intersect_with_vector(begin1, end1, begin2, end2)) << std::endl;

	puts("");
	rx = { -1, -1 };	ry = { 2, 2 };
	shape::rectf rct2 = { rx, ry };
	begin1 = { 0, 0 };	end1 = { 2, 2 };
	std::cout << "�� vs �簢�� ����? " << to_string(intersect(rct2, begin1, end1)) << std::endl;
	begin1 = { 1, 1 };	end1 = { 3, 3 };
	std::cout << "�� vs �簢�� �浹? " << to_string(intersect(rct2, begin1, end1)) << std::endl;
	begin1 = { 4, 4 };	end1 = { 5, 5 };
	std::cout << "�� vs �簢�� ���浹? " << to_string(intersect(rct2, begin1, end1)) << std::endl;

}