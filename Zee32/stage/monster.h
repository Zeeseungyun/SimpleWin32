#pragma once
#include "unit.h"
#include "frame_image.h"

namespace zee {
	class monster {
	public:
		monster() noexcept;
		virtual ~monster() noexcept;

		void init();
		void init_bullet(const int& shoot_type);

		virtual void tick(float delta_time);
		const bool in_screen() const;
		void move(const float& delta_time);
		void shoot(const float& delta_time);
		void destroy(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2f& get_now_pos() const;
		const shape::circlef& get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const int& get_shoot_type() const;
		const math::vec2f& get_arrival_vec() const;
		const int& get_hp() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		const math::vec2f& get_vec_for_player() const;
		void set_size(const math::vec2i& size);
		void set_now_pos_and_body(const math::vec2f& point);
		void set_shoot_type(const int& i);
		void set_arrival_vec(const math::vec2f& vec);
		void set_hp(const int& hp);
		void set_vec_for_player(const math::vec2f& v);

	private:
		void set_body(const math::vec2f& origin, const float& r);

		math::vec2i size_;
		math::vec2f now_pos_;
		shape::circlef body_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;

		std::vector<std::shared_ptr<bullet>> bullets_;
		int shoot_type_;
		math::vec2f arrival_vec_;
		int hp_;
		math::vec2f vec_for_player_;
		float homing_angle_;
	};
}