#pragma once
#include "frame_image.h"
#include "plane.h"

namespace zee {
	class player : public plane {
	public:
		player() noexcept = default;
		virtual ~player() noexcept = default;

		virtual void load_image() override;

		virtual void init() override;
		virtual void spawn() override;
		virtual void move(const float delta_time) override;
		virtual void shoot(const float delta_time) override;
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) override;
		virtual void destroy(const float delta_time) override;
		void add_score(const int score);

		virtual void render(win32gdi::device_context_dynamic& dest_dc) override;


		const int get_direction() const;
		const bool get_is_dir_key_pressed() const;
		const int get_pressed_key() const;
		const float get_delay_frame_ani() const;

		virtual void set_now_pos_and_body(const math::vec2f& point) override;
		void set_direction(int direction);
		void set_is_dir_key_pressed(int is_dir_key_pressed);
		void set_pressed_key(int pressed_key);
		void set_delay_frame_ani(float delay_frame_ani);
		virtual void set_delay(const float delay) override;

	private:
		enum const_var_ {
			player_size,
			player_default_pos,
			player_default_frame,
			player_max_move_size,

			player_bullet_max_num = 50
		};
		const std::vector<math::vec2i> coords_ = {
			{ 64, 58 },		//player_size
			{ 320, 650 },	//player_default_pos
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
		bool is_dir_key_pressed_;
		int pressed_key_;
		float delay_frame_ani_;
	};
}
