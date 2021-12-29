#include "bullet.h"
#include "stage.h"
#include "monster.h"

namespace zee {
	using namespace math;

	bullet::bullet() noexcept :
		size_()
		, body_()
		, max_move_size_()
		, frame_x_()
		, frame_y_()
		, direction_()
		, frame_per_time_()
		, obj_()
		, state_()
		, bomb_point_()
		, angle_()
		, move_type_()
		, vec_for_player_() {
		//ºæ·¿ ÀÌ¹ÌÁö
		frame_image::get().load_frame_image({ (int)back_loop_max_size_x, (int)back_loop_max_size_y }
			, { (int)unit_bullet_size_y, (int)unit_bullet_size_y }, TEXT("assets/bullet.bmp"), (int)obj_type::bullet_player);
		frame_image::get().load_frame_image({ (int)back_loop_max_size_x, (int)back_loop_max_size_y }
			, { (int)unit_bullet_size_x, (int)unit_bullet_size_y }, TEXT("assets/bullet.bmp"), (int)obj_type::bullet_monster);
		//Æø¹ß ÀÌ¹ÌÁö
		frame_image::get().load_frame_image({ (int)back_loop_max_size_x, (int)back_loop_max_size_y }
			, { (int)effect_bomb_size_x, (int)effect_bomb_size_y }, TEXT("assets/bomb.bmp"), (int)obj_type::bomb);
	}
	bullet::~bullet() noexcept {
	}

	void bullet::tick(float delta_time) {
		if (body_.data[0].y > (int)back_min_size_y && body_.data[0].y < (int)back_loop_max_size_y) {
			if (obj_ == (int)obj_type::player) {
				move_player(delta_time);
			}
			if (obj_ == (int)obj_type::monster) {
				move_monster(delta_time);
			}
		}
		hit(delta_time);
	}

	//µð¹ö±ë À§ÇØ move player / monster ºÐ¸®
	void bullet::move_player(const float& delta_time) {
		switch (move_type_)
		{
		case (int)obj_shoot_type::straight:
			shoot_straight();
			break;
		case (int)obj_shoot_type::circle:
			shoot_circle();
			break;
		case (int)obj_shoot_type::follow:
			shoot_follow();
			break;
		}
	}
	void bullet::move_monster(const float& delta_time) {
		switch (move_type_)
		{
		case (int)obj_shoot_type::straight:
			shoot_straight();
			break;
		case (int)obj_shoot_type::circle:
			shoot_circle();
			break;
		case (int)obj_shoot_type::follow:
			shoot_follow();
			break;
		}
	}
	void bullet::shoot_straight() {
		if (obj_ == (int)obj_type::player) {
			const float speed = 5.0f;
			body_.data[0].y -= speed;
			body_.data[1].y -= speed;
		}
		else if (obj_ = (int)obj_type::monster) {
			const float speed = 3.0f;
			body_.data[0].y += speed;
			body_.data[1].y += speed;
		}
	}
	void bullet::shoot_circle() {
		const float speed = 2.0f;
		const int bullet_cnt = 6;

		//°¢µµ¿¡ µû¸¥ Èû ¹æÇâ
		float cosx = cos(angle_);
		float siny = sin(angle_);

		body_.data[0].x += cosx * speed;
		body_.data[0].y += siny * speed;
		body_.data[1].x += cosx * speed;
		body_.data[1].y += siny * speed;
	}

	void bullet::shoot_follow() {
		const float speed = 3.0f;
		body_.data[0] += vec_for_player_ * speed;
		body_.data[1] += vec_for_player_ * speed;
	}

	void bullet::hit(const float& delta_time) {
		if (state_ == (int)obj_state::hit) {
			static float delay = 0.0f;
			const float speed = 3.0f;
			const float frame = 1.0f;
			const float frame_final = 4.0f;
			delay += delta_time * speed;
 			if (delay >= frame) {
 				frame_x_ = { (int)effect_bomb_size_x * (int)delay, 0 };
			}
			if (delay >= frame_final) {
				state_ = (int)obj_state::idle;
				bomb_point_ = body_.data[0];
			}

			delay = (float)math::fmod(delay, frame_final);
			frame_x_.x = math::fmod(frame_x_.x, (int)effect_bomb_final_frame_x);
		}
	}

	void bullet::render(win32gdi::device_context_dynamic& dest_dc) {
		if (body_.data[0].y > 0 && body_.data[0].y < (int)back_scroll_max_size_y) {
			if (state_ == (int)obj_state::idle) {
				if (obj_ == (int)obj_type::player) {
					frame_image::get().render_transparent(
						dest_dc
						, body_.data[0]
						, { (int)monster_bullet_next_frame_x, 0 }
						, (int)obj_type::bullet_player
					);
				}
				else if (obj_ == (int)obj_type::monster) {
					frame_image::get().render_transparent(
						dest_dc
						, body_.data[0]
						, {}
						, (int)obj_type::bullet_monster
					);
				}
			}

			if (state_ == (int)obj_state::hit) {
				frame_image::get().render_transparent(
					dest_dc
					, bomb_point_
					, frame_x_ + frame_y_
					, (int)obj_type::bomb
				);
			}
		}
	}

	const shape::rectf& bullet::get_body() const {
		return body_;
	}
	const math::vec2i& bullet::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& bullet::get_frame_y() const {
		return frame_y_;
	}
	const int& bullet::get_direction() const {
		return direction_;
	}
	const int& bullet::get_obj() const {
		return obj_;
	}
	const int& bullet::get_state() const {
		return state_;
	}
	const math::vec2f& bullet::get_bomb_point() const {
		return bomb_point_;
	}
	const float& bullet::get_angle() const {
		return angle_;
	}
	const int& bullet::get_move_type() const {
		return move_type_;
	}
	const math::vec2f& bullet::get_vec_for_player() const {
		return vec_for_player_;
	}
	void bullet::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void bullet::set_body(const math::vec2f& point) {
		body_.data[0] = point;
		body_.data[1] = body_.data[0] + size_;
	}
	void bullet::set_frame_size(const math::vec2i& size) {
		frame_x_ = { size.x, 0 };
		frame_y_ = { 0, size.y };
	}
	void bullet::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void bullet::set_direction(const int& direction) {
		direction_ = direction;
	}
	void bullet::set_obj(const int& obj) {
		obj_ = obj;
	}
	void bullet::set_state(const int& state) {
		state_ = state;
	}
	void bullet::set_bomb_point(const math::vec2f& point) {
		bomb_point_ = point;
	}
	void bullet::set_angle(const float& angle) {
		angle_ = angle;
	}
	void bullet::set_move_type(const int& i) {
		move_type_ = i;
	}
	void bullet::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
}