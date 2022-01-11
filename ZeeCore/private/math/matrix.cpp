#include "../../public/math/matrix.h"

namespace zee {
namespace math {
	///////////////////////////////////////////////////////////////////////////
	//楳慶 2x2//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	//奄沙
	matrix2f::matrix2f() noexcept : 
		m_{ { 0, 0 } }
	{
	}
	matrix2f::matrix2f(const size_t m, const size_t n) noexcept {
		m_.resize(m);
		matrix2f a, b, c;
		(a = b) = c;//細備澗暗 焼還艦陥. 焼 革..

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

	//楳引 伸 紫戚綜 伊紫
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
			ZEE_LOG(warning, TEXT("matrix"), TEXT("楳 箸精 伸 紫戚綜亜 陥絹艦陥."));
			assert(false);
		}
	}

	//咽疾拭辞 蒋楳慶(昔切)税 伸引 急楳慶(呉獄)税 楳戚 旭焼醤 敗
	const bool matrix2f::is_same_column_vs_row_size(const matrix2f& m) const {
		if (m.get_column_size() == get_row_size()) {
			return true;
		}
		else {
			//3x2咽聖 是背 蒸証
			//ZEE_LOG(warning, TEXT("matrix"), TEXT("湛 楳慶(昔切)税 伸 紫戚綜 != 砧腰属 楳慶(呉獄)税 楳 紫戚綜"));
			//assert(false);
		}
	}

	//尻至切
	matrix2f& matrix2f::operator=(const matrix2f& m) {
		set_m(m);
		return *this;
	}//白哀険呪赤製. 達焼左壱 促疾. 額
	//乞窮 適掘什亜 伽 煽掘醤 廃舘 依精 蒸澗汽
	//企採歳精 煽掘醤馬澗惟 限製.
	//幻鉦 const T& 坦軍 速陥檎 原競廃 戚政亜 赤嬢醤敗.
	//悦汽 戚適掘什澗 箭企 焼還.

	matrix2f& matrix2f::operator+=(const matrix2f& m) {
		if (is_same_size(m)) {
			add(m);
		}
		return *this;
	}

	matrix2f& matrix2f::operator-=(const matrix2f& m) {
		if (is_same_size(m)) {
			sub(m);
		}
		return *this;
	}

	matrix2f& matrix2f::operator*=(const matrix2f& m) {
		if (is_same_size(m)) {
			mul(m);
		}
		return *this;
	}

