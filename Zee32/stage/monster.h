#pragma once
#include "unit.h"
#include "frame_image.h"

namespace zee {
	class monster : public interfaces::tickable {
	public:
		monster() noexcept;
		virtual ~monster() noexcept;

		virtual void tick(float delta_time) override;
		void move(const float& delta_time);
		void rotate(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2i get_frame_size() const;
		const math::vec2i get_now_pos() const;
		const math::vec2f& get_center_point() const;
		const float& get_angle() const;
		const shape::rectf& get_rect() const;
		void set_frame_size(const math::vec2i& size);
		void set_now_pos(const math::vec2f& point);
		void set_center_point(const math::vec2f& point);

	private:
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		shape::rectf rect;
		math::vec2f now_pos_;
		math::vec2f center_point_;
		float angle_;
		win32gdi::device_context_dynamic monster_image_;
	};
}