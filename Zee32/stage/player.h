#pragma once
#include "unit.h"
#include "frame_image.h"
#include "bullet.h"

namespace zee {
	class player : public unit {
	public:
		player() noexcept = default;
		virtual ~player() noexcept = default;

		virtual void load_image() override;

		virtual void init() override;

		virtual void move(const float delta_time) override;
		void shoot(const float delta_time);
		virtual void hit_from(std::shared_ptr<unit> other, const float delta_time) override;
		virtual void destroy(const float delta_time) override;
		void add_score(const int score);
		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;

		const int get_direction() const;
		const bool get_is_dir_key_pressed() const;
		const std::vector<std::shared_ptr<bullet>> get_bullets() const;
		void set_delay(const float delay);

	private:

		enum const_var_ {
			player_size,
			player_default_pos,
			player_default_frame,
			player_max_move_size,

			player_bullet_max_num = 20
		};
		const std::vector<math::vec2i> coords_ = {
			{ 64, 58 },		//player_size
			{ 350, 650 },	//player_default_pos
			{ 64, 0 },		//player_default_frame
			{ 660, 940 },	//player_max_move_size
		};

		enum class key_type_ {
			none,
			arrow_up,
			arrow_left,
			arrow_down,
			arrow_right,
			space
		};


		int direction_;
		bool is_dir_key_pressed;
		int pressed_key_;
		float delay_frame_ani;
		float delay_shoot_;
		float delay_destroy_;
		std::vector<std::shared_ptr<bullet>> bullets_;
	};
}
