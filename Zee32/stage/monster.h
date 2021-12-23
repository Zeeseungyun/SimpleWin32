#pragma once
#include "unit.h"

namespace zee {
	class monster : public interfaces::tickable {
	public:
		monster() noexcept;
		virtual ~monster() noexcept;

		virtual void tick(float delta_time) override;
		void move(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2f& get_rotate_point() const;
		const float& get_angle() const;
		void set_rotate_point(const math::vec2f& point);
		void set_rotate_origin(const math::vec2f& point);

	private:
		math::vec2f rotate_point_;
		math::vec2f rotate_origin_;
		float angle_;
	};
}