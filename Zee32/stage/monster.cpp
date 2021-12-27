#include "monster.h"
#include "stage.h"

namespace zee {
	monster::monster() noexcept :
		frame_x_({ 0, 0 })
		, frame_y_({ 0, 0 })
		, now_pos_({ 0, 0 })
		, center_point_({ 0, 0 })
		, angle_(0.0f) {
		frame_image::get().load_frame_image({ 1152, 2048 }, { 48, 38 }, TEXT("assets/monster_1.bmp"), (int)frame_image_index::monster);
	}
	monster::~monster() noexcept {
	}

	void monster::tick(float delta_time) {
		move(delta_time);
		rotate(delta_time);
	}

	void monster::move(const float& delta_time) {
		rect.data[0] = now_pos_;
		rect.data[1] = now_pos_ + frame_x_ + frame_y_;
	}

	void monster::rotate(const float& delta_time) {
		angle_ += 0.1f;
		if (angle_ >= 360) {
			angle_ = 0.0f;
		}
	}

	void monster::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_destdc_to_backbuffer(dest_dc);
		frame_image::get().render_plg(dest_dc, center_point_, angle_, (int)frame_image_index::monster);
		frame_image::get().render_transparent_backbuffer_to_destdc(dest_dc, {});
	}

	const math::vec2i monster::get_frame_size() const {
		return frame_x_ + frame_y_;
	}
	const math::vec2i monster::get_now_pos() const {
		return now_pos_;
	}
	const math::vec2f& monster::get_center_point() const {
		return center_point_;
	}
	const float& monster::get_angle() const {
		return angle_;
	}
	const shape::rectf& monster::get_rect() const {
		return rect;
	}
	void monster::set_frame_size(const math::vec2i& size) {
		frame_x_ = { size.x, 0 };
		frame_y_ = { 0, size.y };
		rect.data[0] = { 0, 0 };
		rect.data[1] = size;
	}
	void monster::set_now_pos(const math::vec2f& point) {
		now_pos_ = point;
		set_center_point(now_pos_ + frame_x_ / 2 + frame_y_ / 2);
	}
	void monster::set_center_point(const math::vec2f& point) {
		center_point_ = point;
	}
}