#include "../../public/math/matrix.h"

namespace zee {
namespace math {
	//기본
	matrix::matrix() noexcept : mf_{ 0 } {
	}
	explicit matrix::matrix(const matrix& m) noexcept {
		add(m);
	}
	//2x2
	explicit matrix::matrix(const vec2f v0, const vec2f v1) noexcept {
		set_mf({ v0, v1 });
	}
	//3x2
	explicit matrix::matrix(const vec2f v0, const vec2f v1, const vec2f v2) noexcept {
		set_mf({ v0, v1, v2 });
	}
	//3x3
	explicit matrix::matrix(const vec3f v0, const vec3f v1, const vec3f v2) noexcept {
		set_mf({ v0, v1, v2 });
	}
	//4x3
	explicit matrix::matrix(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept {
		set_mf({ v0, v1, v2, v3 });
	}
	matrix::~matrix() noexcept {
	}

	//행과 열 사이즈 검사
	const bool matrix::is_same_size(const std::vector<vec2f>& vv) const {
		if (row == vv.size() && column == vv[0].size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("row size != matrix member row size"));
		}
	}
	const bool matrix::is_same_size(const std::vector<vec3f>& vv) const {
		if (row == vv.size() && column == vv[0].size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("row size != matrix member row size"));
		}
	}
	const bool matrix::is_same_size(const matrix& m) const {
		if (row == m.get_row_size() && column == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("row size != matrix member row size"));
		}
	}

	//곱셈에서 앞행렬의 열과 뒷행렬의 행이 같아야 함
	const bool matrix::is_same_column_vs_row_size(const std::vector<vec2f>& vv) const {
		if (column == vv.size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("first matrix column size != second matrix row size"));
		}
	}
	const bool matrix::is_same_column_vs_row_size(const std::vector<vec3f>& vv) const {
		if (column == vv.size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("first matrix column size != second matrix row size"));
		}
	}
	const bool matrix::is_same_column_vs_row_size(const matrix& m) const {
		if (row == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("size is different from matrix member"));
		}
	}

	//연산자
	const matrix& matrix::operator=(const matrix& m) {
		if (is_same_size(m)) {
			set_mf(m);
		}
	}
	const matrix& matrix::operator+=(const matrix& m) {
		if (is_same_size(m)) {
			add(m);
		}
	}
	const matrix& matrix::operator-=(const matrix& m) {
		if (is_same_size(m)) {
			sub(m);
		}
	}
	const matrix& matrix::operator*=(const matrix& m) {
		if (is_same_size(m)) {
			mul(m);
		}
	}

	//사칙연산
	vec2f& add(const vec2f& m) {

	}
	vec3f& add(const vec3f& m) {

	}
	matrix& matrix::add(const matrix& m) {
		if (is_same_size(m)) {
			//2x2, 3x2
			if (column == 2) {
				mf_[0][0] += m.get_mf()[0][0];	mf_[0][1] += m.get_mf()[0][1];
				mf_[1][0] += m.get_mf()[1][0];	mf_[1][1] += m.get_mf()[1][1];
			}
			//3x2 추가
			if (row == 3 && column == 2) {
				mf_[2][0] += m.get_mf()[2][0];	mf_[2][1] += m.get_mf()[2][1];
			}
			//3x3, 4x3
			if (column == 3) {
				mf_[0][0] += m.get_mf()[0][0];	mf_[0][1] += m.get_mf()[0][1];	mf_[0][2] += m.get_mf()[0][2];
				mf_[1][0] += m.get_mf()[1][0];	mf_[1][1] += m.get_mf()[1][1];	mf_[1][2] += m.get_mf()[1][2];
				mf_[2][0] += m.get_mf()[2][0];	mf_[2][1] += m.get_mf()[2][1];	mf_[2][2] += m.get_mf()[2][2];
			}
			//4x3 추가
			if (row == 4 && column == 3) {
				mf_[3][0] += m.get_mf()[3][0];	mf_[3][1] += m.get_mf()[3][1];	mf_[3][2] += m.get_mf()[3][2];
			}
		}
	}

	vec2f& sub(const vec2f& m) {

	}
	vec3f& sub(const vec3f& m) {

	}
	matrix& matrix::sub(const matrix& m) {
		if (is_same_size(m)) {
			//2x2, 3x2
			if (column == 2) {
				mf_[0][0] -= m.get_mf()[0][0];	mf_[0][1] -= m.get_mf()[0][1];
				mf_[1][0] -= m.get_mf()[1][0];	mf_[1][1] -= m.get_mf()[1][1];
			}
			//3x2 추가
			if (row == 3 && column == 2) {
				mf_[2][0] -= m.get_mf()[2][0];	mf_[2][1] -= m.get_mf()[2][1];
			}
			//3x3, 4x3
			if (column == 3) {
				mf_[0][0] -= m.get_mf()[0][0];	mf_[0][1] -= m.get_mf()[0][1];	mf_[0][2] -= m.get_mf()[0][2];
				mf_[1][0] -= m.get_mf()[1][0];	mf_[1][1] -= m.get_mf()[1][1];	mf_[1][2] -= m.get_mf()[1][2];
				mf_[2][0] -= m.get_mf()[2][0];	mf_[2][1] -= m.get_mf()[2][1];	mf_[2][2] -= m.get_mf()[2][2];
			}
			//4x3 추가
			if (row == 4 && column == 3) {
				mf_[3][0] -= m.get_mf()[3][0];	mf_[3][1] -= m.get_mf()[3][1];	mf_[3][2] -= m.get_mf()[3][2];
			}
		}
	}

	vec2f& mul(const vec2f& m) {

	}
	vec3f& mul(const vec3f& m) {

	}
	matrix& matrix::mul(const matrix& m) {
		if (is_same_column_vs_row_size(m)) {
			//2x2 * 2x2
			if (row == 2 && column == 2 && m.get_row_size() == 2 && m.get_column_size() == 2) {
				matrix ret;
				ret.set_mf({ { 0, 0 },{ 0, 0 } });
				ret.set_mf(
					{
					{ m.get_mf()[0][0] * mf_[0][0]
					+ m.get_mf()[0][1] * mf_[1][0]
								+ 1.0f * mf_[2][0]
					,
					m.get_mf()[0][0] * mf_[0][1]
					+ m.get_mf()[0][1] * mf_[1][1]
								+ 1.0f * mf_[2][1] }
					,
					{ m.get_mf()[1][0] * mf_[0][0]
					+ m.get_mf()[1][1] * mf_[1][0]
								+ 1.0f * mf_[2][0]
					,
					m.get_mf()[1][0] * mf_[0][1]
					+ m.get_mf()[1][1] * mf_[1][1]
								+ 1.0f * mf_[2][1] }
					}
				);
				return ret;
			}
		}
	}

	//get, set
	const std::vector<std::vector<float>>& matrix::get_mf() const {
		return mf_;
	}
	const size_t& matrix::get_row_size() const {
		return mf_.size();
	}
	const size_t& matrix::get_column_size() const {
		return mf_[0].size();
	}
	void matrix::set_mf(const std::vector<vec2f>& vv) {
		//2x2
		if (vv.size() == 2 && vv[0].size() == 2) {
			mf_.resize(2, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = vv[0].x;	mf_[0][1] = vv[0].y;
			mf_[1][0] = vv[0].x;	mf_[1][1] = vv[0].y;
		}
		//3x2
		else if (vv.size() == 3 && vv[0].size() == 2) {
			mf_.resize(3, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = vv[0].x;	mf_[0][1] = vv[0].y;
			mf_[1][0] = vv[0].x;	mf_[1][1] = vv[0].y;
			mf_[2][0] = 1.0f;	mf_[2][1] = 1.0f;
		}
	}
	void matrix::set_mf(const std::vector<vec3f>& vv) {
		//3x3
		if (vv.size() == 3 && vv[0].size() == 3) {
			mf_.resize(3, std::vector<float>(3, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = vv[0].x;	mf_[0][1] = vv[0].y;	mf_[0][2] = vv[0].z;
			mf_[1][0] = vv[1].x;	mf_[1][1] = vv[1].y;	mf_[1][2] = vv[1].z;
			mf_[2][0] = vv[2].x;	mf_[2][1] = vv[2].y;	mf_[2][2] = vv[2].z;
		}
		//4x3
		else if (vv.size() == 4 && vv[0].size() == 3) {
			mf_.resize(4, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = vv[0].x;	mf_[0][1] = vv[0].y;	mf_[0][2] = vv[0].z;
			mf_[1][0] = vv[1].x;	mf_[1][1] = vv[1].y;	mf_[1][2] = vv[1].z;
			mf_[2][0] = vv[2].x;	mf_[2][1] = vv[2].y;	mf_[2][2] = vv[2].z;
			mf_[3][0] = 1.0f;	mf_[3][1] = 1.0f;		mf_[3][2] = 1.0f;
		}
	}
	void matrix::set_mf(const matrix& m) {
		//2x2
		if (m.get_row_size() == 2 && m.get_column_size() == 2) {
			mf_.resize(2, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = m.get_mf()[0][0];	mf_[0][1] = m.get_mf()[0][1];
			mf_[1][0] = m.get_mf()[1][0];	mf_[1][1] = m.get_mf()[1][1];
		}
		//3x2
		else if (m.get_row_size() == 3 && m.get_column_size() == 2) {
			mf_.resize(3, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = m.get_mf()[0][0];	mf_[0][1] = m.get_mf()[0][1];
			mf_[1][0] = m.get_mf()[1][0];	mf_[1][1] = m.get_mf()[1][1];
			mf_[2][0] = 1.0f;	mf_[2][1] = 1.0f;
		}
		//3x3
		if (m.get_row_size() == 3 && m.get_column_size() == 3) {
			mf_.resize(3, std::vector<float>(3, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = m.get_mf()[0][0];	mf_[0][1] = m.get_mf()[0][1];	mf_[0][2] = m.get_mf()[0][2];
			mf_[1][0] = m.get_mf()[1][0];	mf_[1][1] = m.get_mf()[1][1];	mf_[1][2] = m.get_mf()[1][2];
			mf_[2][0] = m.get_mf()[2][0];	mf_[2][1] = m.get_mf()[2][1];	mf_[2][2] = m.get_mf()[2][2];
		}
		//4x3
		else if (m.get_row_size() == 4 && m.get_column_size() == 3) {
			mf_.resize(4, std::vector<float>(3, 0));
			row = mf_.size();
			column = mf_[0].size();
			mf_[0][0] = m.get_mf()[0][0];	mf_[0][1] = m.get_mf()[0][1];	mf_[0][2] = m.get_mf()[0][2];
			mf_[1][0] = m.get_mf()[1][0];	mf_[1][1] = m.get_mf()[1][1];	mf_[1][2] = m.get_mf()[1][2];
			mf_[2][0] = m.get_mf()[2][0];	mf_[2][1] = m.get_mf()[2][1];	mf_[2][2] = m.get_mf()[2][2];
			mf_[3][0] = 1.0f;	mf_[3][1] = 1.0f;		mf_[3][2] = 1.0f;
		}
	}
}//math
}//zee