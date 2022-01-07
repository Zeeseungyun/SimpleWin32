#pragma once
#include "unit.h"
#include "frame_image.h"

namespace zee {
	class bullet : public unit {
	public:
		bullet() noexcept = default;
		virtual ~bullet() noexcept = default;

		virtual void load_image();
		void init_bullet(const int obj_type, const math::vec2f& now_pos, const math::vec2f& size);

		virtual void move(const float delta_time) override;
		virtual void hit_from(std::shared_ptr<unit> other, const float delta_time) override;
		virtual void destroy(const float delta_time) override;
		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;

		const int get_subj_type() const;
		const float get_homing_angle() const;
		const float get_circle_angle() const;
		const math::vec2f& get_vec_for_player() const;

		void set_subj_type(const int& obj);
		void set_homing_angle(const float angle);
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

			{16, 16},		//monster_bullet_straight_size
			{26, 26},		//monster_bullet_circle_size
			{24, 32},		//monster_bullet_homing_size
			{16, 30},		//monster_bullet_arround_size
			{30, 30},		//monster_bullet_wave_size
			{0, 0},			//monster_bullet_frame
		};

		float frame_per_time_;
		int subj_type_;	//ÃÑ¾Ë ½ð °´Ã¼
		float homing_angle_;
		float circle_angle_;
		math::vec2f vec_for_player_;
	};
}
