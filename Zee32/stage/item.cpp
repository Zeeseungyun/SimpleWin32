#include "item.h"
#include "player.h"

namespace zee {
	void item::load_image() {
		frame_image::get().load_frame_image(coords_[item_size]
			, TEXT("assets/item_score.bmp"), (int)obj_type::item);
	}

	void item::init(const int obj_state) {
		unit::init(obj_state);
		set_size(coords_[item_size]);
		set_my_score(3);
	}
	void item::spawn_from(const std::shared_ptr<unit> other) {
		projectile::spawn_from(other);
	}

	void item::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//other이 player를 가리키고 nullptr이 아니면
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			unit::hit_from(other, delta_time);
			other_player->add_score(get_my_score());
			//아이템은 먹는 즉시 파괴
			item::destroy(delta_time);
		}
	}
	
	void item::destroy(const float delta_time) {
		unit::destroy(delta_time);
	}

	void item::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen() && get_state() == (int)obj_state::idle) {
			frame_image::get().render_transparent(
				dest_dc,
				get_now_pos(),
				{},
				(int)obj_type::item
			);
		}

		//충돌범위 테스트
		unit::render(dest_dc);
	}
}