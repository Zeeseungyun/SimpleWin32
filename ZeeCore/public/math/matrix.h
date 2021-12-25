#pragma once
#include "math.h"
#include <vector>
#include "../zlog/zlog.h"

namespace zee{
namespace math {
	class matrix {
	public:
		static constexpr float npos = -1.0f;
		//�⺻
		matrix() noexcept;
		explicit matrix(const std::vector<vec2f>& vv) noexcept;
		explicit matrix(const std::vector<vec3f>& vv) noexcept;
		explicit matrix(const matrix& m) noexcept;
		//2x2
		explicit matrix(const vec2f v0, const vec2f v1) noexcept;
		//3x2
		explicit matrix(const vec2f v0, const vec2f v1, const vec2f v2) noexcept;
		//3x3
		explicit matrix(const vec3f v0, const vec3f v1, const vec3f v2) noexcept;
		//4x3
		explicit matrix(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept;
		virtual ~matrix() noexcept;

		//��� �� ������ �˻�
		const bool is_same_size(const std::vector<vec2f>& vv) const;
		const bool is_same_size(const std::vector<vec3f>& vv) const;
		const bool is_same_size(const matrix& m) const;

		//�������� ������� ���� ������� ���� ���ƾ� ��
		const bool is_same_column_vs_row_size(const matrix& m) const;

		//������
		const matrix& operator=(const matrix& m);
		const matrix& operator+=(const matrix& m);
		const matrix& operator-=(const matrix& m);
		const matrix& operator*=(const matrix& m);

		//����
		void add(const matrix& m);
		void sub(const matrix& m);

		void mul(const vec2f& v);
		void mul(const vec3f& v);
		void mul(const matrix& m);

		
		//��Ľ�: 2x2 ad-bc, 3x3 d(ei-fh)-b(di-fg)+c(dh-eg)
		const float determinant();
		//�����: (1, 0; 0, 1) �׵���� ������ ��. 1/ad-bc * (d -b; -c a). ������ �α� ���� �ʱ� ��� ��ȯ.
		const matrix& inverse(const std::vector<vec2f>& vv);
		const matrix& inverse(const matrix& m);
		//�׵���� ����
		const bool is_identity();

		//get, set
		const std::vector<std::vector<float>>& get_mf() const;
		const size_t get_row_size() const;
		const size_t get_column_size() const;

		void set_m2f(const std::vector<vec2f>& vv);
		void set_m3f(const std::vector<vec3f>& vv);
		void set_mf(const matrix& m);


	private:
		std::vector<std::vector<float>> mf_;
		size_t row;
		size_t column;
	};
}//math
}//zee