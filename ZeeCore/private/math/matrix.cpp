#include "../../public/math/matrix.h"

namespace zee {
namespace math {
	///////////////////////////////////////////////////////////////////////////
	//행렬 2x2//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	//기본
	matrix2f::matrix2f() noexcept : 
		m_{ { 0, 0 } }
	{
	}
	matrix2f::matrix2f(const size_t m, const size_t n) noexcept {
		m_.resize(m);
	}
	matrix2f::matrix2f(const std::vector<vec2f>& vv) noexcept {
		set_vec(vv);
	}
	matrix2f::matrix2f(const matrix2f& m) noexcept {
		set_m(m);
	}
	//2x2
	matrix2f::matrix2f(const vec2f v0, const vec2f v1) noexcept {
		set_vec({ v0, v1 });
	}
	//3x2
	matrix2f::matrix2f(const vec2f v0, const vec2f v1, const vec2f v2) noexcept {
		set_vec({ v0, v1, v2 });
	}

	//행과 열 사이즈 검사
	const bool matrix2f::is_same_size(const std::vector<vec2f>& vv) const {
		matrix2f m(vv);
		return is_same_size(m);
	}
	const bool matrix2f::is_same_size(const matrix2f& m) const {
		if (get_row_size() == m.get_row_size() 
			&& get_column_size() == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행 혹은 열 사이즈가 다릅니다."));
			assert(false);
		}
	}

	//곱셈에서 앞행렬(인자)의 열과 뒷행렬(멤버)의 행이 같아야 함
	const bool matrix2f::is_same_column_vs_row_size(const matrix2f& m) const {
		if (m.get_column_size() == get_row_size()) {
			return true;
		}
		else {
			//3x2곱을 위해 없앰
			//ZEE_LOG(warning, TEXT("matrix"), TEXT("첫 행렬(인자)의 열 사이즈 != 두번째 행렬(멤버)의 행 사이즈"));
			//assert(false);
		}
	}

	//연산자
	const matrix2f& matrix2f::operator=(const matrix2f& m) {
		set_m(m);
		return *this;
	}

	const matrix2f& matrix2f::operator+=(const matrix2f& m) {
		if (is_same_size(m)) {
			add(m);
		}
		return *this;
	}

	const matrix2f& matrix2f::operator-=(const matrix2f& m) {
		if (is_same_size(m)) {
			sub(m);
		}
		return *this;
	}

	const matrix2f& matrix2f::operator*=(const matrix2f& m) {
		if (is_same_size(m)) {
			mul(m);
		}
		return *this;
	}

	//사칙연산
	void matrix2f::add(const matrix2f& m) {
		if (is_same_size(m)) {
			for (int i = 0; i != get_row_size(); i++) {
				m_[i] += m.get_m()[i];
			}
		}
	}

	void matrix2f::sub(const matrix2f& m) {
		if (is_same_size(m)) {
			for (int i = 0; i != get_row_size(); i++) {
				m_[i] -= m.get_m()[i];
			}
		}
	}

	void matrix2f::mul(const vec2f& v) {
		matrix2f m{ {v} };
		mul(m);
	}
	void matrix2f::mul(const matrix2f& m) {
		//vec2f가 넘어왔다면 마지막 번째 1.0f 세팅
		//변환(이동, 스케일, 회전) 행렬의 dx, dy와 곱셈을 위해 맞춰주기 위함임
		std::vector<vec3f> v(3);
		if (m.get_row_size() == 1 && m.get_column_size() == 2) {
			v[0][0] = m.get_m()[0][0];
			v[0][1] = m.get_m()[0][1];
			v[0][2] = 1.0f;
		}
		//곱연산
		if (is_same_column_vs_row_size(m)) {
			matrix2f tmp(m.get_row_size(), m.get_column_size());

			for (int i = 0; i != m.get_row_size(); i++) {
				for (int j = 0; j != m_[0].size(); j++) {
					for (int k = 0; k != m_.size(); k++) {
						tmp.m_[i][j] += v[i][k] * m_[k][j];
					}
				}
			}
			m_ = tmp.get_m();
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("현재 지원하지 않는 사이즈 곱입니다."));
			assert(false);
		}

	}

	//변환
	void matrix2f::translation(const float dx, const float dy) {
		set_vec({
			{ 1, 0 },
			{ 0, 1 },
			{ dx, dy }
		});
	}
	void matrix2f::translation(const float angle, const float dx, const float dy) {
		set_vec({
			{ 1, 0 },
			{ 0, 1 },
			{ cos(angle) * dx, sin(angle) * dy }
		});
	}

	void matrix2f::scale(const float sx, const float sy) {
		set_vec({
			{ sx, 0 },
			{ 0, sy },
			{ 0, 0 }
		});
	}

	void matrix2f::rotation(const float angle, const float dx, const float dy) {
		set_vec({
			{ cos(angle), -sin(angle) },
			{ sin(angle), cos(angle) },
			{ dx, dy }
		});
	}

	//행렬식: 2x2 ad-bc
	const float matrix2f::determinant() {
		if (get_row_size() == 2) {
			float a = m_[0][0], b = m_[0][1];
			float c = m_[1][0], d = m_[1][1];
			return a * d - b * c;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행렬식 사이즈가 잘못되었습니다."));
			assert(false);
		}
	}


