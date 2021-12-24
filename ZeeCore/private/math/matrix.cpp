#include "../../public/math/matrix.h"

namespace zee {
namespace math {
	//기본
	matrix::matrix() noexcept : mf_{ 0 }, row(0), column(0) {
	}
	matrix::matrix(const matrix& m) noexcept {
		add(m);
	}
	//2x2
	matrix::matrix(const vec2f v0, const vec2f v1) noexcept {
		set_m2f({ { v0, v1 } });
	}
	//3x2
	matrix::matrix(const vec2f v0, const vec2f v1, const vec2f v2) noexcept {
		set_m2f({ { v0, v1, v2 } });
	}
	//3x3
	matrix::matrix(const vec3f v0, const vec3f v1, const vec3f v2) noexcept {
		set_m3f({ { v0, v1, v2 } });
	}
	//4x3
	matrix::matrix(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept {
		set_m3f({ { v0, v1, v2, v3 } });
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
		return false;
	}
	const bool matrix::is_same_size(const std::vector<vec3f>& vv) const {
		if (row == vv.size() && column == vv[0].size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("row size != matrix member row size"));
		}
		return false;
	}
	const bool matrix::is_same_size(const matrix& m) const {
		if (row == m.get_row_size() && column == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("row size != matrix member row size"));
		}
		return false;
	}

