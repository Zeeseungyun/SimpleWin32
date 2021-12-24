#include "monster.h"

namespace zee {
	monster::monster() noexcept : rotate_origin_({ 0, 0 }), angle_(0.0f), rotate_point_({ 0, 0 }) {
	}
	monster::~monster() noexcept {
	}

	void monster::tick(float delta_time) {
		move(delta_time);
	}

	void monster::move(const float& delta_time) {
		angle_ += 0.1f;
		if (angle_ >= 360) {
			angle_ = 0.0f;
		}
	}

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		//회전 테스트
		frame_image::get().render_rotate(dest_dc, rotate_point_, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 100, rotate_point_.y }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 300, rotate_point_.y }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 500, rotate_point_.y }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 700, rotate_point_.y }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 100, rotate_point_.y + 200}, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 300, rotate_point_.y + 200 }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 500, rotate_point_.y + 200 }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 700, rotate_point_.y + 200 }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 100, rotate_point_.y + 400 }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 300, rotate_point_.y + 400 }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 500, rotate_point_.y + 400 }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 700, rotate_point_.y + 400 }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 100, rotate_point_.y + 600 }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 300, rotate_point_.y + 600 }, -angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 500, rotate_point_.y + 600 }, angle_);
		frame_image::get().render_rotate(dest_dc, { rotate_point_.x + 700, rotate_point_.y + 600 }, -angle_);
	}

	const math::vec2f& monster::get_rotate_point() const {
		return rotate_point_;
	}
	const float& monster::get_angle() const {
		return angle_;
	}
	void monster::set_rotate_point(const math::vec2f& point) {
		rotate_point_ = point;
	}
	void monster::set_rotate_origin(const math::vec2f& point) {
		rotate_origin_ = point;
	}
}