#include "plane.h"

namespace zee {

	void plane::init(const int obj_state) {
		unit::init(obj_state);
		set_atk(1);
		set_my_score(1);
	}

	void plane::spawn() {
	}

	void plane::destroy(const float delta_time) {
		unit::destroy(delta_time);
	}
	void plane::render(win32gdi::device_context_dynamic& dest_dc) {
		//충돌범위 테스트
		unit::render(dest_dc);
	}

}