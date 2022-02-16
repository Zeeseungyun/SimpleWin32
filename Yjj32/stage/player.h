#pragma once
#include "frame_image.h"
#include "plane.h"

namespace yjj {
	class player final : public plane {
	public:
		player() noexcept = default;
		virtual ~player() noexcept = default;

		virtual void load_image() final;

		virtual void init() final;
		virtual void spawn() final;
		virtual void move(const float delta_time) final;
		virtual void shoot(const float delta_time) final;
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time) final;
		virtual void destroy(const float delta_time) final;
		void add_score(const int score);

		virtual void render(win32gdi::device_context_dynamic& dest_dc) final;


		int get_direction() const;
		bool get_is_dir_key_pressed() const;
		int get_pressed_key() const;
		float get_delay_frame_ani() const;

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
			destroy_player_zone,

			player_bullet_max_num = 50
		};
		const std::vector<math::vec2i> coords_player_ = {
			{64, 58},		//player_size
			{320, 650},		//player_default_pos
			{64, 0},		//player_default_frame
			{660, 940},		//player_max_move_size
			{2000, 2000},	//destroy_player_zone
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
