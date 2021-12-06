#include "shape.h"

namespace zee {
namespace shape {
	//충돌
	//cf) intersect, contain: 왼쪽 매개변수가 오른쪽 매개변수를 충돌, 포함할 경우
	//점 vs 선
	const shape::collide_type intersect(const math::vec2f& begin, const math::vec2f& end, const math::vec2f& pt) noexcept;
	//점 vs 사각형
	const shape::collide_type intersect(const shape::rectf& rect, const math::vec2f& pt) noexcept;
	//점 vs 원형
	const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& pt) noexcept;
	//선 vs 선 1. 외적 벡터 이용 방법
	const int comparator(const math::vec2f& l, const math::vec2f& r);
	void swap(math::vec2f& l, math::vec2f& r);
	const shape::collide_type intersect_with_vector(math::vec2f begin1, math::vec2f end1, math::vec2f begin2, math::vec2f end2) noexcept;
	//선 vs 선 2.1. 방정식 pt대입 버전 
	const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2, math::vec2f& out_pt) noexcept;
	//선 vs 선. 2.2. 방정식 pt대입 버전 활용
	const shape::collide_type intersect(const math::vec2f& begin1, const math::vec2f& end1, const math::vec2f& begin2, const math::vec2f& end2) noexcept;

	//선 vs 사각형 (사각형이 선을 포함하는가)
	const shape::collide_type intersect(const shape::rectf rect, const math::vec2f begin, const math::vec2f end) noexcept;
	//선 vs 원형 (원형이 선을 포함하는가)
	const shape::collide_type intersect(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept;
	const shape::collide_type intersect2(const shape::circlef& circle, const math::vec2f& begin, const math::vec2f& end) noexcept;
	//사각형 vs 사각형
	const shape::collide_type intersect(const shape::rectf& rect1, const shape::rectf& rect2);
	//원형 vs 원형
	const shape::collide_type intersect(const shape::circlef& circle1, const shape::circlef& circle2);
	//사각형 vs 원형
	const shape::collide_type intersect(const shape::rectf& rect, const shape::circlef& circle);
	//원형 vs 사각형
	const shape::collide_type intersect(const shape::circlef& circle, const shape::rectf& rect);
}
}