#include "stage.h"

namespace zee {
	using namespace math;

	unit::unit() noexcept : size_({ 0, 0 }), max_move_size_({ 0, 0 })
		, frame_x_({ 0, 0 }), frame_y_({ 0, 0 }), direction_(0)
		, now_pos_({ 0, 0 }), is_pressed(false)
		, frame_per_time_(0.0f)
		, background_src_pos_({ 0.0f, 0.0f })
		, background_src_size_({ 0, 0 }) {

	}
	unit::~unit() noexcept {
	}

	void unit::tick(float delta_time) {
		switch (kind_of_background) {
		case loop: {
			static const float show_loop_time = 120.0f;
			switch (background_direction_) {
			//배경 루프 이미지
			//direction 이미지 기준 0: 좌->우.. 1: 우->좌.. 2: 상->하.. 3: 하->상
			case 0:
			case 1:
				background_src_pos_.x += delta_time * show_loop_time;
				background_src_pos_.x = (float)math::fmod(background_src_pos_.x, background_src_size_.x);
				break;
			case 2:
			case 3:
				background_src_pos_.y += delta_time * show_loop_time;
				background_src_pos_.y = (float)math::fmod(background_src_pos_.y, background_src_size_.y);
				break;
			}
			//유닛 이동
			move(delta_time);
			break;
		}

		case scroll: {
			static const int background_speed = 10;
			if (is_pressed) {
				switch (direction_) {
				//배경 정지 이미지
				case 0:
					if (now_pos_.y > 0 && background_src_pos_.y > 0) {
						background_src_pos_.y -= background_speed;
					}
					break;
				case 1:
					if (now_pos_.x > 0 && background_src_pos_.x > 0) {
						background_src_pos_.x -= background_speed;
					}
					break;
				case 2:
					if (now_pos_.y < 770 && background_src_pos_.y < 1152) {
						background_src_pos_.y += background_speed;
					}
					break;
				case 3:
					if (now_pos_.x < 705 && background_src_pos_.x < 390) {
						background_src_pos_.x += background_speed;
					}
					break;
				}
			}
			break;
		}//case
		}//switch (kind_of_background)

		set_frame_direction(delta_time);
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

		if (is_pressed) {
			//이미지상 프레임 애니메이션
			static const float frame_time = 15.0f;
			static const float move_frame = 8.0f;
			frame_per_time_ += delta_time * frame_time;
			//mod == %	//fmod == floating %
			frame_per_time_ = math::fmod(frame_per_time_, move_frame);
			frame_x_ = { size_.x * (int)frame_per_time_, 0 };
			frame_y_ = { 0, size_.y * direction_ };
		}
	}

	void unit::move(const float& delta_time) {
		static const float speed = 8.0f;
		if (is_pressed) {
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
	void unit::render(win32gdi::device_context_dynamic& dest_dc) {
		frame_image::get().render_alphablend(dest_dc
			, now_pos_
			, frame_x_
			, frame_y_
		);
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
	const math::vec2f& unit::get_now_pos() const {
		return now_pos_;
	}
	const bool unit::get_is_pressed() const {
		return is_pressed;
	}
	const math::vec2f& unit::get_background_src_pos() const {
		return background_src_pos_;
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
	void unit::set_background_src_pos(const math::vec2f& src_pos) {
		background_src_pos_ = src_pos;
	}
	void unit::set_background_src_size(const math::vec2f& size) {
		background_src_size_ = size;
	}
}