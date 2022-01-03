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
		, homing_angle_()
		, circle_angle_()
		, move_type_()
		, vec_for_player_()  {
	}
	bullet::~bullet() noexcept {
	}

	void bullet::tick(float delta_time) {
		move(delta_time);
	}

	const bool bullet::in_screen() const {
		return now_pos_.x > coords[back_min_size].x && now_pos_.x < coords[back_max_size].x
			&& now_pos_.y > coords[back_min_size].y && now_pos_.y < coords[back_max_size].y;
	}

	void bullet::move(const float delta_time) {
		float speed;
		switch (move_type_)
		{
		case (int)obj_shoot_type::straight: {
			if (obj_ == (int)obj_type::unit) {
				speed = 500.0f;
				set_now_pos_and_body({ now_pos_.x, now_pos_.y - delta_time * speed });
			}
			else {
				speed = 200.0f;
				set_now_pos_and_body({ now_pos_.x, now_pos_.y + delta_time * speed });
			}
			break;
		}
		case (int)obj_shoot_type::circle: {
			speed = 80.0f;
			//각도별 힘
			float cosx = cos(circle_angle_);
			float siny = sin(circle_angle_);

			set_now_pos_and_body({ now_pos_.x + cosx * delta_time * speed
				, now_pos_.y + siny * delta_time * speed });
			break;
		}
		case (int)obj_shoot_type::homing: {
			//유도탄 이동
			//stage tick 에서 얻은 vec
			speed = 150.0f;
			set_now_pos_and_body({ now_pos_.x + vec_for_player_.x * delta_time * speed,
				now_pos_.y + vec_for_player_.y * delta_time * speed });

			//유도탄 회전각
			if (move_type_ == (int)obj_shoot_type::homing) {
				homing_angle_ = math::atan2(vec_for_player_.x, vec_for_player_.y);
			}
			break;
		}
		case (int)obj_shoot_type::arround: {
			speed = 400.0f;
			set_now_pos_and_body({ now_pos_.x, now_pos_.y + delta_time * speed });
			break;
		}
		case (int)obj_shoot_type::wave: {
			speed = 150.0f;
			//각도별 힘
			float cosx = cos(circle_angle_);
			float siny = sin(circle_angle_);
			set_now_pos_and_body({ now_pos_.x + cosx * delta_time * speed
				, now_pos_.y + siny * delta_time * speed });
			break;
		}//case
		}//switch
	}

	void bullet::render(win32gdi::device_context_dynamic& dest_dc) {

		frame_image::get().render_destdc_to_backbuffer(dest_dc);

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
		else {
			switch (move_type_)
			{
			case (int)obj_shoot_type::straight: {
				//직선탄
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::monster_bullet_straight
				);
				break;
			}
			case (int)obj_shoot_type::circle: {
				//원형탄
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::monster_bullet_circle
				);
				break;
			}
			case (int)obj_shoot_type::homing: {
				//유도탄
				frame_image::get().render_plg(
					dest_dc
					, body_.origin
					, homing_angle_
					, (int)obj_type::monster_bullet_homing
				);
				frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {});
				break;
			}
			case (int)obj_shoot_type::arround: {
				//어라운드
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::monster_bullet_arround
				);
				break;
			}
			case (int)obj_shoot_type::wave: {
				//웨이브
				frame_image::get().render_transparent(
					dest_dc
					, now_pos_
					, {}
					, (int)obj_type::monster_bullet_wave
				);
				break;
			}//case
			}//switch
		}


		//충돌범위 테스트
		shape::circlef circle{ body_.origin, body_.radius };
		if (key_state::is_toggle_on(keys::tab)) {
			dest_dc.circle(circle);
		}
	}

	const math::vec2i& bullet::get_size() const {
		return size_;
	}
	const math::vec2f& bullet::get_now_pos() const {
		return now_pos_;
	}
	const shape::circlef& bullet::get_body() const {
		return body_;
	}
	const math::vec2i& bullet::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& bullet::get_frame_y() const {
		return frame_y_;
	}
	const int bullet::get_obj() const {
		return obj_;
	}
	const float bullet::get_homing_angle() const {
		return homing_angle_;
	}
	const float bullet::get_circle_angle() const {
		return circle_angle_;
	}
	const int bullet::get_move_type() const {
		return move_type_;
	}
	const math::vec2f& bullet::get_vec_for_player() const {
		return vec_for_player_;
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
	void bullet::set_homing_angle(const float angle) {
		homing_angle_ = angle;
	}
	void bullet::set_circle_angle(const float angle) {
		circle_angle_ = angle;
	}
	void bullet::set_move_type(const int i) {
		move_type_ = i;
	}
	void bullet::set_vec_for_player(const math::vec2f& v) {
		vec_for_player_ = v;
	}
}