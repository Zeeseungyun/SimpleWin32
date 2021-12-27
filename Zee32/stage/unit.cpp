#include "unit.h"
#include "stage.h"

namespace zee {
	using namespace math;

	unit::unit() noexcept : size_({ 0, 0 })
		, max_move_size_({ 0, 0 })
		, frame_x_({ 0, 0 })
		, frame_y_({ 0, 0 })
		, now_pos_({ 0, 0 })
		, direction_(0)
		, is_dir_key_pressed(false)
		, frame_per_time_(0.0f) {
		
		frame_image::get().load_frame_image({ 1152, 2048 }, { 64, 64 }, TEXT("assets/walk.bmp"), (int)frame_image_index::player);
	}
	unit::~unit() noexcept {
	}

	void unit::tick(float delta_time) {
		move(delta_time);
	}

	void unit::move(const float& delta_time) {

		is_dir_key_pressed = false;
		if (key_state::is_down(keys::W)) {
			is_dir_key_pressed = true;
			direction_ = 0;
		}
		else if (key_state::is_down(keys::A)) {
			is_dir_key_pressed = true;
			direction_ = 1;
		}
		else if (key_state::is_down(keys::S)) {
			is_dir_key_pressed = true;
			direction_ = 2;
		}
		else if (key_state::is_down(keys::D)) {
			is_dir_key_pressed = true;
			direction_ = 3;
		}
		if (key_state::is_down(keys::space)) {
			static float delay = 0.3f;
			delay += delta_time;
			if (delay >= 0.3f) {
				std::shared_ptr<bullet> spawned_bullet = std::make_shared<bullet>();

				spawned_bullet->set_max_move_size({ 1152, 2048 });
				spawned_bullet->set_size({ 14, 14 });
				spawned_bullet->set_now_pos(now_pos_ + size_ / 2);
				spawned_bullet->set_direction(0);
				bullets_.push_back(spawned_bullet);
				
				static const int max_bullet_num = 20;
				if (bullets_.size() >= max_bullet_num) {
					bullets_.erase(bullets_.begin());
				}

				delay = 0.0f;
			}
		}
		
		for (auto& bullet_obj : bullets_) {
			bullet_obj->tick(delta_time);
		}


		if (is_dir_key_pressed) {
			//이미지상 프레임 애니메이션
			static const float frame_time = 15.0f;
			static const float move_frame = 8.0f;
			frame_per_time_ += delta_time * frame_time;
			//mod == %	//fmod == floating %
			frame_per_time_ = math::fmod(frame_per_time_, move_frame);
			frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			frame_y_ = { 0, size_.y * direction_ };
		}

		if (kind_of_background != scroll) {
			static const float speed = 8.0f;
			if (is_dir_key_pressed) {
				//위치 이동
				switch (direction_) {
				case 0:
					if (now_pos_.y > 0) {
						now_pos_.y -= speed;
					}
					break;
				case 1:
					if (now_pos_.x > 0) {
						now_pos_.x -= speed;
					}
					break;
				case 2:
					if (now_pos_.y < max_move_size_.y) {
						now_pos_.y += speed;
					}
					break;
				case 3:
					if (now_pos_.x < max_move_size_.x) {
						now_pos_.x += speed;
					}
					break;
				}
			}
		}
	}

	void unit::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_destdc_to_backbuffer(dest_dc);
		frame_image::get().render_alphablend(
			dest_dc
			, now_pos_
			, frame_x_
			, frame_y_
			, 0
		);

		for (auto& bullet_obj : bullets_) {
			bullet_obj->render(dest_dc);
		}
	}

	const math::vec2i& unit::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& unit::get_frame_y() const {
		return frame_y_;
	}
	const math::vec2f& unit::get_now_pos() const {
		return now_pos_;
	}
	const int& unit::get_direction_() const {
		return direction_;
	}
	const bool unit::get_is_pressed() const {
		return is_dir_key_pressed;
	}
	const std::vector<std::shared_ptr<bullet>> unit::get_bullets() const {
		return bullets_;
	}
	void unit::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void unit::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void unit::set_now_pos(const math::vec2f& now_pos) {
		now_pos_ = now_pos;
	}
}