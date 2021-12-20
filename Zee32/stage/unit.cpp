#include "unit.h"

namespace zee {
	unit::unit() noexcept : size_({ 0, 0 }), max_move_size_({ 0, 0 })
		, frame_x_({ 0, 0 }), frame_y_({ 0, 0 }), direction_(0)
		, now_pos_({ 0, 0 }), is_pressed(false)
		, frame_per_time_(0.0f), move_frame_(9.0f), speed_(8.0f) {
	}
	unit::~unit() noexcept {
	}

	void unit::set_frame_direction(const float& delta_time) {
		is_pressed = false;
		if (key_state::is_down(keys::W)) {
			is_pressed = true;
			direction_ = 0;
		}
		else if (key_state::is_down(keys::A)) {
			is_pressed = true;
			direction_ = 1;
		}
		else if (key_state::is_down(keys::S)) {
			is_pressed = true;
			direction_ = 2;
		}
		else if (key_state::is_down(keys::D)) {
			is_pressed = true;
			direction_ = 3;
		}

		if(is_pressed) {
			//이미지상 프레임 애니메이션
			static const float frame_time = 15.0f;;
			frame_per_time_ += delta_time * frame_time;
			//mod == %	//fmod == floating %
			frame_per_time_ = math::fmod(frame_per_time_, move_frame_);
			frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			frame_y_ = { 0, size_.y * direction_ };
		}
	}

	void unit::move(const float& delta_time) {
		if (is_pressed) {
			//위치 이동
			switch (direction_) {
			case 0:
				if (now_pos_.y > 0) {
					now_pos_.y -= (int)speed_;
				}
				break;
			case 1:
				if (now_pos_.x > 0) {
					now_pos_.x -= (int)speed_;
				}
				break;
			case 2:
				if (now_pos_.y < max_move_size_.y) {
					now_pos_.y += (int)speed_;
				}
				break;
			case 3:
				if (now_pos_.x < max_move_size_.x) {
					now_pos_.x += (int)speed_;
				}
				break;
			}
		}
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
	const math::vec2i& unit::get_now_pos() const {
		return now_pos_;
	}
	const bool unit::get_is_pressed() const {
		return is_pressed;
	}
	void unit::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void unit::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void unit::set_now_pos(const math::vec2i& now_pos) {
		now_pos_ = now_pos;
	}
}