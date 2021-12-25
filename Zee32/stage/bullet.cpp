#include "bullet.h"

namespace zee {
	using namespace math;

	bullet::bullet() noexcept : 
		size_({ 0, 0 })
		, max_move_size_({ 0, 0 })
		, frame_x_({ 0, 0 })
		, frame_y_({ 0, 0 })
		, now_pos_({ 0, 0 })
		, direction_(0)
		, frame_per_time_(0.0f) {

	}
	bullet::~bullet() noexcept {
	}

	void bullet::tick(float delta_time) {
		if (now_pos_.x >= 800 || now_pos_.y >= 900) {
			
		}
		move(delta_time);
	}

	void bullet::move(const float& delta_time) {
		static const float speed = 8.0f;
		//위치 이동
		if (now_pos_.y > 0) {
			now_pos_.y -= speed;
		}
	}

	void bullet::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_transparent(dest_dc, now_pos_);
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
	const math::vec2f& bullet::get_now_pos() const {
		return now_pos_;
	}
	void bullet::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void bullet::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void bullet::set_now_pos(const math::vec2f& now_pos) {
		now_pos_ = now_pos;
	}
	void bullet::set_direction(const int& direction) {
		direction_ = direction;
	}
}