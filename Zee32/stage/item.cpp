#include "item.h"
#include "player.h"

namespace zee {
	void item::load_image() {
		frame_image::get().load_frame_image(coords_[item_size]
			, TEXT("assets/item_score.bmp"), (int)obj_type::item);
	}

	void item::init() {
		unit::init();
		set_my_score(3);
		set_size(coords_[item_size]);
		set_state((int)obj_state::idle);
	}

	void item::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//other이 player를 가리키고 nullptr이 아니면
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			unit::hit_from(other, delta_time);
			other_player->add_score(my_score_);
		}
	}

	void item::destroy(const float delta_time) {
		unit::destroy(delta_time);
	}

	void item::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen() && state_ == (int)obj_state::idle) {
			frame_image::get().render_transparent(
				dest_dc,
				now_pos_,
				{},
				(int)obj_type::item
			);
		}

		//충돌범위 테스트
		unit::render(dest_dc);
	}
}