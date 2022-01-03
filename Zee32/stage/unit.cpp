#include "unit.h"
#include "stage.h"

namespace zee {
	using namespace math;

	unit::unit() noexcept :
		size_()
		, now_pos_()
		, body_()
		, max_move_size_()
		, frame_x_()
		, frame_y_()
		, direction_()
		, is_dir_key_pressed()
		, pressed_key_()
		, delay()
		, shoot_type_()
		, hp_()
		, delay_shoot_()
		, delay_destroy_()  {
	}
	unit::~unit() noexcept {
	}

	void unit::init() {
		set_size(coords[unit_size]);
		set_now_pos_and_body(coords[unit_default_pos]);
		set_frame_size(coords[unit_default_frame]);
		set_max_move_size(coords[unit_max_move_size]);
		set_shoot_type((int)obj_shoot_type::straight);
		set_hp((int)obj_state::idle);
		set_delay(0.2f);
	}

	//stage에서 호출
	void unit::init_bullet(const int& shoot_type) {
		std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
		spawned_bullet->set_size(coords[unit_bullet_straight_size]);
		spawned_bullet->set_now_pos_and_body(
			{ now_pos_.x + size_.x / 2 - coords[unit_bullet_straight_size].x / 2
			, now_pos_.y + size_.y / 2 }
		);	//obj 위치
		spawned_bullet->set_max_move_size(coords[back_max_size]);
		spawned_bullet->set_frame_size(coords[unit_bullet_frame]);
		spawned_bullet->set_obj((int)obj_type::unit);
		spawned_bullet->set_move_type(shoot_type);
		bullets_.push_back(spawned_bullet);
	}

	void unit::tick(float delta_time) {
		move(delta_time);
		shoot(delta_time);
		destroy(delta_time);
	}

	const bool unit::in_screen() const {
		return now_pos_.x > coords[back_min_size].x && now_pos_.x < coords[back_max_size].x
			&& now_pos_.y > coords[back_min_size].y && now_pos_.y < coords[back_max_size].y;
	}

	void unit::move(const float delta_time) {

		is_dir_key_pressed = false;
		pressed_key_ = (int)key_type::none;

		if (key_state::is_down(keys::arrow_up) || key_state::is_down(keys::W)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type::arrow_up;
			direction_ = 0;
		}
		else if (key_state::is_down(keys::arrow_left) || key_state::is_down(keys::A)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type::arrow_left;
			direction_ = 1;
		}
		else if (key_state::is_down(keys::arrow_down) || key_state::is_down(keys::S)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type::arrow_down;
			direction_ = 2;
		}
		else if (key_state::is_down(keys::arrow_right) || key_state::is_down(keys::D)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type::arrow_right;
			direction_ = 3;
		}

		//이미지상 프레임 애니메이션
		if (is_dir_key_pressed) {
			//x축
			const float speed = 12.0f;
			const float frame = 2.0f;
			if (pressed_key_ == (int)key_type::arrow_left
				|| pressed_key_ == (int)key_type::arrow_right) {

				if (pressed_key_ == (int)key_type::arrow_left) {
					delay -= delta_time * speed;
				}
				else if (pressed_key_ == (int)key_type::arrow_right) {
					delay += delta_time * speed;
				}
				if (delay <= 0) {
					delay = 0.0f;
				}
				else if (delay >= frame) {
					delay = frame;
				}
				frame_x_ = { size_.x * (int)delay, 0 };
			}
			//y축
			/*
			if (pressed_key_ == (int)key_type::arrow_up 
				|| pressed_key_ == (int)key_type::arrow_down) {

				frame_per_time_ += delta_time * speed;
				frame_per_time_ = (float)math::fmod(frame_per_time_, frame);
				frame_y_ = { 0, size_.y * (int)frame_per_time_ };
			}
			*/

			//walk 이미지
			/*
			const float move_frame = 8.0f;
			frame_per_time_ = math::fmod(frame_per_time_, move_frame_x);
			frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			frame_y_ = { 0, size_.y * direction_ };
			*/
		}

		if (background_type_ == loop) {
			const float speed = 400.0f;
			if (is_dir_key_pressed) {
				//위치 이동
				switch (direction_) {
				case 0:
					if (now_pos_.y > 0) {
						set_now_pos_and_body({ now_pos_.x, now_pos_.y - delta_time * speed });
					}
					break;
				case 1:
					if (now_pos_.x > 0) {
						set_now_pos_and_body({ now_pos_.x - delta_time * speed, now_pos_.y });
					}
					break;
				case 2:
					if (now_pos_.y < max_move_size_.y) {
						set_now_pos_and_body({ now_pos_.x, now_pos_.y + delta_time * speed });
					}
					break;
				case 3:
					if (now_pos_.x < max_move_size_.x) {
						set_now_pos_and_body({ now_pos_.x + delta_time * speed, now_pos_.y });
					}
					break;
				}
			}
		}
	}

