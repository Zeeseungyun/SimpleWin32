#pragma once
#include "frame_image.h"
#include "plane.h"


namespace yjj {
	
	class player;

	class monster final : public plane {
	public:
		monster() noexcept = default;
		virtual ~monster() noexcept = default;

		virtual void load_image() final;

		virtual void init() final;
		virtual void spawn() final;
		virtual void move(const float delta_time) final;
		virtual void shoot(const float delta_time) final;
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) final;
		virtual void destroy(const float delta_time) final;

		virtual void render(win32gdi::device_context_dynamic& dest_dc) final;


		const math::vec2f& get_arrival_vec() const;
		const math::vec2f& get_vec_for_player() const;
		float get_homing_angle() const;
		float get_homing_degree() const;
		float get_wave_angle() const;
		float get_wave_angle_numer() const;

		void set_arrival_vec(const math::vec2f& vec);
		void set_vec_for_player(const math::vec2f& v);
		void set_homing_angle(const float angle);
		void set_homing_degree(const float degree);
		void set_wave_angle(const float angle);
		void set_wave_angle_numer(const float angle);
		virtual	void set_delay(const float delay) override;

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
			destroy_monster_zone,

			monster_bullet_max_num = 1000
		};
		const std::vector<math::vec2i> coords_monster_{
			{64, 48},		//monster_straight_size
			{64, 64},		//monster_circle_size
			{60, 64},		//monster_homing_size
			{62, 62},		//monster_arround_size
			{126, 62},		//monster_wave_size
			{0, 0},			//monster_default_frame
			{0, -100},		//monster_min_pos
			{500, 900},		//monster_max_pos
			{350, 200},		//monster_boss_pos
			{2200, 2200},	//destroy_monster_zone
		};

		math::vec2f arrival_vec_;
		math::vec2f vec_for_player_;
		float homing_angle_;
		float homing_degree_;
		float wave_angle_;
		float wave_angle_numer_;
	};
}