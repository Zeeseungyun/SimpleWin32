#include "player.h"
#include "bullet.h"

namespace zee {
	using namespace math;

	void player::load_image() {
		frame_image::get().load_frame_image(coords_[player_size]
			, TEXT("assets/player.bmp"), (int)obj_type::player_straight);
	}
	
	void player::init(const int obj_state) {
		plane::init(obj_state);
		set_size(coords_[player_size]);
		set_frame_size(coords_[player_default_frame]);
		spawn();

		//플레이어 뷸렛
		for (int i = 0; i != player_bullet_max_num; i++) {
			std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();
			spawned_bullet->init((int)obj_state::idle);
			bullets_.push_back(spawned_bullet);
		}
	}

	void player::spawn() {
		set_now_pos_and_body({ coords_[player_default_pos].x + get_body().radius,
			coords_[player_default_pos].y + get_body().radius });
		set_hp(1);
		set_state((int)obj_state::idle);
		set_delay(0.2f);
	}

	void player::move(const float delta_time) {

		vec2f velocity;
		const float speed = 400.0f;
		set_is_dir_key_pressed(false);
		set_pressed_key((int)key_type_::none);

		//키 입력 이벤트
		if (key_state::is_down(keys::arrow_up) || key_state::is_down(keys::W)) {
			set_is_dir_key_pressed(true);
			set_pressed_key((int)key_type_::arrow_up);
			set_direction((int)obj_dir::up);

			velocity -= vec2f::constants::unit_y * delta_time * speed;
		}
		
		if (key_state::is_down(keys::arrow_left) || key_state::is_down(keys::A)) {
			set_is_dir_key_pressed(true);
			set_pressed_key((int)key_type_::arrow_left);
			set_direction((int)obj_dir::left);
			
			velocity -= vec2f::constants::unit_x * delta_time * speed;

		}
		
		if (key_state::is_down(keys::arrow_down) || key_state::is_down(keys::S)) {
			set_is_dir_key_pressed(true);
			set_pressed_key((int)key_type_::arrow_down);
			set_direction((int)obj_dir::down);

			velocity += vec2f::constants::unit_y * delta_time * speed;
		}
		
		if (key_state::is_down(keys::arrow_right) || key_state::is_down(keys::D)) {
			set_is_dir_key_pressed(true);
			set_pressed_key((int)key_type_::arrow_right);
			set_direction((int)obj_dir::right);
			velocity += vec2f::constants::unit_x * delta_time * speed;
		}

		//이미지상 프레임 애니메이션
		if (get_is_dir_key_pressed()) {
			//x축
			const float speed = 9.0f;
			const float frame = 2.0f;
			if (get_pressed_key() == (int)key_type_::arrow_left
				|| get_pressed_key() == (int)key_type_::arrow_right) {

				if (get_pressed_key() == (int)key_type_::arrow_left) {
					set_delay_frame_ani(get_delay_frame_ani() - delta_time * speed);
				}
				else if (get_pressed_key() == (int)key_type_::arrow_right) {
					set_delay_frame_ani(get_delay_frame_ani() + delta_time * speed);
				}
				if (get_delay_frame_ani() <= 0) {
					set_delay_frame_ani(0.0f);
				}
				else if (get_delay_frame_ani() >= frame) {
					set_delay_frame_ani(frame);
				}
				set_frame_size({ get_size().x * (int)get_delay_frame_ani(), 0});
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

		//위치 이동
		//if (background_type == loop) {}
		if (get_is_dir_key_pressed()) {
			set_now_pos_and_body(velocity + get_now_pos());
		}


		//리스폰
		if (key_state::is_down(keys::R)) {
			set_my_score(0);
			spawn();
		}
	}

	void player::shoot(const float delta_time) {
		if (key_state::is_down(keys::space)) {

			const float frame = 0.2f;
			set_delay_shoot(get_delay_shoot() + delta_time);
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

		//뷸렛 틱
		for (auto& bullet_obj : get_bullets()) {
			bullet_obj->move(delta_time);
		}
	}
	void player::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		unit::hit_from(other, delta_time);
	}

	void player::destroy(const float delta_time) {
		//공통
		plane::destroy(delta_time);
	}
	void player::add_score(const int score) {
		set_my_score(get_my_score() + score);
	}

	void player::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			//frame_image::get().render_destdc_to_backbuffer(dest_dc);

			if (get_state() == (int)obj_state::idle) {
				frame_image::get().render_transparent(
					dest_dc,
					get_now_pos(),
					get_frame_x() + get_frame_y(),
					get_obj_type()
				);
			}
			if (get_state() == (int)obj_state::hit) {
				frame_image::get().render_alphablend(
					dest_dc,
					get_now_pos(),
					get_frame_x() + get_frame_y(),
					get_obj_type()
				);
			}
		}

		//플레이어 뷸렛
		for (auto& bullet_obj : get_bullets()) {
			bullet_obj->render(dest_dc);
		}

		//플레인 렌더
		plane::render(dest_dc);
	}

	const int player::get_direction() const {
		return direction_;
	}
	const bool player::get_is_dir_key_pressed() const {
		return is_dir_key_pressed_;
	}
	const int player::get_pressed_key() const {
		return pressed_key_;
	}
	const float player::get_delay_frame_ani() const {
		return delay_frame_ani_;
	}
	const float player::get_delay_shoot() const {
		return delay_shoot_;
	}
	const std::vector<std::shared_ptr<bullet>> player::get_bullets() const {
		return bullets_;
	}

	void player::set_now_pos_and_body(const math::vec2f& point) {
		now_pos_ = point;
		//player는 충돌 범위를 작게 함
		set_body(now_pos_ + size_ / 2, (float)math::min(size_.x, size_.y) / 2 - 15);
	}
	void player::set_direction(int direction) {
		direction_ = direction;
	}
	void player::set_delay(const float delay) {
		delay_frame_ani_ = 0.2f;
		delay_shoot_ = delay;
		delay_destroy_ = delay;
	}
	void player::set_is_dir_key_pressed(int is_dir_key_pressed) {
		is_dir_key_pressed_ = is_dir_key_pressed;
	}
	void player::set_pressed_key(int pressed_key) {
		pressed_key_ = pressed_key;
	}
	void player::set_delay_frame_ani(float delay_frame_ani) {
		delay_frame_ani_ = delay_frame_ani;
	}
	void player::set_delay_shoot(float delay_shoot) {
		delay_shoot_ = delay_shoot;
	}
}