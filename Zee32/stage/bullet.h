#pragma once
#include "../win32helper/win32helper.h"
#include <interfaces/tickable.h>
#include <vector>
#include "frame_image.h"

namespace zee {
	class bullet : public interfaces::tickable {
	public:
		bullet() noexcept;
		virtual ~bullet() noexcept;

		virtual void tick(float delta_time) override;
		void move_player(const float& delta_time);
		void move_monster(const float& delta_time);
		void shoot_straight();
		void shoot_circle();
		void shoot_follow();
		void hit(const float& delta_time);
		void render(win32gdi::device_context_dynamic& dest_dc);

		const shape::rectf& get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		const int& get_direction() const;
		const int& get_obj() const;
		const int& get_state() const;
		const math::vec2f& get_bomb_point() const;
		const float& get_angle() const;
		const int& get_move_type() const;
		const math::vec2f& get_vec_for_player() const;
		void set_size(const math::vec2i& size);
		void set_body(const math::vec2f& point);
		void set_frame_size(const math::vec2i& size);
		void set_max_move_size(const math::vec2i& size);
		void set_direction(const int& direction);
		void set_obj(const int& obj);
		void set_state(const int& state);
		void set_bomb_point(const math::vec2f& point);
		void set_angle(const float& angle);
		void set_move_type(const int& i);
		void set_vec_for_player(const math::vec2f& v);

	private:
		math::vec2i size_;
		shape::rectf body_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		int direction_;
		float frame_per_time_;
		int obj_;	//ÃÑ¾Ë ½ð °´Ã¼
		int state_;
		math::vec2i bomb_point_;
		float angle_;
		int move_type_;
		math::vec2f vec_for_player_;
	};
}
