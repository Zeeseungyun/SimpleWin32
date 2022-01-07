#pragma once
#include "math.h"
#include <vector>
#include "../zlog/zlog.h"

namespace zee{
namespace math {
	/*�� ���� ���� (x, y)*/

	///////////////////////////////////////////////////////////////////////////
	//��� 2x2//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	class matrix2f {
	public:
		//�⺻
		matrix2f() noexcept;
		explicit matrix2f(const size_t m, const size_t n) noexcept;
		explicit matrix2f(const std::vector<vec2f>& vv) noexcept;
		explicit matrix2f(const matrix2f& m) noexcept;
		//2x2
		explicit matrix2f(const vec2f v0, const vec2f v1) noexcept;
		//3x2
		explicit matrix2f(const vec2f v0, const vec2f v1, const vec2f v2) noexcept;

		virtual ~matrix2f() noexcept = default;

		//��� �� ������ �˻�
		const bool is_same_size(const std::vector<vec2f>& vv) const;
		const bool is_same_size(const matrix2f& m) const;

		//�������� ������� ���� ������� ���� ���ƾ� ��
		const bool is_same_column_vs_row_size(const matrix2f& m) const;

		//������
		const matrix2f& operator=(const matrix2f& m);
		const matrix2f& operator+=(const matrix2f& m);
		const matrix2f& operator-=(const matrix2f& m);
		const matrix2f& operator*=(const matrix2f& m);

		//����
		void add(const matrix2f& m);
		void sub(const matrix2f& m);

		void mul(const vec2f& v);
		void mul(const matrix2f& m);

		//��ȯ
		void translation(const float dx, const float dy);
		void translation(const float angle, const float dx, const float dy);
		void scale(const float sx, const float sy);
		void rotation(const float angle, const float dx, const float dy);
		
		//��Ľ�: 2x2 ad-bc
		const float determinant();
		//�����: (1, 0; 0, 1) �׵���� ������ ��. 1/ad-bc * (d -b; -c a).
		void inverse(const std::vector<vec2f>& vv);
		void inverse(const matrix2f& m);
		//�������: ����
		const bool is_identity();
		//��ġ���: ��� ���� ��ȯ
		void transposed();

		//get, set
		const std::vector<vec2f>& get_m() const;
		const size_t get_row_size() const;
		const size_t get_column_size() const;

		void set_vec(const std::vector<vec2f>& vv);
		void set_m(const matrix2f& m);


	private:
		std::vector<vec2f> m_;
	};





	///////////////////////////////////////////////////////////////////////////
	//��� 3x3//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	class matrix3f {
	public:
		//�⺻
		matrix3f() noexcept;
		explicit matrix3f(const size_t m, const size_t n) noexcept;
		explicit matrix3f(const std::vector<vec3f>& vv) noexcept;
		explicit matrix3f(const matrix3f& m) noexcept;
		//3x3
		explicit matrix3f(const vec3f v0, const vec3f v1, const vec3f v2) noexcept;
		//4x3
		explicit matrix3f(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept;

		virtual ~matrix3f() noexcept = default;

		//��� �� ������ �˻�
		const bool is_same_size(const std::vector<vec3f>& vv) const;
		const bool is_same_size(const matrix3f& m) const;

		//�������� ������� ���� ������� ���� ���ƾ� ��
		const bool is_same_column_vs_row_size(const matrix3f& m) const;

		//������
		const matrix3f& operator=(const matrix3f& m);
		const matrix3f& operator+=(const matrix3f& m);
		const matrix3f& operator-=(const matrix3f& m);
		const matrix3f& operator*=(const matrix3f& m);

		//����
		void add(const matrix3f& m);
		void sub(const matrix3f& m);

		void mul(const vec3f& v);
		void mul(const matrix3f& m);

		//��ȯ
		void translation(const float dx, const float dy, const float dz);
		void translation(const float angle, const float dx, const float dy, const float dz);
		void scale(const float sx, const float sy, const float sz);
		void rotation(const float angle, const float dx, const float dy, const float dz);

		//��Ľ�: 2x2 ad-bc, 3x3 d(ei-fh)-b(di-fg)+c(dh-eg)
		const float determinant();
		//�������: ����
		const bool is_identity();
		//��ġ���: ��� ���� ��ȯ
		void transposed();

		//get, set
		const std::vector<vec3f>& get_m() const;
		const size_t get_row_size() const;
		const size_t get_column_size() const;

		void set_vec(const std::vector<vec3f>& vv);
		void set_m(const matrix3f& m);


	private:
		std::vector<vec3f> m_;
	};
}//math
}//zee