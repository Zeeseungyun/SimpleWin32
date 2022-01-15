#pragma once
#include "frame_image.h"
#include "plane.h"


namespace zee {
	
	class player;

	class monster : public plane {
	public:
		monster() noexcept = default;
		virtual ~monster() noexcept = default;

		virtual void load_image() override;

		virtual void init() override;
		virtual void spawn() override;
		virtual void move(const float delta_time) override;
		virtual void shoot(const float delta_time) override;
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) override;
		virtual void destroy(const float delta_time) override;

		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;


		const math::vec2f& get_arrival_vec() const;
		const math::vec2f& get_vec_for_player() const;
		const float get_homing_angle() const;
		const float get_homing_degree() const;
		const float get_delay_destroy() const;

		void set_arrival_vec(const math::vec2f& vec);
		void set_vec_for_player(const math::vec2f& v);
		void set_homing_angle(const float angle);
		void set_homing_degree(const float degree);
		void set_delay_destroy(const float delay);
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

			monster_bullet_max_num = 300
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

		math::vec2f arrival_vec_;
		math::vec2f vec_for_player_;
		float homing_angle_;
		float homing_degree_;
	};
}