#include "shape.h"

namespace zee {
namespace shape {
	//�浹
	//cf) intersect, contain: ���� �Ű������� ������ �Ű������� �浹, ������ ���
	//�� vs ��
	const shape::collide_type intersect(const math::vec2f& begin, const math::vec2f& end, const math::vec2f& pt) noexcept;
	//�� vs �簢��
	const shape::collide_type intersect(const shape::rectf& rect, const math::vec2f& pt) noexcept;
	//�� vs ����
	const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& pt) noexcept;
	//�� vs �� 1. ���� ���� �̿� ���
	const int comparator(const math::vec2f& l, const math::vec2f& r);
	void swap(math::vec2f& l, math::vec2f& r);
	const shape::collide_type intersect_with_vector(math::vec2f begin1, math::vec2f end1, math::vec2f begin2, math::vec2f end2) noexcept;
	//�� vs �� 2.1. ������ pt���� ���� 
	const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2, math::vec2f& out_pt) noexcept;
	//�� vs ��. 2.2. ������ pt���� ���� Ȱ��
	const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2) noexcept;

	//�� vs �簢�� (�簢���� ���� �����ϴ°�)
	const shape::collide_type intersect(const shape::rectf rect, const math::vec2f begin, const math::vec2f end) noexcept;
	//�� vs ���� (������ ���� �����ϴ°�)
	const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept;
	const shape::collide_type intersect2(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept;
	//�簢�� vs �簢��
	const shape::collide_type intersect(const shape::rectf& rect1, const shape::rectf& rect2);
	//���� vs ����
	const shape::collide_type intersect(const shape::circlef& circle1, const shape::circlef& circle2);
	//�簢�� vs ����
	const shape::collide_type intersect(const shape::rectf& rect, const shape::circlef& circle);
	//���� vs �簢��
	const shape::collide_type intersect(const shape::circlef& circle, const shape::rectf& rect);
}
}