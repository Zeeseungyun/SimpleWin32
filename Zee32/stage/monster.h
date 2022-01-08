#pragma once
#include "unit.h"
#include "frame_image.h"
#include "bullet.h"

class player;

namespace zee {
	class monster : public unit {
	public:
		monster() noexcept = default;
		virtual ~monster() noexcept = default;

		virtual void load_image() override;
		virtual void init() override;

		virtual void move(const float delta_time) override;
		void shoot(const float delta_time);
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) override;
		virtual void destroy(const float delta_time) override;
		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;

		const math::vec2f& get_arrival_vec() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		const math::vec2f& get_vec_for_player() const;
		void set_arrival_vec(const math::vec2f& vec);
		void set_vec_for_player(const math::vec2f& v);
		void set_delay(const float delay);

	private:

		enum const_var_ {
			monster_straight_size,
			monster_circle_size,
			monster_homing_size,
			monster_arround_size,
			monster_wave_size,
			monster_default_frame,
			monster_min_pos,
			monster_max_pos,
			monster_boss_pos,

			monster_bullet_max_num = 60,
		};
		const std::vector<math::vec2i> coords_{
			{64, 48},		//monster_straight_size
			{64, 64},		//monster_circle_size
			{60, 64},		//monster_homing_size
			{52, 62},		//monster_arround_size
			{126, 62},		//monster_wave_size
			{0, 0},			//monster_default_frame
			{0, -100},		//monster_min_pos
			{500, 900},		//monster_max_pos
			{350, 200},		//monster_boss_pos
		};

		std::vector<std::shared_ptr<bullet>> bullets_;
		math::vec2f arrival_vec_;
		math::vec2f vec_for_player_;
		float homing_angle_;
		float delay_shoot;
		float delay_destroy_;
	};
}