#include "../../public/shape/intersect.h"

namespace zee {
namespace shape {
	//�浹
	//cf) intersect, contain: ���� �Ű������� ������ �Ű������� �浹, ������ ���
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
}
}