	//곱셈에서 앞행렬의 열과 뒷행렬의 행이 같아야 함 (+ 3x2 1x2 연산 위해 1 차이만큼을 인정함)
	const bool matrix::is_same_column_vs_row_size(const std::vector<vec2f>& vv) const {
		if (column == vv.size() || column == vv.size() + 1) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("first matrix column size != second matrix row size"));
		}
		return false;
	}
	const bool matrix::is_same_column_vs_row_size(const std::vector<vec3f>& vv) const {
		if (column == vv.size() || column == vv.size() + 1) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("first matrix column size != second matrix row size"));
		}
		return false;
	}
	const bool matrix::is_same_column_vs_row_size(const matrix& m) const {
		if (row == m.get_column_size() || row == m.get_column_size() + 1) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("first matrix column size != second matrix row size"));
		}
		return false;
	}

	//연산자
	const matrix& matrix::operator=(const matrix& m) {
		if (is_same_size(m)) {
			set_mf(m);
		}
		return *this;
	}
	const matrix& matrix::operator+=(const matrix& m) {
		if (is_same_size(m)) {
			add(m);
		}
		return *this;
	}
	const matrix& matrix::operator-=(const matrix& m) {
		if (is_same_size(m)) {
			sub(m);
		}
		return *this;
	}
	const matrix& matrix::operator*=(const matrix& m) {
		if (is_same_size(m)) {
			mul(m);
		}
		return *this;
	}

	//사칙연산
	void matrix::add(const matrix& m) {
		if (is_same_size(m)) {
			for (int i = 0; i != row; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] += m.get_mf()[i][j];
				}
			}
		}
	}

	void matrix::sub(const matrix& m) {
		if (is_same_size(m)) {
			for (int i = 0; i != row; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] -= m.get_mf()[i][j];
				}
			}
		}
	}

	void matrix::mul(const vec2f& v) {
		std::vector<vec2f> vv{ v };
		if (is_same_column_vs_row_size(vv)) {
			//mf_ = 3x2 -> 1x2
			if (vv.size() == 1 && vv[0].size() == 2) {
				mf_[0] = {
					vv[0][0] * mf_[0][0]
					+ vv[0][1] * mf_[1][0]
						+ 1.0f * mf_[2][0]
					,
					vv[0][0] * mf_[0][1]
					+ vv[0][1] * mf_[1][1]
						+ 1.0f * mf_[2][1]
				};
				mf_.resize(1);
				mf_[0].resize(2);
			}
		}
	}
	void matrix::matrix::mul(const vec3f& v) {
		std::vector<vec3f> vv{ v };
		if (is_same_column_vs_row_size(vv)) {
			//mf_ = 4x3 -> 1x3
			if (vv.size() == 1 && vv[0].size() == 3) {
				mf_[0] = {
					vv[0][0] * mf_[0][0]
					+ vv[0][1] * mf_[1][0]
					+ vv[0][2] * mf_[2][0]
						+ 1.0f * mf_[3][0]
					,
					vv[0][0] * mf_[0][1]
					+ vv[0][1] * mf_[1][1]
					+ vv[0][2] * mf_[2][1]
						+ 1.0f * mf_[3][1]
					,
					vv[0][0] * mf_[0][2]
					+ vv[0][1] * mf_[1][2]
					+ vv[0][2] * mf_[2][2]
						+ 1.0f * mf_[3][2] 
				};
				mf_.resize(1);
				mf_[0].resize(3);
			}
		}
	}
	void matrix::mul(const matrix& m) {
		if (is_same_column_vs_row_size(m)) {
			//mf_ = 3x2 -> 1x2
			if (m.get_row_size() == 1 && m.get_column_size() == 2) {
				mf_[0] = {
					{ m.get_mf()[0][0] * mf_[0][0]
					+ m.get_mf()[0][1] * mf_[1][0]
								+ 1.0f * mf_[2][0]
					,
					m.get_mf()[0][0] * mf_[0][1]
					+ m.get_mf()[0][1] * mf_[1][1]
								+ 1.0f * mf_[2][1] }
				};
				mf_.resize(1);
				mf_[0].resize(2);
			}
			//mf_ = 4x3 -> 1x3
			else if (m.get_row_size() == 1 && m.get_column_size() == 3) {
				mf_[0] = {
					{ m.get_mf()[0][0] * mf_[0][0]
					+ m.get_mf()[0][1] * mf_[1][0]
					+ m.get_mf()[0][2] * mf_[2][0]
								+ 1.0f * mf_[3][0]
					,
					m.get_mf()[0][0] * mf_[0][1]
					+ m.get_mf()[0][1] * mf_[1][1]
					+ m.get_mf()[0][2] * mf_[2][1]
								+ 1.0f * mf_[3][1]
					,
					m.get_mf()[0][0] * mf_[0][2]
					+ m.get_mf()[0][1] * mf_[1][2]
					+ m.get_mf()[0][2] * mf_[2][2]
								+ 1.0f * mf_[3][2] }
				};
				mf_.resize(1);
				mf_[0].resize(3);
			}
			else {
				ZEE_LOG(warning, TEXT("matrix"), TEXT("현재 지원하지 않는 사이즈 곱입니다."));
			}
		}
	}

	//get, set
	const std::vector<std::vector<float>>& matrix::get_mf() const {
		return mf_;
	}
	const size_t matrix::get_row_size() const {
		return mf_.size();
	}
	const size_t matrix::get_column_size() const {
		return mf_[0].size();
	}
	void matrix::set_m2f(const std::vector<vec2f>& vv) {
		//2x2
		if (vv.size() == 2 && vv[0].size() == 2) {
			mf_.resize(2, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = vv[i][j];
				}
			}
		}
		//3x2
		else if (vv.size() == 3 && vv[0].size() == 2) {
			mf_.resize(3, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row - 1; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = vv[i][j];
				}
			}
			for (int i = 0; i != column; i++) {
				mf_[2][i] = vv[2][i];
			}
		}
	}
	void matrix::set_m3f(const std::vector<vec3f>& vv) {
		//3x3
		if (vv.size() == 3 && vv[0].size() == 3) {
			mf_.resize(3, std::vector<float>(3, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = vv[i][j];
				}
			}
		}
		//4x3
		else if (vv.size() == 4 && vv[0].size() == 3) {
			mf_.resize(4, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row - 1; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = vv[i][j];
				}
			}
			for (int i = 0; i != column; i++) {
				mf_[3][i] = vv[3][i];
			}
		}
	}
	void matrix::set_mf(const matrix& m) {
		//2x2
		if (m.get_row_size() == 2 && m.get_column_size() == 2) {
			mf_.resize(2, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = m.get_mf()[i][j];
				}
			}
		}
		//3x2
		else if (m.get_row_size() == 3 && m.get_column_size() == 2) {
			mf_.resize(3, std::vector<float>(2, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row - 1; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = m.get_mf()[i][j];
				}
			}
			for (int i = 0; i != column; i++) {
				mf_[2][i] = m.get_mf()[2][i];
			}
		}
		//3x3
		if (m.get_row_size() == 3 && m.get_column_size() == 3) {
			mf_.resize(3, std::vector<float>(3, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = m.get_mf()[i][j];
				}
			}
		}
		//4x3
		else if (m.get_row_size() == 4 && m.get_column_size() == 3) {
			mf_.resize(4, std::vector<float>(3, 0));
			row = mf_.size();
			column = mf_[0].size();
			for (int i = 0; i != row - 1; i++) {
				for (int j = 0; j != column; j++) {
					mf_[i][j] = m.get_mf()[i][j];
				}
			}
			for (int i = 0; i != column; i++) {
				mf_[3][i] = m.get_mf()[3][i];
			}
		}
	}
}//math
}//zee