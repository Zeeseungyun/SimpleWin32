#include "player.h"
#include "bullet.h"

namespace zee {
	using namespace math;

	void player::load_image() {
		frame_image::get().load_frame_image(unit::coords[back_max_size], coords_[player_size]
			, TEXT("assets/player.bmp"), (int)unit::obj_type::player_straight);
	}
	
	void player::init() {
		set_size(coords_[player_size]);
		set_now_pos_and_body(coords_[player_default_pos]);
		set_frame_size(coords_[player_default_frame]);
		set_max_move_size(coords_[player_max_move_size]);
		set_hp(1);
		set_atk(1);
		set_state((int)obj_state::idle);
		set_delay(0.2f);
		set_my_score(0);
	}

	void player::move(const float delta_time) {

		vec2f velocity;
		const float speed = 400.0f;
		is_dir_key_pressed = false;
		pressed_key_ = (int)key_type_::none;

		if (key_state::is_down(keys::arrow_up) || key_state::is_down(keys::W)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type_::arrow_up;
			direction_ = 0;

			velocity -= vec2f::constants::unit_y * delta_time * speed;
		}
		
		if (key_state::is_down(keys::arrow_left) || key_state::is_down(keys::A)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type_::arrow_left;
			direction_ = 1;
			
			velocity -= vec2f::constants::unit_x * delta_time * speed;

		}
		
		if (key_state::is_down(keys::arrow_down) || key_state::is_down(keys::S)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type_::arrow_down;
			direction_ = 2;

			velocity += vec2f::constants::unit_y * delta_time * speed;
		}
		
		if (key_state::is_down(keys::arrow_right) || key_state::is_down(keys::D)) {
			is_dir_key_pressed = true;
			pressed_key_ = (int)key_type_::arrow_right;
			direction_ = 3;//이넘으로 했으면 좋았을거같슴. 
			velocity += vec2f::constants::unit_x * delta_time * speed;
		}

		//이미지상 프레임 애니메이션
		if (is_dir_key_pressed) {
			//x축
			const float speed = 9.0f;
			const float frame = 2.0f;
			if (pressed_key_ == (int)key_type_::arrow_left
				|| pressed_key_ == (int)key_type_::arrow_right) {

				if (pressed_key_ == (int)key_type_::arrow_left) {
					delay_frame_ani -= delta_time * speed;
				}
				else if (pressed_key_ == (int)key_type_::arrow_right) {
					delay_frame_ani += delta_time * speed;
				}
				if (delay_frame_ani <= 0) {
					delay_frame_ani = 0.0f;
				}
				else if (delay_frame_ani >= frame) {
					delay_frame_ani = frame;
				}
				frame_x_ = { size_.x * (int)delay_frame_ani, 0 };
			}
			//y축 프레임 이동
			/*
			if (pressed_key_ == (int)key_type::arrow_up
				|| pressed_key_ == (int)key_type::arrow_down) {

				frame_per_time_ += delta_time * speed;
				frame_per_time_ = (float)math::fmod(frame_per_time_, frame);
				frame_y_ = { 0, size_.y * (int)frame_per_time_ };
			}
			*/

			//walk.bmp
			/*
			const float move_frame = 8.0f;
			frame_per_time_ = math::fmod(frame_per_time_, move_frame_x);
			frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			frame_y_ = { 0, size_.y * direction_ };
			*/
		}

		//if (background_type == loop) {}
		
		//위치 이동
		if (is_dir_key_pressed) {
			set_now_pos_and_body(velocity + now_pos_);
		}
	}

	void player::shoot(const float delta_time) {
		if (key_state::is_down(keys::space)) {

			const float frame = 0.2f;
			delay_shoot_ += delta_time;
			if (delay_shoot_ >= frame) {
				std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
				spawned_bullet->init_bullet(obj_type_, now_pos_, size_);
 				bullets_.push_back(spawned_bullet);
			}
 			delay_shoot_ = (float)math::fmod(delay_shoot_, frame);
		}

		//뷸렛 틱
		for (auto& bullet_obj : bullets_) {
			bullet_obj->move(delta_time);
			bullet_obj->destroy(delta_time);
		}
	}
	void player::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		unit::hit_from(other, delta_time);
	}

	void player::destroy(const float delta_time) {
		unit::destroy(delta_time);

		//리스폰
		if (key_state::is_down(keys::R)) {
			init();
		}

		//뷸렛이 밖에 있으면 제거 (추후 다른 함수로 빼기)
		for (int i = 0; i != bullets_.size(); ) {
			if (!(bullets_[i]->in_screen())) {
				bullets_.erase(bullets_.begin() + i);
			} else {
				i++;
			}
		}
	}
	void player::add_score(const int score) {
		my_score_ += score;
	}

	void player::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			//frame_image::get().render_destdc_to_backbuffer(dest_dc, (int)obj_type::player_straight);

			if (state_ == (int)obj_state::idle) {
				frame_image::get().render_transparent(
					dest_dc,
					now_pos_,
					frame_x_ + frame_y_,
					(int)obj_type::player_straight
				);
			}
			if (state_ == (int)obj_state::hit) {
				frame_image::get().render_alphablend(
					dest_dc,
					now_pos_,
					frame_x_ + frame_y_,
					(int)obj_type::player_straight
				);
			}
		}

		//뷸렛 렌더
		for (auto& bullet_obj : bullets_) {
 			bullet_obj->render(dest_dc);
		}

		//충돌범위 테스트
		unit::render(dest_dc);
	}

	const int player::get_direction() const {
		return direction_;
	}
	const bool player::get_is_dir_key_pressed() const {
		return is_dir_key_pressed;
	}
	const std::vector<std::shared_ptr<bullet>> player::get_bullets() const {
		return bullets_;
	}

	void player::set_delay(const float delay) {
		delay_frame_ani = 0.2f;
		delay_shoot_ = delay;
		delay_destroy_ = delay;
	}
}