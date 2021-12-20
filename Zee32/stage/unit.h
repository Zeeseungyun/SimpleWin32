#pragma once
#include "../win32helper/win32helper.h"
#include "../application/key_state.h"

namespace zee {
	class unit {
	public:
		unit() noexcept;
		virtual ~unit() noexcept;
		void set_frame_direction(const float& delta_time);
		void move(const float& delta_time);

		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const int& get_direction() const;
		const math::vec2i& get_now_pos() const;
		const bool get_is_pressed() const;
		void set_size(const math::vec2i& size);
		void set_max_move_size(const math::vec2i& size);
		void set_now_pos(const math::vec2i& now_pos);

	private:
		math::vec2i size_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		int direction_;
		math::vec2i now_pos_;
		bool is_pressed;
		float frame_per_time_;
		const float move_frame_;
		const float speed_;
	};
}
