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
		void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const math::vec2f& get_now_pos() const;
		const int& get_direction_() const;
		const bool get_is_pressed() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		void set_size(const math::vec2i& size);
		void set_max_move_size(const math::vec2i& size);
		void set_now_pos(const math::vec2f& now_pos);

	private:
		math::vec2i size_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		math::vec2f now_pos_;
		int direction_;
		bool is_dir_key_pressed;
		float frame_per_time_;
		
		std::vector<std::shared_ptr<bullet>> bullets_;
	};
}
