#pragma once
#include "../win32helper/win32helper.h"
#include <interfaces/tickable.h>
#include <vector>
#include "frame_image.h"

namespace zee {
	class bullet {
	public:
		bullet() noexcept;
		virtual ~bullet() noexcept;

		virtual void tick(float delta_time);
		const bool in_screen() const;
		void move(const float delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2i& get_size() const;
		const math::vec2f& get_now_pos() const;
		const shape::circlef& get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const int get_obj() const;
		const float get_homing_angle() const;
		const float get_circle_angle() const;
		const int get_move_type() const;
		const math::vec2f& get_vec_for_player() const;
		void set_size(const math::vec2i& size);
		void set_now_pos_and_body(const math::vec2f& point);
		void set_frame_size(const math::vec2i& size);
		void set_max_move_size(const math::vec2i& size);
		void set_obj(const int& obj);
		void set_homing_angle(const float angle);
		void set_circle_angle(const float angle);
		void set_move_type(const int i);
		void set_vec_for_player(const math::vec2f& v);

	private:
		void set_body(const math::vec2f& origin, const float& r);

		math::vec2i size_;
		math::vec2f now_pos_;
		shape::circlef body_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		float frame_per_time_;
		int obj_;	//ÃÑ¾Ë ½ð °´Ã¼
		float homing_angle_;
		float circle_angle_;
		int move_type_;
		math::vec2f vec_for_player_;
	};
}
