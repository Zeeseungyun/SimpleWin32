#include "projectile.h"

namespace zee {

	void projectile::init() {
		unit::init();
		set_state((int)obj_state::die);
		set_hp(0);
	}

	void projectile::spawn_from(const std::shared_ptr<unit> other) {
		set_state((int)obj_state::idle);
		set_hp(1);
		set_now_pos_and_body(
			{ other->get_body().origin.x - other->get_body().radius / 2,
			other->get_body().origin.y - other->get_body().radius / 2 }
		);
	}

	void projectile::spawn_from(const int obj_type, const shape::circlef& obj_body) {
		set_state((int)obj_state::idle);
		set_hp(1);
	}

	void projectile::render(win32gdi::device_context_dynamic& dest_dc) {

		if (in_screen() 
			&& get_state() == (int)obj_state::idle) {
			//폭발
			frame_image::get().render_transparent(
				dest_dc,
				get_now_pos(),
				get_frame_x() + get_frame_y(),
				get_obj_type()
			);
		}

		//충돌범위 테스트
		unit::render(dest_dc);
	}
}