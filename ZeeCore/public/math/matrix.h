#pragma once
#include "math.h"
#include <vector>
#include "../zlog/zlog.h"

namespace zee{
namespace math {
	class matrix {
	public:
		//기본
		matrix() noexcept;
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

		//행과 열 사이즈 검사
		const bool is_same_size(const std::vector<vec2f>& vv) const;
		const bool is_same_size(const std::vector<vec3f>& vv) const;
		const bool is_same_size(const matrix& m) const;

		//곱셈에서 앞행렬의 열과 뒷행렬의 행이 같아야 함
		const bool is_same_column_vs_row_size(const std::vector<vec2f>& vv) const;
		const bool is_same_column_vs_row_size(const std::vector<vec3f>& vv) const;
		const bool is_same_column_vs_row_size(const matrix& m) const;

		//연산자
		const matrix& operator=(const matrix& m);
		const matrix& operator+=(const matrix& m);
		const matrix& operator-=(const matrix& m);
		const matrix& operator*=(const matrix& m);

		//연산
		vec2f& add(const vec2f& m);
		vec3f& add(const vec3f& m);
		matrix& add(const matrix& m);

		vec2f& sub(const vec2f& m);
		vec3f& sub(const vec3f& m);
		matrix& sub(const matrix& m);

		vec2f& mul(const vec2f& m);
		vec3f& mul(const vec3f& m);
		matrix& mul(const matrix& m);


		//행렬식: 2x2는 넓이, 3x3은 부피
		void determinant(const vec2f& v);
		//역행렬 (1, 0; 0, 1) 단위행렬 만드는 행렬
		void inverse(const vec2f& v);
		//전치행렬
		void transpose(const vec2f& v);
		//?
		void is_identity(const vec2f& v);

		//get, set
		const std::vector<std::vector<float>>& get_mf() const;
		const size_t& get_row_size() const;
		const size_t& get_column_size() const;

		void set_mf(const std::vector<vec2f>& vv);
		void set_mf(const std::vector<vec3f>& vv);
		void set_mf(const matrix& m);



	private:
		std::vector<std::vector<float>> mf_;
		size_t row;
		size_t column;
	};
}//math
}//zee