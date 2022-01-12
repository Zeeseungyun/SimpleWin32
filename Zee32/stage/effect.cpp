#include "effect.h"

namespace zee {
	void effect::load_image() {
		frame_image::get().load_frame_image(coords_[effect_bomb_size]
			, TEXT("assets/bomb.bmp"), (int)obj_type::bomb);
	}

	void effect::init(const int obj_state) {
		unit::init(obj_state);
		set_size(coords_[effect_bomb_size]);
		set_frame_size(coords_[effect_bomb_default_frame]);
	}

	void effect::spawn_from(const std::shared_ptr<unit> other) {
		projectile::spawn_from(other);
		set_delay_bomb(0.0f);
	}

	void effect::destroy(const float delta_time) {
		//Æø¹ß ÀÌÆåÆ® Æ½: Æø¹ßÀº ÇÃ·¹ÀÌ¾î³ª ÀûÀÌ Á×À» ¶§ state::die ==> state::idle
		const float speed = 8.0f;
		const float frame = 3.0f;
		set_delay_bomb(get_delay_bomb() + delta_time * speed);

		set_frame_size({ coords_[effect_bomb_size].x * (int)get_delay_bomb(), 0});

		if (get_delay_bomb() >= frame) {
			set_state((int)obj_state::die);
		}
		set_delay_bomb((float)math::fmod(get_delay_bomb(), frame));
	}
	void effect::render(win32gdi::device_context_dynamic& dest_dc) {
		if(in_screen() && get_state() == (int)obj_state::idle) {
			//Æø¹ß
			frame_image::get().render_transparent(
				dest_dc,
				get_now_pos(),
				get_frame_x() + get_frame_y(),
				(int)obj_type::bomb
			);
		}

		//Ãæµ¹¹üÀ§ Å×½ºÆ®
		unit::render(dest_dc);
	}

	const float effect::get_delay_bomb() const {
		return delay_bomb_;
	}
	void effect::set_delay_bomb(const float delay) {
		delay_bomb_ = delay;
	}
}