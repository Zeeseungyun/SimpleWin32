#pragma once
#include "../win32helper/win32helper.h"
#include "../application/key_state.h"
#include <interfaces/tickable.h>
#include <vector>
#include "frame_image.h"
#include "bullet.h"

namespace zee {
	class unit : public interfaces::tickable {
	public:
		unit() noexcept;
		virtual ~unit() noexcept;
		virtual void tick(float delta_time) override;
		void move(const float& delta_time);
		void shoot(const float& delta_time);
		void hit(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const shape::rectf get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const int& get_direction() const;
		const bool& get_is_pressed() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		const int& get_state() const;
		void set_size(const math::vec2i& size);
		void set_body(const math::vec2f& point);
		void set_frame_size(const math::vec2i& size);
		void set_max_move_size(const math::vec2i& size);
		void set_state(const int& state);

	private:
		math::vec2i size_;
		shape::rectf body_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		int direction_;
		bool is_dir_key_pressed;
		bool is_arrow_up_pressed;
		bool is_arrow_left_pressed;
		bool is_arrow_down_pressed;
		bool is_arrow_right_pressed;
		float frame_per_time_;

		std::vector<std::shared_ptr<bullet>> bullets_;
		int state_;
	};
}
