#include "unit.h"

namespace zee {
	using namespace math;

	void unit::init(const int obj_state) {
		set_state(obj_state);
		set_now_pos_and_body(coords[back_destroy_zone]);
		switch (obj_state)
		{
		case (int)obj_state::idle:
			set_hp(1);
			break;
		case (int)obj_state::die:
			set_hp(0);
			break;
		}
	}

	void unit::load_image() {
		//각 클래스
	}

	const bool unit::in_screen() const {
		return get_now_pos().x > coords[back_min_size].x && get_now_pos().x < coords[back_max_size].x
			&& get_now_pos().y > coords[back_min_size].y && get_now_pos().y < coords[back_max_size].y;
	}

	void unit::move(const float delta_time) {
	}

	void unit::hit_from(const std::shared_ptr<unit> other, const float delta_time) {
		//피깎
		set_hp(get_hp() - other->get_atk());
	}

	void unit::destroy(const float delta_time) {
		set_hp(0);
		set_state((int)obj_state::die);
		set_now_pos_and_body(coords[back_destroy_zone]);
	}

	void unit::render(win32gdi::device_context_dynamic& dest_dc) {
		if (in_screen()) {
			//충돌범위 테스트
			shape::circlef circle{ get_body().origin, get_body().radius};
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
	int unit::get_obj_type() const {
		return obj_type_;
	}
	int unit::get_hp() const {
		return hp_;
	}
	int unit::get_atk() const {
		return atk_;
	}
	int unit::get_state() const {
		return state_;
	}
	int unit::get_my_score() const {
		return my_score_;
	}
	int unit::get_high_score() const {
		return high_score_;
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
	void unit::set_high_score(const int high_score) {
		high_score_ = high_score;
	}
}