#include "effect.h"

namespace yjj {
	void effect::load_image() {
		frame_image::get().load_frame_image(coords_effect_[effect_bomb_size]
			, TEXT("assets/bomb.bmp"), (int)obj_type::effect_bomb);
	}

	void effect::init() {
		projectile::init();
		set_size(coords_effect_[effect_bomb_size]);
		set_frame_size(coords_effect_[effect_bomb_default_frame]);
		set_obj_type((int)obj_type::effect_bomb);
		set_now_pos_and_body(coords_effect_[destroy_effect_zone]);
	}

	void effect::spawn_from(const std::shared_ptr<unit> other) {
		//sound::get().play_sound_effect(TEXT("assets/sound_effect/Explosion5.wav"));
		projectile::spawn_from(other);
	}

	void effect::destroy(const float delta_time) {
		// �÷��̾ ���� ���� �� state::die ==> state::idle
		// ������
		constexpr float speed = 8.0f;
		constexpr float frame = 3.0f;
		set_delay_bomb(get_delay_bomb() + delta_time * speed);

		set_frame_size({ coords_effect_[effect_bomb_size].x * (int)get_delay_bomb(), 0});

		if (get_delay_bomb() >= frame) {
			set_state((int)obj_state::die);
			set_now_pos_and_body(coords_effect_[destroy_effect_zone]);
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