#pragma once
#include <interfaces/tickable.h>
#include <vector>
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include "../application/key_state.h"
#include "frame_image.h"
#include "../sound/sound.h"

namespace yjj {
	enum class obj_type {
		//
		player_straight,
		//
		monster_straight,
		monster_circle,
		monster_homing,
		monster_arround,
		monster_wave,
		//
		player_bullet_straight,
		//
		monster_bullet_straight,
		monster_bullet_circle,
		monster_bullet_homing,
		monster_bullet_arround,
		monster_bullet_wave,
		//
		effect_bomb,
		item,
		//
		max
	};

	enum class obj_state {
		idle,
		hit,
		die,
		god,	//무적
	};

	enum class obj_dir {
		up,
		left,
		down,
		right,
	};

	enum const_var {
		back_min_size,
		back_max_size,
	};

	const std::vector<math::vec2i> coords{
		{-150, -200},	//back_min_size
		{720, 1000},	//back_max_size
	};

	class unit {
	public:
		unit() noexcept = default;
		virtual ~unit() noexcept = default;

		virtual void load_image();

		virtual const bool in_screen() const;

		virtual void init();	//최초 초기화
		virtual void move(const float delta_time);
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time);
		virtual void destroy(const float delta_time);

		virtual void render(win32gdi::device_context_dynamic& dest_dc);


		const math::vec2i& get_size() const;
		const math::vec2f& get_now_pos() const;
		const shape::circlef& get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		int get_hp() const;
		int get_obj_type() const;
		int get_atk() const;
		int get_state() const;
		int get_my_score() const;
		int get_high_score() const;

		virtual void set_now_pos_and_body(const math::vec2f& point);
		void set_size(const math::vec2i& size);
		void set_body(const math::vec2f& origin, const float r);
		void set_frame_size(const math::vec2i& size);
		void set_obj_type(const int i);
		void set_hp(const int hp);
		void set_atk(const int atk);
		void set_state(const int state);
		void set_my_score(const int score);
		void set_high_score(const int high_score);

	protected:
		math::vec2i size_;
		math::vec2f now_pos_;
		shape::circlef body_;	//@param body_ : 충돌범위를 의미
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		int obj_type_;
		int hp_;
		int atk_;
		int state_;
		float delay_hit_;
		int my_score_;
		int high_score_;
	};
}
