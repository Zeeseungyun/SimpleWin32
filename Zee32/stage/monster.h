#pragma once
#include "unit.h"
#include "frame_image.h"

namespace zee {
	class monster : public interfaces::tickable {
	public:
		monster() noexcept;
		virtual ~monster() noexcept;

		virtual void tick(float delta_time) override;
		void respawn(const float& delta_time);
		void move(const float& delta_time);
		void rotate(const float& delta_time);
		void shoot(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const shape::rectf& get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const math::vec2f& get_center_point() const;
		const float& get_angle() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		const int& get_spawn_point() const;
		void set_size(const math::vec2i& size);
		void set_body(const math::vec2f& point);
		void set_center_point(const math::vec2f& point);
		void set_spawn_point(const int& i);

	private:
		math::vec2i size_;
		shape::rectf body_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		math::vec2f center_point_;
		float angle_;

		std::vector<std::shared_ptr<bullet>> bullets_;
		int spawn_point_;
		float shoot_delay;
	};
}