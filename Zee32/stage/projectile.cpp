#include "projectile.h"

namespace zee {
	void projectile::init(const int obj_state) {
		unit::init(obj_state);
	}

	void projectile::spawn_from(const std::shared_ptr<unit> other) {
		unit::init((int)obj_state::idle);
		set_now_pos_and_body(
			{ other->get_body().origin.x - other->get_body().radius / 2,
			other->get_body().origin.y - other->get_body().radius / 2 }
		);
	}

	void projectile::spawn_from(const int obj_type, const shape::circlef& obj_body) {
		unit::init((int)obj_state::idle);
	}

	void projectile::destroy(const float delta_time) {
		unit::destroy(delta_time);
	}
	void projectile::render(win32gdi::device_context_dynamic& dest_dc) {
	}

	void projectile::set_delay(const float delay) {
	}
}