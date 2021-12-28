#include "monster.h"
#include "stage.h"

namespace zee {
	monster::monster() noexcept :
		size_()
		, body_()
		, frame_x_()
		, frame_y_()
		, center_point_()
		, angle_()
		, spawn_point_()
		, shoot_delay()  {
		frame_image::get().load_frame_image({ (int)back_loop_max_size_x, (int)back_loop_max_size_y }
			, { (int)monster_size_x, (int)monster_size_y }, TEXT("assets/monster.bmp"), (int)obj_type::monster);
	}
	monster::~monster() noexcept {
	}

	void monster::tick(float delta_time) {
		respawn(delta_time);
		move(delta_time);
		rotate(delta_time);
		shoot(delta_time);
	}

	void monster::respawn(const float& delta_time) {
		if (body_.data[0].x >= (int)back_loop_max_size_x || body_.data[0].y >= (int)back_loop_max_size_y
			|| body_.data[0].x <= (int)back_min_size_x || body_.data[0].y <= (int)back_min_size_y) {
			switch (spawn_point_)
			{
			case (int)obj_spawn::mon_left:
				body_.data[0].x = (int)monster_default_left_pos_x;
				body_.data[0].y = (int)monster_default_left_pos_y;
				break;
			case (int)obj_spawn::mon_middle:
				body_.data[0].x = (int)monster_default_middle_pos_x;
				body_.data[0].y = (int)monster_default_middle_pos_y;
				break;
			case (int)obj_spawn::mon_right:
				body_.data[0].x = (int)monster_default_right_pos_x;
				body_.data[0].y = (int)monster_default_right_pos_y;
				break;
			}
			body_.data[1].x = body_.data[0].x + size_.x;
			body_.data[1].y = body_.data[0].y + size_.y;
		}
	}

	void monster::move(const float& delta_time) {
		const float speed_1 = 1.0f;
		const float speed_2 = 2.0f;
		const float speed_3 = 3.0f;
		const float speed_4 = 0.5f;
		const float speed_5 = 0.7f;

		switch (spawn_point_)
		{
		case (int)obj_spawn::mon_left:
			body_.data[0].x += speed_1;
			body_.data[0].y += speed_4;
			body_.data[1].x += speed_1;
			body_.data[1].y += speed_4;
			break;
		case (int)obj_spawn::mon_middle:
			body_.data[0].y += speed_2;
			body_.data[1].y += speed_2;
			break;
		case (int)obj_spawn::mon_right:
			body_.data[0].x -= speed_3;
			body_.data[0].y += speed_5;
			body_.data[1].x -= speed_3;
			body_.data[1].y += speed_5;
			break;
		}
	}

	void monster::rotate(const float& delta_time) {
		angle_ += 0.1f;
		if (angle_ >= 360) {
			angle_ = 0.0f;
		}
	}

	void monster::shoot(const float& delta_time) {
		const float frame = 2.0f;
		shoot_delay += delta_time;
		if (shoot_delay >= frame) {
			std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();

			spawned_bullet->set_obj((int)obj_type::monster);
			spawned_bullet->set_max_move_size({ (int)back_loop_max_size_x, (int)back_loop_max_size_y });
			spawned_bullet->set_size({ (int)monster_bullet_size_x, (int)monster_bullet_size_y });
			spawned_bullet->set_body({ body_.data[0].x + size_.x / 2, body_.data[0].y + size_.y / 2 });
			spawned_bullet->set_frame_size({ (int)monster_bullet_frame_x, (int)monster_bullet_frame_y });
			bullets_.push_back(spawned_bullet);

			if (bullets_.size() >= monster_max_bullet_num) {
				bullets_.erase(bullets_.begin());
			}
		}

		shoot_delay = (float)fmod(shoot_delay, frame);

		for (auto& bullet_obj : bullets_) {
			bullet_obj->tick(delta_time);
		}
	}

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_destdc_to_backbuffer(dest_dc);
		//회전 테스트
		/*frame_image::get().render_plg(dest_dc, center_point_, angle_, (int)frame_image_index::monster);
		frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {});*/

		//몸체
		frame_image::get().render_transparent(
			dest_dc
			, body_.data[0]
			, {}
			, (int)obj_type::monster
		);

		//총알
		for (auto& bullet_obj : bullets_) {
			bullet_obj->render(dest_dc);
		}

		//충돌범위
		/*shape::rectf rect = {body_.data[0], body_.data[1]};
		dest_dc.rectangle(rect);*/
	}

	const shape::rectf& monster::get_body() const {
		return body_;
	}
	const math::vec2i& monster::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& monster::get_frame_y() const {
		return frame_y_;
	}
	const math::vec2f& monster::get_center_point() const {
		return center_point_;
	}
	const float& monster::get_angle() const {
		return angle_;
	}
	const std::vector<std::shared_ptr<bullet>> monster::get_bullets() const {
		return bullets_;
	}
	const int& monster::get_spawn_point() const {
		return spawn_point_;
	}
	void monster::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void monster::set_body(const math::vec2f& point) {
		body_.data[0] = point;
		body_.data[1] = body_.data[0] + size_;
		set_center_point(body_.data[0] + size_ / 2);
	}
	void monster::set_center_point(const math::vec2f& point) {
		center_point_ = point;
	}
	void monster::set_spawn_point(const int& i) {
		spawn_point_ = i;
	}
}