	//紫帳尻至
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
		//vec2f亜 角嬢尽陥檎 原走厳 腰属 1.0f 室特
		//痕発(戚疑, 什追析, 噺穿) 楳慶税 dx, dy人 咽疾聖 是背 限仲爽奄 是敗績
		std::vector<vec3f> v(3);
		for (int i = 0; i != m.get_row_size(); i++) {
			for (int j = 0; j != v[0].size(); j++) {
				if (j == 2) {
					v[i][j] = 1.0f;
				}
				else {
					v[i][j] = m.get_m()[i][j];
				}
			}
		}
		//咽尻至
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
			ZEE_LOG(warning, TEXT("matrix"), TEXT("薄仙 走据馬走 省澗 紫戚綜 咽脊艦陥."));
			assert(false);
		}

	}

	//痕発
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

	//楳慶縦: 2x2 ad-bc
	const float matrix2f::determinant() const { //const嬢巨姶? 寿走馬畏戎陥
		if (get_row_size() == 2) {
			float a = m_[0][0], b = m_[0][1];//呪舛亀 照馬澗杏.
			float c = m_[1][0], d = m_[1][1];
			return a * d - b * c;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("楳慶縦 紫戚綜亜 設公鞠醸柔艦陥."));
			assert(false);
		}
	}


	//蝕楳慶: (1, 0; 0, 1) 牌去楳慶 蟹神澗 縦. 1/ad-bc * (d -b; -c a).
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
				ZEE_LOG(warning, TEXT("matrix"), TEXT("ad - bc亜 0戚檎 蝕楳慶聖 幻級 呪 蒸柔艦陥."));
				assert(false);
			}
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("薄仙 走据馬走 省澗 紫戚綜 蝕楳慶脊艦陥."));
			assert(false);
		}
	}

	//舘是楳慶: 食採
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
			ZEE_LOG(warning, TEXT("matrix"), TEXT("2x2 戚雌税 舛号楳慶幻 牌去楳慶昔走 毒舘拝 呪 赤柔艦陥."));
			assert(false);
		}
	}

	//穿帖楳慶: 楳引 伸聖 嘘発
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
	//楳慶 3x3//////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	//奄沙
	matrix3f::matrix3f() noexcept 
		: m_{ { 0, 0, 0 } }
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
		set_vec({ v0, v1, v2 });
	}
	//4x3
	matrix3f::matrix3f(const vec3f v0, const vec3f v1, const vec3f v2, const vec3f v3) noexcept {
		set_vec({ v0, v1, v2, v3 });
	}

	//楳引 伸 紫戚綜 伊紫
	const bool matrix3f::is_same_size(const std::vector<vec3f>& vv) const {
		//剰稽 姥薄背兜生檎
		//戚係惟 馬澗惟 慨畏閃? しぞ
		//剰澗 梓端研 析採君 馬蟹 幻級切蟹推 益竹? 革革
		//希 可 可馬惟 坐食 
		matrix3f m(vv);
		//戚闇 鋼企稽 促醤馬澗惟 限走省聖猿推?
		//嬢託杷 戚更 疑旋拝雁聖 潤辞 陥獣 哀焼帖錘陥壱 背亀推

		return is_same_size(m);
	}
	const bool matrix3f::is_same_size(const matrix3f& m) const {
		return is_same_size(m.m_);

		if (get_row_size() == m.get_row_size() && get_column_size() == m.get_column_size()) {
			return true;
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("楳 箸精 伸 紫戚綜亜 陥絹艦陥."));
			assert(false);
		}
	}

	//咽疾拭辞 蒋楳慶(昔切)税 伸引 急楳慶(呉獄)税 楳戚 旭焼醤 敗
	const bool matrix3f::is_same_column_vs_row_size(const matrix3f& m) const {
		if (m.get_column_size() == get_row_size()) {
			return true;
		}
		else {
			//4x3咽聖 是背 蒸証
			//ZEE_LOG(warning, TEXT("matrix"), TEXT("湛 楳慶(昔切)税 伸 紫戚綜 != 砧腰属 楳慶(呉獄)税 楳 紫戚綜"));
			//assert(false);
		}
	}

	//尻至切
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

	//紫帳尻至
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
		//戚惟 杭 焼 析舘 困斗研 陥 皐壱 戚訓 依級亀 陥 蒸蕉醤 馬澗汽
		// //戚暗 戚耕 床壱赤倉? 額
		// ぞぞ./.
		//vec2f亜 角嬢尽陥檎 原走厳 腰属 1.0f 室特
		//痕発(戚疑, 什追析, 噺穿) 楳慶税 dx, dy人 咽疾聖 是背 限仲爽奄 是敗績
		std::vector<vec4f> v(4);
		for (int i = 0; i != m.get_row_size(); i++) {
			for (int j = 0; j != v[0].size(); j++) {
				if (j == 3) {
					v[i][j] = 1.0f;
				}
				else {
					v[i][j] = m.get_m()[i][j];
				}
			}
		}
		//咽尻至
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
			ZEE_LOG(warning, TEXT("matrix"), TEXT("薄仙 走据馬走 省澗 紫戚綜 咽脊艦陥."));
			assert(false);
		}

	}

	//痕発
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
		//z逐
		//3x4? 陥 楳戚 馬蟹梢 弦惟 梅澗汽 限澗走 乞牽畏嬢推. 益 戚疑 是背 2x2亀 3x2壱 3x3精 4x3戚壱
		//班 戚闇 紫叔 transform昔汽 t葵戚 級嬢亜檎
		//蟹亀 戚掘 速醸製 せせせせ 古闘遣什拭陥亜
		//闘沓什廿戚虞壱 適掘什研 幻級壱 暗奄辞 楳慶聖 嗣焼神惟 馬澗惟 限澗暗旭奄亀..
		//薦亜 郊空闇 益撹 益煽 2x2楳慶戚櫛 3x3楳慶戚醸製.
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

	//楳慶縦: 3x3 d(ei-fh)-b(di-fg)+c(dh-eg)
	const float matrix3f::determinant() {//c
		if (get_row_size() == 3) {
			float a = m_[0][0], b = m_[0][1], c = m_[0][2];
			float d = m_[1][0], e = m_[1][1], f = m_[1][2];
			float g = m_[2][0], h = m_[2][1], i = m_[2][2];
			return d * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
		}
		else {
			ZEE_LOG(warning, TEXT("matrix"), TEXT("楳慶縦 紫戚綜亜 設公鞠醸柔艦陥."));
			assert(false);
		}
	}

	//舘是楳慶: 食採
	const bool matrix3f::is_identity() {//c
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
			ZEE_LOG(warning, TEXT("matrix"), TEXT("3x3 戚雌税 舛号楳慶幻 牌去楳慶昔走 毒舘拝 呪 赤柔艦陥."));
			assert(false);
		}
	}

	//穿帖楳慶: 楳引 伸聖 嘘発
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