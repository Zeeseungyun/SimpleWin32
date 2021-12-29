#include "unit.h"
#include "stage.h"

namespace zee {
	using namespace math;

	unit::unit() noexcept :
		size_()
		, body_()
		, max_move_size_()
		, frame_x_()
		, frame_y_()
		, direction_()
		, is_dir_key_pressed()
		, is_arrow_up_pressed()
		, is_arrow_left_pressed()
		, is_arrow_down_pressed()
		, is_arrow_right_pressed()
		, frame_per_time_()
		, state_() {
		frame_image::get().load_frame_image({ (int)back_loop_max_size_x, (int)back_loop_max_size_y }
			, { (int)unit_size_x, (int)unit_size_y }, TEXT("assets/player.bmp"), (int)obj_type::player);
	}
	unit::~unit() noexcept {
	}

	void unit::tick(float delta_time) {
		move(delta_time);
		shoot(delta_time);
		hit(delta_time);
	}

	void unit::move(const float& delta_time) {

		is_dir_key_pressed = false;
		is_arrow_up_pressed = false;
		is_arrow_left_pressed = false;
		is_arrow_down_pressed = false;
		is_arrow_right_pressed = false;

		if (key_state::is_down(keys::arrow_up) || key_state::is_down(keys::W)) {
			is_dir_key_pressed = true;
			is_arrow_up_pressed = true;
			direction_ = 0;
		}
		else if (key_state::is_down(keys::arrow_left) || key_state::is_down(keys::A)) {
			is_dir_key_pressed = true;
			is_arrow_left_pressed = true;
			direction_ = 1;
		}
		else if (key_state::is_down(keys::arrow_down) || key_state::is_down(keys::S)) {
			is_dir_key_pressed = true;
			is_arrow_down_pressed = true;
			direction_ = 2;
		}
		else if (key_state::is_down(keys::arrow_right) || key_state::is_down(keys::D)) {
			is_dir_key_pressed = true;
			is_arrow_right_pressed = true;
			direction_ = 3;
		}

		//이미지상 프레임 애니메이션
		if (is_dir_key_pressed) {
			//x축 이동
			const float speed = 15.0f;
			if (is_arrow_left_pressed || is_arrow_right_pressed) {
				if (is_arrow_left_pressed) {
					frame_per_time_ -= delta_time * speed;
				}
				else if (is_arrow_right_pressed) {
					frame_per_time_ += delta_time * speed;
				}
				if (frame_per_time_ <= 0) {
					frame_per_time_ = 0.0f;
				}
				else if (frame_per_time_ >= 4.0f) {
					frame_per_time_ = 4.0f;
				}
				frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			}
			//y축 이동
			if (is_arrow_up_pressed || is_arrow_down_pressed) {
				const float frame = 2.0f;
				frame_per_time_ += delta_time * speed;
				frame_per_time_ = (float)math::fmod(frame_per_time_, frame);
				frame_y_ = { 0, size_.y * (int)frame_per_time_ };
			}

			//walk 이미지
			/*const float move_frame = 8.0f;
			frame_per_time_ = math::fmod(frame_per_time_, move_frame_x);
			frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			frame_y_ = { 0, size_.y * direction_ };*/
		}

		if (background_type_ != scroll) {
			const float speed = 8.0f;
			if (is_dir_key_pressed) {
				//위치 이동
				switch (direction_) {
				case 0:
					if (body_.data[0].y > 0) {
						body_.data[0].y -= speed;
						body_.data[1].y -= speed;
					}
					break;
				case 1:
					if (body_.data[0].x > 0) {
						body_.data[0].x -= speed;
						body_.data[1].x -= speed;
					}
					break;
				case 2:
					if (body_.data[0].y < max_move_size_.y) {
						body_.data[0].y += speed;
						body_.data[1].y += speed;
					}
					break;
				case 3:
					if (body_.data[0].x < max_move_size_.x) {
						body_.data[0].x += speed;
						body_.data[1].x += speed;
					}
					break;
				}
			}
		}
	}

	void unit::shoot(const float& delta_time) {
		//총알 쏘기
		if (key_state::is_down(keys::space)) {

			static float delay = 0.2f;
			const float frame = 0.2f;
			delay += delta_time;
			if (delay >= frame) {
				std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();

				spawned_bullet->set_obj((int)obj_type::player);
				spawned_bullet->set_max_move_size({ (int)back_scroll_max_size_x, (int)back_scroll_max_size_y });
				spawned_bullet->set_size({ (int)unit_bullet_size_x, (int)unit_bullet_size_y });
				spawned_bullet->set_body({ body_.data[0].x + size_.x / 2, body_.data[0].y + size_.y / 2 });
				spawned_bullet->set_frame_size({ (int)unit_bullet_frame_x, (int)unit_bullet_frame_y });
				spawned_bullet->set_direction(0);
				bullets_.push_back(spawned_bullet);

				while (bullets_.size() > unit_max_bullet_num) {
					bullets_.erase(bullets_.begin());
				}

				delay = (float)math::fmod(delay, frame);
			}
		}

		//총알 틱
		for (auto& bullet_obj : bullets_) {
			bullet_obj->tick(delta_time);
		}
	}

	void unit::hit(const float& delta_time) {
		if (state_ == (int)obj_state::hit) {
			static float delay = 0.0f;
			const float frame = 0.8f;
			delay += delta_time;
			if (delay >= frame) {
				state_ = (int)obj_state::idle;
			}

			delay = (float)math::fmod(delay, frame);
		}
	}

	void unit::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_destdc_to_backbuffer(dest_dc);
		if (state_ == (int)obj_state::idle) {
			frame_image::get().render_transparent(
				dest_dc
				, body_.data[0]
				, frame_x_ + frame_y_
				, (int)obj_type::player
			);
		}
		if (state_ == (int)obj_state::hit) {
			frame_image::get().render_alphablend(
				dest_dc
				, body_.data[0]
				, frame_x_ + frame_y_
				, (int)obj_type::player
			);
		}

		//총알
		for (auto& bullet_obj : bullets_) {
			bullet_obj->render(dest_dc);
		}
	}

	const shape::rectf unit::get_body() const {
		return body_;
	}
	const math::vec2i& unit::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& unit::get_frame_y() const {
		return frame_y_;
	}
	const int& unit::get_direction() const {
		return direction_;
	}
	const bool& unit::get_is_pressed() const {
		return is_dir_key_pressed;
	}
	const std::vector<std::shared_ptr<bullet>> unit::get_bullets() const {
		return bullets_;
	}
	const int& unit::get_state() const {
		return state_;
	}
	void unit::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void unit::set_body(const math::vec2f& point) {
		body_.data[0] = point;
		body_.data[1] = body_.data[0] + size_;
	}
	void unit::set_frame_size(const math::vec2i& size) {
		frame_x_ = { size.x, 0 };
		frame_y_ = { 0, size.y };
	}
	void unit::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void unit::set_state(const int& state) {
		state_ = state;
	}
}