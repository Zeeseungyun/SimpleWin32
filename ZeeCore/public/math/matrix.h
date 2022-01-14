#pragma once
#include "math.h"
#include <vector>
#include "../zlog/zlog.h"

namespace zee{
namespace math {
	/*행 기준 벡터 (x, y)*/
	//이미 돌아가는거 고치기 애매하니까 
	//클래스를 따로 파셈
	// 머슨 용도로여 동적할당 빼셈
	// 수학연산하는 클래스 중에 동적할당일어나는거는 없음


	///////////////////////////////////////////////////////////////////////////
	//행렬 2x2//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	class matrix2f {
	public:
		//기본
		matrix2f() noexcept;
		explicit matrix2f(const size_t m) noexcept;
		explicit matrix2f(const std::vector<vec2f>& vv) noexcept;
		explicit matrix2f(const matrix2f& m) noexcept;
		//2x2
		explicit matrix2f(const vec2f v0, const vec2f v1) noexcept;
		//3x2
		explicit matrix2f(const vec2f v0, const vec2f v1, const vec2f v2) noexcept;

		virtual ~matrix2f() noexcept = default;

		//행과 열 사이즈 검사
		const bool is_same_size(const std::vector<vec2f>& vv) const;
		const bool is_same_size(const matrix2f& m) const;

		//곱셈에서 앞행렬의 열과 뒷행렬의 행이 같아야 함
		const bool is_same_column_vs_row_size(const matrix2f& m) const;

		//연산자
		matrix2f& operator=(const matrix2f& m);//누가 대입 연산자 오버로딩의 반환값을 const T&로 하래여 a = b = c 이런 거 때메 리턴해야되는거아니여요?
		matrix2f& operator+=(const matrix2f& m);
		matrix2f& operator-=(const matrix2f& m);
		matrix2f& operator*=(const matrix2f& m);

		//연산
		void add(const matrix2f& m);
		void sub(const matrix2f& m);

		void mul(const vec2f& v);
		void mul(const matrix2f& m);

		//변환
		void translation(const float dx, const float dy);
		void translation(const float angle, const float dx, const float dy);
		void scale(const float sx, const float sy);
		void rotation(const float angle, const float dx, const float dy);
		
		//행렬식: 2x2 ad-bc
		const float determinant() const;
		//역행렬: (1, 0; 0, 1) 항등행렬 나오는 식. 1/ad-bc * (d -b; -c a).
		void inverse(const std::vector<vec2f>& vv);
		void inverse(const matrix2f& m);
		//단위행렬: 여부
		const bool is_identity() const;
		//전치행렬: 행과 열을 교환
		void transposed();

		//get, set
		const std::vector<vec2f>& get_m() const;
		const size_t get_row_size() const;
		const size_t get_column_size() const;

		void set_vec(const std::vector<vec2f>& vv);
		void set_m(const matrix2f& m);


	private:
		//? 사이즈가 정해져있는데 동적할당을 왜해요? 적ㄱㄱ거 뺄게요 예전꺼
		std::vector<vec2f> m_;//이거 2x2로 만들고 3x3으로 만들라고
	};

	//circle같은거니 rect같은거나 vec이나
	// 전부다 동적할당이 없져? 네
	// ㅇㅋ 수학연산하는 그런 클래스엔 동적할당을 쓰는게 아님
	// 최적화 관련되서 연관이 너무 많아서




	///////////////////////////////////////////////////////////////////////////
	//행렬 3x3//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	class matrix3f {
	public:
		//기본
		matrix3f() noexcept;
		explicit matrix3f(const size_t m) noexcept;
		explicit matrix3f(const std::vector<vec3f>& vv) noexcept;
		explicit matrix3f(const matrix3f& m) noexcept;
		//3x3
		explicit matrix3f(const vec3f v0, const vec3f v1, const vec3f v2) noexcept;
		//4x3
		explicit matrix3f(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept;

		virtual ~matrix3f() noexcept = default;

		//행과 열 사이즈 검사
		const bool is_same_size(const std::vector<vec3f>& vv) const;
		const bool is_same_size(const matrix3f& m) const;

		//곱셈에서 앞행렬의 열과 뒷행렬의 행이 같아야 함
		const bool is_same_column_vs_row_size(const matrix3f& m) const;

		//연산자
		matrix3f& operator=(const matrix3f& m);
		matrix3f& operator+=(const matrix3f& m);
		matrix3f& operator-=(const matrix3f& m);
		matrix3f& operator*=(const matrix3f& m);

		//연산
		void add(const matrix3f& m);
		void sub(const matrix3f& m);

		void mul(const vec3f& v);
		void mul(const matrix3f& m);

		//변환
		void translation(const float dx, const float dy, const float dz);
		void translation(const float angle, const float dx, const float dy, const float dz);
		void scale(const float sx, const float sy, const float sz);
		void rotation(const float angle, const float dx, const float dy, const float dz);

		//행렬식: 2x2 ad-bc, 3x3 d(ei-fh)-b(di-fg)+c(dh-eg)
		const float determinant() const;
		//단위행렬: 여부
		const bool is_identity() const;
		//전치행렬: 행과 열을 교환
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