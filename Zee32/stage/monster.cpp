#include "monster.h"

namespace zee {
	monster::monster() noexcept :
		frame_x_({ 0, 0 })
		, frame_y_({ 0, 0 })
		, now_pos_({ 0, 0 })
		, rotate_point_({ 0, 0 })
		, angle_(0.0f) {
	}
	monster::~monster() noexcept {
	}

	void monster::tick(float delta_time) {
		rotate(delta_time);
	}

	void monster::rotate(const float& delta_time) {
		angle_ += 0.1f;
		if (angle_ >= 360) {
			angle_ = 0.0f;
		}
	}

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_background_to_backbuffer(dest_dc);
		frame_image::get().render_plg(dest_dc, { rotate_point_.x + 50, rotate_point_.y + 50 }, angle_);

		//회전 테스트
		for (int i = 100; i < 900; i += 100) {
			for (int j = 100; j < 800; j += 100) {
				frame_image::get().render_plg(dest_dc, { rotate_point_.x + i, rotate_point_.y + j }, -angle_);
			}
		}

		frame_image::get().render_transparent(dest_dc, {});
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
}