#include "unit.h"

namespace zee {
	using namespace math;

	void unit::init() {
	}

	void unit::load_image() {
	}

	const bool unit::in_screen() const {
		return now_pos_.x > coords[back_min_size].x && now_pos_.x < coords[back_max_size].x
			&& now_pos_.y > coords[back_min_size].y && now_pos_.y < coords[back_max_size].y;
	}

	void unit::move(const float delta_time) {
	}
	void unit::hit_from(std::shared_ptr<unit> other, const float delta_time) {
		hp_ -= other->atk_;

		//점멸 등 처리 위함
		if (state_ == (int)obj_state::hit && hp_ > 0) {
			const float frame = 0.8f;
			delay_hit_ += delta_time;
			if (delay_hit_ >= frame) {
				state_ = (int)obj_state::idle;
			}
		}

		if (hp_ <= 0) {
			//파괴
			destroy(delta_time);
		}
	}

	void unit::destroy(const float delta_time) {
		if (hp_ <= 0 || !(in_screen())) {
			hp_ = 0;
			state_ = (int)obj_state::die;
			set_now_pos_and_body(coords[back_max_size]);
		}
	}

	void unit::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			//충돌범위 테스트
			shape::circlef circle{ body_.origin, body_.radius };
			if (key_state::is_toggle_on(keys::tab)) {
				dest_dc.circle(circle);
			}
		}
	}

	const math::vec2i& unit::get_size() const {
		return size_;
	}
	const math::vec2f& unit::get_now_pos() const {
		return now_pos_;
	}
	const shape::circlef& unit::get_body() const {
		return body_;
	}
	const math::vec2i& unit::get_frame_x() const {
		return frame_x_;
	}
	const math::vec2i& unit::get_frame_y() const {
		return frame_y_;
	}
	const int unit::get_obj_type() const {
		return obj_type_;
	}
	const int unit::get_hp() const {
		return hp_;
	}
	const int unit::get_state() const {
		return state_;
	}
	const int unit::get_my_score() const {
		return my_score_;
	}

	void unit::set_now_pos_and_body(const math::vec2f& point) {
		now_pos_ = point;
		set_body(now_pos_ + size_ / 2, (float)math::min(size_.x, size_.y) / 2);
	}
	void unit::set_body(const math::vec2f& origin, const float r) {
		body_.origin = origin;
		body_.radius = r;
	}
	void unit::set_size(const math::vec2i& size) {
		size_ = size;
	}
	void unit::set_frame_size(const math::vec2i& size) {
		frame_x_ = { size.x, 0 };
		frame_y_ = { 0, size.y };
	}
	void unit::set_max_move_size(const math::vec2i& size) {
		max_move_size_ = size;
	}
	void unit::set_obj_type(const int i) {
		obj_type_ = i;
	}
	void unit::set_hp(const int hp) {
		hp_ = hp;
	}
	void unit::set_atk(const int atk) {
		atk_ = atk;
	}
	void unit::set_state(const int state) {
		state_ = state;
	}
	void unit::set_my_score(const int score) {
		my_score_ = score;
	}
}