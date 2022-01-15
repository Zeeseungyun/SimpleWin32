#include "effect.h"

namespace zee {
	void effect::load_image() {
		frame_image::get().load_frame_image(coords_[effect_bomb_size]
			, TEXT("assets/bomb.bmp"), (int)obj_type::effect_bomb);
	}

	void effect::init() {
		projectile::init();
		set_size(coords_[effect_bomb_size]);
		set_frame_size(coords_[effect_bomb_default_frame]);
		set_obj_type((int)obj_type::effect_bomb);
	}

	void effect::spawn_from(const std::shared_ptr<unit> other) {
		projectile::spawn_from(other);
		set_delay_bomb(0.0f);
	}

	void effect::destroy(const float delta_time) {
		//플레이어나 적이 죽을 때 state::die ==> state::idle
		constexpr float speed = 8.0f;
		constexpr float frame = 3.0f;
		set_delay_bomb(get_delay_bomb() + delta_time * speed);

		set_frame_size({ coords_[effect_bomb_size].x * (int)get_delay_bomb(), 0});

		if (get_delay_bomb() >= frame) {
			set_state((int)obj_state::die);
		}
		set_delay_bomb((float)math::fmod(get_delay_bomb(), frame));
	}

	void effect::render(win32gdi::device_context_dynamic& dest_dc) {
		projectile::render(dest_dc);
	}

	float effect::get_delay_bomb() const {
		return delay_bomb_;
	}
	void effect::set_delay_bomb(const float delay) {
		delay_bomb_ = delay;
	}
}