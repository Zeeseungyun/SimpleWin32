#include "../../public/math/matrix.h"

namespace zee {
namespace math {
	//기본
	matrix::matrix() noexcept : mf_{ { 0, 0 } }, row(1), column(1) {
	}
	matrix::matrix(const std::vector<vec2f>& vv) noexcept {
		set_m2f(vv);
	}
	matrix::matrix(const std::vector<vec3f>& vv) noexcept {
		set_m3f(vv);
	}
	matrix::matrix(const matrix& m) noexcept {
		set_mf(m);
	}
	//2x2
	matrix::matrix(const vec2f v0, const vec2f v1) noexcept {
		set_m2f({ v0, v1 });
	}
	//3x2
	matrix::matrix(const vec2f v0, const vec2f v1, const vec2f v2) noexcept {
		set_m2f({ v0, v1, v2 });
	}
	//3x3
	matrix::matrix(const vec3f v0, const vec3f v1, const vec3f v2) noexcept {
		set_m3f({ v0, v1, v2 });
	}
	//4x3
	matrix::matrix(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept {
		set_m3f({ v0, v1, v2, v3 });
	}
	matrix::~matrix() noexcept {
	}

	//행과 열 사이즈 검사
	const bool matrix::is_same_size(const std::vector<vec2f>& vv) const {
		if (row == vv.size() && column == vv[0].size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행 사이즈 != 열 사이즈"));
		}
		return false;
	}
	const bool matrix::is_same_size(const std::vector<vec3f>& vv) const {
		if (row == vv.size() && column == vv[0].size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행 사이즈 != 열 사이즈"));
		}
		return false;
	}
	const bool matrix::is_same_size(const matrix& m) const {
		if (row == m.get_row_size() && column == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행 사이즈 != 열 사이즈"));
		}
		return false;
	}

	//곱셈에서 앞행렬의 열과 뒷행렬의 행이 같아야 함 (+ 3x2 1x2 연산 위해 1 차이만큼을 인정함)
	const bool matrix::is_same_column_vs_row_size(const matrix& m) const {
		if (row == m.get_column_size() || row == m.get_column_size() + 1) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("첫 행렬의 열 사이즈 != 두번째 행렬의 행 사이즈"));
		}
		return false;
	}

	//연산자
	const matrix& matrix::operator=(const matrix& m) {
		set_mf(m);
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
		matrix m{ {v} };
		mul(m);
	}
	void matrix::matrix::mul(const vec3f& v) {
		matrix m{ {v} };
		mul(m);
	}
	void matrix::mul(const matrix& m) {
		if (is_same_column_vs_row_size(m)) {
			//mf_ = 3x2 -> 1x2
			if (m.get_row_size() == 1 && m.get_column_size() == 2 && mf_.size() > 2) {
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
			else if (m.get_row_size() == 1 && m.get_column_size() == 3 && mf_.size() > 3) {
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

	//행렬식: 2x2 ad-bc, 3x3 d(ei-fh)-b(di-fg)+c(dh-eg)
	const float matrix::determinant() {
		if (row == 2 && column == 2) {
			return mf_[0][0] * mf_[1][1] - mf_[0][1] * mf_[1][0];
		}
		else if (row == 3 && column == 3) {
			float a = mf_[0][0], b = mf_[0][1], c = mf_[0][2];
			float d = mf_[1][0], e = mf_[1][1], f = mf_[1][2];
			float g = mf_[2][0], h = mf_[2][1], i = mf_[2][2];
			return d * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행렬식 사이즈가 잘못되었습니다."));
			return npos;
		}
	}


	//역행렬: (1, 0; 0, 1) 항등행렬 나오는 식. 1/ad-bc * (d -b; -c a). 없으면 로그 띄우고 초기 행렬 반환.
	const matrix& matrix::inverse(const std::vector<vec2f>& vv) {
		if (vv.size() == 2 && vv[0].size() == 2) {
			this->set_m2f(vv);
		}
		return inverse(*this);
	}
	const matrix& matrix::inverse(const matrix& m) {
		matrix ret;
		if (m.get_row_size() == 2 && m.get_column_size() == 2) {
			if (m.get_mf()[0][0] * m.get_mf()[1][1] - m.get_mf()[0][1] * m.get_mf()[1][0]) {
				float a = m.get_mf()[0][0], b = m.get_mf()[0][1];
				float c = m.get_mf()[1][0], d = m.get_mf()[1][1];
				ret.set_m2f({
					{ 1 / (a * d - b * c) * d, 1 / (a * d - b * c) * -b },
					{ 1 / (a * d - b * c) * -c, 1 / (a * d - b * c) * a }
				});
			}
			else {
				ZEE_LOG(warning, TEXT("matrix"), TEXT("ad - bc가 0이면 역행렬을 만들 수 없습니다."));
			}
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("현재 지원하지 않는 사이즈 역행렬입니다."));
		}
		return ret;
	}

	//항등행렬 여부
	const bool matrix::is_identity() {
		bool is_identity = false;
		if (this->get_row_size() > 1 && this->get_column_size() > 1
			&& this->get_row_size() == this->get_column_size()) {
			for (int i = 0; i != this->get_row_size(); i++) {
				for (int j = 0; j != this->get_column_size(); j++) {
					if (i == j && this->get_mf()[i][j] == 1) {
						is_identity = true;
					}
					else if (i != j && this->get_mf()[i][j] == 0) {
						is_identity = true;
					}
					else {
						is_identity = false;
						break;
					}
				}
				if (!is_identity) {
					break;
				}
			}
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("2x2 이상의 정방행렬만 항등행렬인지 판단할 수 있습니다."));
		}
		return is_identity;
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
		row = vv.size();
		column = vv[0].size();
		mf_.resize(vv.size());
		for (int i = 0; i != row; i++) {
			mf_[i].resize(vv[i].size());
		}
		//1x2, 2x2, 3x2
		for (int i = 0; i != row; i++) {
			for (int j = 0; j != column; j++) {
				mf_[i][j] = vv[i][j];
			}
		}
	}
	void matrix::set_m3f(const std::vector<vec3f>& vv) {
		row = vv.size();
		column = vv[0].size();
		mf_.resize(vv.size());
		for (int i = 0; i != row; i++) {
			mf_[i].resize(vv[i].size());
		}
		//1x3, 3x3, 4x3
		for (int i = 0; i != row; i++) {
			for (int j = 0; j != column; j++) {
				mf_[i][j] = vv[i][j];
			}
		}
	}
	void matrix::set_mf(const matrix& m) {
		row = m.get_row_size();
		column = m.get_column_size();
		mf_.resize(m.get_row_size(), std::vector<float>(m.get_column_size(), npos));
		//1x2, 2x2, 3x2, 1x3, 3x3, 4x3
		for (int i = 0; i != row; i++) {
			for (int j = 0; j != column; j++) {
				mf_[i][j] = m.get_mf()[i][j];
			}
		}
	}
}//math
}//zee