	//역행렬: (1, 0; 0, 1) 항등행렬 나오는 식. 1/ad-bc * (d -b; -c a).
	void matrix2f::inverse(const std::vector<vec2f>& vv) {
		if (vv.size() == 2) {
			set_vec(vv);
		}
		inverse(*this);
	}
	void matrix2f::inverse(const matrix2f& m) {
		if (m.get_row_size() == 2) {
			if (m.get_m()[0][0] * m.get_m()[1][1] - m.get_m()[0][1] * m.get_m()[1][0]) {
				float a = m.get_m()[0][0]; float b = m.get_m()[0][1];
				float c = m.get_m()[1][0]; float d = m.get_m()[1][1];
				set_vec({
					{ 1 / (a * d - b * c) * d, 1 / (a * d - b * c) * -b },
					{ 1 / (a * d - b * c) * -c, 1 / (a * d - b * c) * a }
				});
			}
			else {
				ZEE_LOG(warning, TEXT("matrix"), TEXT("ad - bc가 0이면 역행렬을 만들 수 없습니다."));
				assert(false);
			}
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("현재 지원하지 않는 사이즈 역행렬입니다."));
			assert(false);
		}
	}

	//단위행렬: 여부
	const bool matrix2f::is_identity() {
		bool is_identity = false;
		if (get_row_size() > 1 
			&& get_column_size() > 1
			&& get_row_size() == get_column_size()) 
		{
			for (int i = 0; i != get_row_size(); i++) {
				for (int j = 0; j != get_column_size(); j++) {

					is_identity = (i == j) ? (get_m()[i][j] == 1) : (get_m()[i][j] == 0);

					if (!is_identity) {
						return is_identity;
					}
				}
			}
			return is_identity;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("2x2 이상의 정방행렬만 항등행렬인지 판단할 수 있습니다."));
			assert(false);
		}
	}

	//전치행렬: 행과 열을 교환
	void matrix2f::transposed() {
		matrix2f tmp(m_[0].size(), m_.size());

		for (int i = 0; i != tmp.get_m().size(); i++) {
			for (int j = 0; j != tmp.get_m()[0].size(); j++) {
				tmp.m_[i][j] = m_[j][i];
			}
		}

		set_m(tmp);
	}

	//get, set
	const std::vector<vec2f>& matrix2f::get_m() const {
		return m_;
	}
	const size_t matrix2f::get_row_size() const {
		return m_.size();
	}
	const size_t matrix2f::get_column_size() const {
		return m_[0].size();
	}
	void matrix2f::set_vec(const std::vector<vec2f>& vv) {
		m_.resize(vv.size());
		for (int i = 0; i != get_row_size(); i++) {
			m_[i] = vv[i];
		}
	}
	void matrix2f::set_m(const matrix2f& m) {
		m_.resize(m.get_row_size());
		for (int i = 0; i != get_row_size(); i++) {
			m_[i] = m.get_m()[i];
		}
	}








	///////////////////////////////////////////////////////////////////////////
	//행렬 3x3//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	//기본
	matrix3f::matrix3f() noexcept :
		m_{ { 0, 0, 0 } }
	{
	}
	matrix3f::matrix3f(const size_t m, const size_t n) noexcept {
		m_.resize(m);
	}
	matrix3f::matrix3f(const std::vector<vec3f>& vv) noexcept {
		set_vec(vv);
	}
	matrix3f::matrix3f(const matrix3f& m) noexcept {
		set_m(m);
	}
	//3x3
	matrix3f::matrix3f(const vec3f v0, const vec3f v1, const vec3f v2) noexcept {
		set_vec({ v0, v1 });
	}
	//4x3
	matrix3f::matrix3f(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept {
		set_vec({ v0, v1, v2, v3 });
	}

	//행과 열 사이즈 검사
	const bool matrix3f::is_same_size(const std::vector<vec3f>& vv) const {
		matrix3f m(vv);
		return is_same_size(m);
	}
	const bool matrix3f::is_same_size(const matrix3f& m) const {
		if (get_row_size() == m.get_row_size()
			&& get_column_size() == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행 혹은 열 사이즈가 다릅니다."));
			assert(false);
		}
	}

	//곱셈에서 앞행렬(인자)의 열과 뒷행렬(멤버)의 행이 같아야 함
	const bool matrix3f::is_same_column_vs_row_size(const matrix3f& m) const {
		if (m.get_column_size() == get_row_size()) {
			return true;
		}
		else {
			//4x3곱을 위해 없앰
			//ZEE_LOG(warning, TEXT("matrix"), TEXT("첫 행렬(인자)의 열 사이즈 != 두번째 행렬(멤버)의 행 사이즈"));
			//assert(false);
		}
	}

	//연산자
	const matrix3f& matrix3f::operator=(const matrix3f& m) {
		set_m(m);
		return *this;
	}

	const matrix3f& matrix3f::operator+=(const matrix3f& m) {
		if (is_same_size(m)) {
			add(m);
		}
		return *this;
	}

	const matrix3f& matrix3f::operator-=(const matrix3f& m) {
		if (is_same_size(m)) {
			sub(m);
		}
		return *this;
	}

	const matrix3f& matrix3f::operator*=(const matrix3f& m) {
		if (is_same_size(m)) {
			mul(m);
		}
		return *this;
	}

	//사칙연산
	void matrix3f::add(const matrix3f& m) {
		if (is_same_size(m)) {
			for (int i = 0; i != get_row_size(); i++) {
				m_[i] += m.get_m()[i];
			}
		}
	}

	void matrix3f::sub(const matrix3f& m) {
		if (is_same_size(m)) {
			for (int i = 0; i != get_row_size(); i++) {
				m_[i] -= m.get_m()[i];
			}
		}
	}

	void matrix3f::mul(const vec3f& v) {
		matrix3f m{ {v} };
		mul(m);
	}
	void matrix3f::mul(const matrix3f& m) {
		//vec2f가 넘어왔다면 마지막 번째 1.0f 세팅
		//변환(이동, 스케일, 회전) 행렬의 dx, dy와 곱셈을 위해 맞춰주기 위함임
		std::vector<vec4f> v(4);
		if (m.get_row_size() == 1 && m.get_column_size() == 3) {
			v[0][0] = m.get_m()[0][0];
			v[0][1] = m.get_m()[0][1];
			v[0][2] = m.get_m()[0][2];
			v[0][3] = 1.0f;
		}
		//곱연산
		if (is_same_column_vs_row_size(m)) {
			matrix3f tmp(m.get_row_size(), m.get_column_size());

			for (int i = 0; i != m.get_row_size(); i++) {
				for (int j = 0; j != m_[0].size(); j++) {
					for (int k = 0; k != m_.size(); k++) {
						tmp.m_[i][j] += v[i][k] * m_[k][j];
					}
				}
			}
			m_ = tmp.get_m();
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("현재 지원하지 않는 사이즈 곱입니다."));
			assert(false);
		}

	}

	//변환
	void matrix3f::translation(const float dx, const float dy, const float dz) {
		set_vec({
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
			{ dx, dy, dz }
		});
	}
	void matrix3f::translation(const float angle, const float dx, const float dy, const float dz) {
		set_vec({
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
			{ cos(angle) * dx, sin(angle) * dy, dz }
		});
	}

	void matrix3f::scale(const float sx, const float sy, const float sz) {
		//z축
		set_vec({
			{ sx, 0, 0 },
			{ 0, sy, 0 },
			{ 0, 0, sz },
			{ 0, 0, 0 }
		});
	}

	void matrix3f::rotation(const float angle, const float dx, const float dy, const float dz) {
		set_vec({
			{ cos(angle), -sin(angle), 0 },
			{ sin(angle), cos(angle), 0 },
			{ dx, dy, 1 },
			{ 0, 0, 0 }
			});
	}

	//행렬식: 3x3 d(ei-fh)-b(di-fg)+c(dh-eg)
	const float matrix3f::determinant() {
		if (get_row_size() == 3) {
			float a = m_[0][0], b = m_[0][1], c = m_[0][2];
			float d = m_[1][0], e = m_[1][1], f = m_[1][2];
			float g = m_[2][0], h = m_[2][1], i = m_[2][2];
			return d * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("행렬식 사이즈가 잘못되었습니다."));
			assert(false);
		}
	}

	//단위행렬: 여부
	const bool matrix3f::is_identity() {
		bool is_identity = false;
		if (get_row_size() > 1
			&& get_column_size() > 1
			&& get_row_size() == get_column_size())
		{
			for (int i = 0; i != get_row_size(); i++) {
				for (int j = 0; j != get_column_size(); j++) {

					is_identity = (i == j) ? (get_m()[i][j] == 1) : (get_m()[i][j] == 0);

					if (!is_identity) {
						return is_identity;
					}
				}
			}
			return is_identity;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("3x3 이상의 정방행렬만 항등행렬인지 판단할 수 있습니다."));
			assert(false);
		}
	}

	//전치행렬: 행과 열을 교환
	void matrix3f::transposed() {
		matrix3f tmp(m_[0].size(), m_.size());

		for (int i = 0; i != tmp.get_m().size(); i++) {
			for (int j = 0; j != tmp.get_m()[0].size(); j++) {
				tmp.m_[i][j] = m_[j][i];
			}
		}

		set_m(tmp);
	}

	//get, set
	const std::vector<vec3f>& matrix3f::get_m() const {
		return m_;
	}
	const size_t matrix3f::get_row_size() const {
		return m_.size();
	}
	const size_t matrix3f::get_column_size() const {
		return m_[0].size();
	}
	void matrix3f::set_vec(const std::vector<vec3f>& vv) {
		m_.resize(vv.size());
		for (int i = 0; i != get_row_size(); i++) {
			m_[i] = vv[i];
		}
	}
	void matrix3f::set_m(const matrix3f& m) {
		m_.resize(m.get_row_size());
		for (int i = 0; i != get_row_size(); i++) {
			m_[i] = m.get_m()[i];
		}
	}
}//math
}//zee