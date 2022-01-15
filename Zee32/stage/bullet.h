#pragma once
#include "frame_image.h"
#include "projectile.h"

namespace zee {
	//Final specifier
	class bullet final : public projectile {
	public:
		bullet() noexcept = default;
		virtual ~bullet() noexcept = default;

		virtual void load_image();

		virtual void init() final;
		virtual void spawn_from(const int obj_type, const shape::circlef& obj_body) final;
		virtual void move(const float delta_time) final;
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) final;

		virtual void render(win32gdi::device_context_dynamic& dest_dc) final;

		int get_subj_type() const;
		float get_homing_angle() const;
		float get_homing_degree() const;
		float get_circle_angle() const;
		const math::vec2f& get_vec_for_player() const;

		void set_subj_type(const int& obj);
		void set_homing_angle(const float angle);
		void set_homing_degree(const float degree);
		void set_circle_angle(const float angle);
		void set_vec_for_player(const math::vec2f& v);

	private:

		enum const_var_ {
			player_bullet_straight_size,
			player_bullet_homing_size,
			player_bullet_frame,

			monster_bullet_straight_size,
			monster_bullet_circle_size,
			monster_bullet_homing_size,
			monster_bullet_arround_size,
			monster_bullet_wave_size,
			monster_bullet_frame,

			player_bullet_max_num = 20,
			monster_bullet_max_num = 60,
		};
		const std::vector<math::vec2i> coords_{
			{ 16, 16 },		//player_bullet_straight_size
			{ 13, 22 },		//player_bullet_homing_size
			{ 0, 0 },		//player_bullet_frame

			{ 16, 16 },		//monster_bullet_straight_size
			{ 26, 26 },		//monster_bullet_circle_size
			{ 32, 32 },		//monster_bullet_homing_size
			{ 16, 30 },		//monster_bullet_arround_size
			{ 30, 30 },		//monster_bullet_wave_size
			{ 0, 0 },			//monster_bullet_frame
		};

		float frame_per_time_;
		int subj_type_;	//ÃÑ¾Ë ½ð °´Ã¼
		float homing_angle_;
		float homing_degree_;
		float circle_angle_;
		float wave_angle_;
		math::vec2f vec_for_player_;
	};
}
