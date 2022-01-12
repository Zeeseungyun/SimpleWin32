#include "plane.h"

namespace zee {

	void plane::init(const int obj_state) {
		unit::init(obj_state);
		set_atk(1);
	}

	void plane::spawn() {
	}
}