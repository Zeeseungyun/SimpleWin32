#include "bullet.h"
#include "stage.h"
#include "monster.h"

namespace zee {
	using namespace math;

	bullet::bullet() noexcept :
		size_()
		, now_pos_()
		, body_()
		, max_move_size_()
		, frame_x_()
		, frame_y_()
		, frame_per_time_()
		, obj_()
		, follow_angle_()
		, circle_angle_()
		, move_type_()
		, vec_for_player_()
		, hp_()  {
	}
	bullet::~bullet() noexcept {
	}

	void bullet::tick(float delta_time) {
		move(delta_time);
		rotate();
		destroy(delta_time);
	}

	const bool bullet::in_screen() const {
		return now_pos_.x > coords[back_min_size].x && now_pos_.x < coords[back_loop_max_size].x
			&& now_pos_.y > coords[back_min_size].y && now_pos_.y < coords[back_loop_max_size].y;
	}

	void bullet::move(const float& delta_time) {
		if (hp_ == (int)obj_state::idle && in_screen()) {
			switch (move_type_)
			{
			case (int)obj_shoot_type::straight:
				move_straight();
				break;
			case (int)obj_shoot_type::circle:
				move_circle();
				break;
			case (int)obj_shoot_type::follow:
				move_follow();
				break;
			}
		}
	}
	void bullet::move_straight() {
		if (obj_ == (int)obj_type::unit) {
			const float speed = 5.0f;
			set_now_pos_and_body({ now_pos_.x, now_pos_.y - speed });
		}
		else if (obj_ = (int)obj_type::monster_1) {
			const float speed = 4.0f;
			set_now_pos_and_body({ now_pos_.x, now_pos_.y + speed });
		}
	}
	void bullet::move_circle() {
		const float speed = 2.0f;
		const int bullet_cnt = 6;

		//각도별 힘
		float cosx = cos(circle_angle_);
		float siny = sin(circle_angle_);

		set_now_pos_and_body({ now_pos_.x + cosx * speed
			, now_pos_.y + siny * speed });
	}

	void bullet::move_follow() {
		const float speed = 3.0f;
		set_now_pos_and_body(now_pos_ + vec_for_player_ * speed);
	}

	void bullet::rotate() {
		//유도탄 회전 각도
		if (move_type_ == (int)obj_shoot_type::follow) {
			follow_angle_ = math::atan2(vec_for_player_.x, vec_for_player_.y);
		}
	}

	void bullet::destroy(const float& delta_time) {
		if (!(in_screen())) {
			hp_ = (int)obj_state::die;
		}
		if (hp_ == (int)obj_state::die) {
			set_now_pos_and_body(coords[back_destroy_zone]);
		}
	}

	void bullet::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			//플레이어
			if (obj_ == (int)obj_type::unit) {
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::unit_bullet_straight
				);
			}
			//적
			//직선탄
			if (obj_ == (int)obj_type::monster_1
				&& move_type_ == (int)obj_shoot_type::straight) {
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::monster_bullet_straight
				);
			}
			//원형탄
			else if (obj_ == (int)obj_type::monster_2
				&& move_type_ == (int)obj_shoot_type::circle) {
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::monster_bullet_circle
				);
			}
			//유도탄
			else if (obj_ == (int)obj_type::monster_3
				&& move_type_ == (int)obj_shoot_type::follow) {
				frame_image::get().render_plg(
					dest_dc
					, body_.origin
					, follow_angle_
					, (int)obj_type::monster_bullet_follow
				);
				frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {});
			}

			//충돌범위 테스트
			if (in_screen()) {
				shape::circlef circle{ body_.origin, body_.radius };
				if (key_state::is_toggle_on(keys::tab)) {
					dest_dc.circle(circle);
				}
			}
		}

	}

	const math::vec2f bullet::get_now_pos() const {
		return now_pos_;
	}
	const shape::circlef bullet::get_body() const {
		return body_;
	}
	const math::vec2i& bullet::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& bullet::get_frame_y() const {
		return frame_y_;
	}
	const int& bullet::get_obj() const {
		return obj_;
	}
	const float& bullet::get_follow_angle() const {
		return follow_angle_;
	}
	const float& bullet::get_circle_angle() const {
		return circle_angle_;
	}
	const int& bullet::get_move_type() const {
		return move_type_;
	}
	const math::vec2f& bullet::get_vec_for_player() const {
		return vec_for_player_;
	}
	const int& bullet::get_hp() const {
		return hp_;
	}
	void bullet::set_now_pos_and_body(const math::vec2f& point) {
		now_pos_ = point;
		set_body(now_pos_ + size_ / 2, (float)math::min(size_.x, size_.y) / 2);
	}
	void bullet::set_body(const math::vec2f& origin, const float& r) {
		body_.origin = origin;
		body_.radius = r;
	}
	void bullet::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void bullet::set_frame_size(const math::vec2i& size) {
		frame_x_ = { size.x, 0 };
		frame_y_ = { 0, size.y };
	}
	void bullet::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void bullet::set_obj(const int& obj) {
		obj_ = obj;
	}
	void bullet::set_follow_angle(const float& angle) {
		follow_angle_ = angle;
	}
	void bullet::set_circle_angle(const float& angle) {
		circle_angle_ = angle;
	}
	void bullet::set_move_type(const int& i) {
		move_type_ = i;
	}
	void bullet::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
	void bullet::set_hp(const int& hp) {
		hp_ = hp;
	}
}