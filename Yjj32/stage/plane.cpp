#include "plane.h"

namespace yjj {

	void plane::init() {
		set_state((int)obj_state::idle);
		set_hp(1);
		set_atk(1);
	}

	void plane::spawn() {
		//°¢ Å¬·¡½º
	}


	void plane::shoot(const float delta_time) {
		//ºæ·¿ Æ½
		for (auto& bullet_obj : get_bullets()) {
			bullet_obj->move(delta_time);
		}
	}

	void plane::shoot_per_delay(const float frame) {
		if (get_delay_shoot() >= frame) {
			for (auto& bullet_obj : get_bullets()) {
				if (bullet_obj->get_state() == (int)obj_state::die) {
					bullet_obj->spawn_from(get_obj_type(), get_body());
					break;
				}
			}
		}
		set_delay_shoot((float)math::fmod(get_delay_shoot(), frame));
	}


	void plane::render(win32gdi::device_context_dynamic& dest_dc) {
		//ºæ·¿
		for (auto& bullet_obj : get_bullets()) {
			bullet_obj->render(dest_dc);
		}
	}



	const std::vector<std::shared_ptr<bullet>> plane::get_bullets() const {
		return bullets_;
	}
	float plane::get_delay_shoot() const {
		return delay_shoot_;
	}
	float plane::get_delay_destroy() const {
		return delay_destroy_;
	}
	float plane::get_speed() const {
		return speed_;
	}

	void plane::set_delay(const float delay) {
		//°¢ Å¬·¡½º
	}
	void plane::set_delay_shoot(const float delay_shoot) {
		delay_shoot_ = delay_shoot;
	}
	void plane::set_delay_destroy(const float delay) {
		delay_destroy_ = delay;
	}
	void plane::set_speed(const float speed) {
		speed_ = speed;
	}
}