	void unit::shoot(const float delta_time) {
		//뷸렛 쏘기
		if (key_state::is_down(keys::space)) {

			const float frame = 0.2f;
			delay_shoot_ += delta_time;
			if (delay_shoot_ >= frame) {
				init_bullet((int)obj_shoot_type::straight);
			}
			delay_shoot_ = (float)math::fmod(delay_shoot_, frame);
		}

		//뷸렛 틱
		if (!bullets_.empty()) {
			for (auto& bullet_obj : bullets_) {
				bullet_obj->tick(delta_time);
			}
		}
	}

	void unit::destroy(const float delta_time) {
		if (hp_ == (int)obj_state::die) {
			//현재 반투명 상태 지연시간만 처리
			const float frame = 0.8f;
			delay_destroy_ += delta_time;
			if (delay_destroy_ >= frame) {
				hp_ = (int)obj_state::idle;
			}

			delay_destroy_ = (float)math::fmod(delay_destroy_, frame);
		}

		//뷸렛 제거
		for (int i = 0; i != bullets_.size(); ) {
			if (!(bullets_[i]->in_screen())) {
				bullets_.erase(bullets_.begin() + i);
			}
			else {
				i++;
			}
		}
	}

	void unit::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_destdc_to_backbuffer(dest_dc);
		if (hp_ == (int)obj_state::idle) {
			frame_image::get().render_transparent(
				dest_dc
				, now_pos_
				, frame_x_ + frame_y_
				, (int)obj_type::unit
			);
		}
		if (hp_ == (int)obj_state::die) {
			frame_image::get().render_alphablend(
				dest_dc
				, now_pos_
				, frame_x_ + frame_y_
				, (int)obj_type::unit
			);
		}

		//뷸렛 렌더
		for (auto& bullet_obj : bullets_) {
			bullet_obj->render(dest_dc);
		}

		//충돌범위 테스트
		shape::circlef circle{ body_.origin, body_.radius };
		if (key_state::is_toggle_on(keys::tab)) {
			dest_dc.circle(circle);
		}
	}

	const math::vec2f& unit::get_now_pos() const {
		return now_pos_;
	}
	const shape::circlef& unit::get_body() const {
		return body_;
	}
	const math::vec2i& unit::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& unit::get_frame_y() const {
		return frame_y_;
	}
	const int unit::get_direction() const {
		return direction_;
	}
	const int unit::get_shoot_type() const {
		return shoot_type_;
	}
	const int unit::get_hp() const {
		return hp_;
	}
	const bool unit::get_is_dir_key_pressed() const {
		return is_dir_key_pressed;
	}
	const std::vector<std::shared_ptr<bullet>> unit::get_bullets() const {
		return bullets_;
	}

	void unit::set_now_pos_and_body(const math::vec2f& point) {
		now_pos_ = point;
		set_body(now_pos_ + size_ / 2, (float)math::min(size_.x, size_.y) / 2);
	}
	void unit::set_body(const math::vec2f& origin, const float r) {
		body_.origin = origin;
		body_.radius = r;
	}
	void unit::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void unit::set_frame_size(const math::vec2i& size) {
		frame_x_ = { size.x, 0 };
		frame_y_ = { 0, size.y };
	}
	void unit::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void unit::set_shoot_type(const int i) {
		shoot_type_ = i;
	}
	void unit::set_hp(const int hp) {
		hp_ = hp;
	}
	void unit::set_delay(const float delay) {
		delay_shoot_ = delay;
		delay_destroy_ = delay;
	}
}