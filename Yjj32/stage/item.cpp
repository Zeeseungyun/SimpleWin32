#include "item.h"
#include "player.h"

namespace yjj {
	void item::load_image() {
		frame_image::get().load_frame_image(coords_item_[item_size]
			, TEXT("assets/item_score.bmp"), (int)obj_type::item);
	}

	void item::init() {
		projectile::init();
		set_size(coords_item_[item_size]);
		set_my_score(3);
		set_obj_type((int)obj_type::item);
		set_now_pos_and_body(coords_item_[destroy_item_zone]);
	}

	void item::spawn_from(const std::shared_ptr<unit> other) {
		projectile::spawn_from(other);
	}

	void item::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//other이 player를 가리키고 nullptr이 아니면
		if (std::shared_ptr<player> other_player = std::dynamic_pointer_cast<player>(other)) {
			unit::hit_from(other, delta_time);
			other_player->add_score(get_my_score());
			item::destroy(delta_time);
			//sound::get().play_sound_effect(TEXT("assets/sound_effect/Pickup_Coin2.wav"));
		}
	}
	
	void item::destroy(const float delta_time) {
		unit::destroy(delta_time);
		set_now_pos_and_body(coords_item_[destroy_item_zone]);
	}

	void item::render(win32gdi::device_context_dynamic& dest_dc) {
		projectile::render(dest_dc);
	}
}