#pragma once
#include "unit.h"
#include "frame_image.h"

namespace zee {
	class monster : public interfaces::tickable {
	public:
		monster() noexcept;
		virtual ~monster() noexcept;

		virtual void tick(float delta_time) override;
		const bool in_screen() const;
		void move(const float& delta_time);
		void rotate(const float& delta_time);
		void shoot(const float& delta_time);
		void hit_to_idle(const float& delta_time);
		void destroy(const float& delta_time);
		void spawn(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		void init_bullet(const int& shoot_type);

		const math::vec2f get_now_pos() const;
		const shape::circlef get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const float& get_angle() const;
		const int& get_shoot_type() const;
		const int& get_state() const;
		const math::vec2f& get_bomb_point() const;
		const math::vec2f& get_arrival_point() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		void set_size(const math::vec2i& size);
		void set_now_pos_and_body(const math::vec2f& point);
		void set_shoot_type(const int& i);
		void set_state(const int& state);
		void set_bomb_point(const math::vec2f& point);
		void set_arrival_point(const math::vec2f& point);

	private:
		void set_body(const math::vec2f& origin, const float& r);

		math::vec2i size_;
		math::vec2f now_pos_;
		shape::circlef body_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		float angle_;

		std::vector<std::shared_ptr<bullet>> bullets_;
		int shoot_type_;
		int state_;
		math::vec2f bomb_point_;
		math::vec2f arrival_point_;
	};
}