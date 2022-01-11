#include "effect.h"

namespace zee {
	void effect::load_image() {
		frame_image::get().load_frame_image(coords_[effect_bomb_size]
			, TEXT("assets/bomb.bmp"), (int)obj_type::bomb);
	}

	void effect::init() {
		set_size(coords_[effect_bomb_size]);
		set_frame_size(coords_[effect_bomb_default_frame]);
		set_delay(0.0f);
	}

	void effect::destroy(const float delta_time) {
		//���� ����Ʈ ƽ: ������ �÷��̾ ���� ���� �� state::die ==> state::idle
		if (state_ == (int)obj_state::idle) {
			const float speed = 8.0f;
			const float frame = 3.0f;
			delay_bomb_ += delta_time * speed;

			set_frame_size({ coords_[effect_bomb_size].x * (int)delay_bomb_, 0 });

			if (delay_bomb_ >= frame) {
				set_state((int)obj_state::die);
			}
			delay_bomb_ = (float)math::fmod(delay_bomb_, frame);
		}
	}
	void effect::render(win32gdi::device_context_dynamic& dest_dc) {
		if(in_screen() && state_ == (int)obj_state::idle) {
			//����
			frame_image::get().render_transparent(
				dest_dc,
				now_pos_,
				frame_x_ + frame_y_,
				(int)obj_type::bomb
			);
		}

		//�浹���� �׽�Ʈ
		unit::render(dest_dc);
	}

	void effect::set_delay(const float delay) {
		delay_bomb_ = delay;
	}
}