#pragma once
#include "../win32helper/win32helper.h"
#include "../application/application_delegates.h"
#include <memory>
#include <interfaces/tickable.h>
#include "../application/application.h"
#include "../application/key_state.h"
#include "../application/tick_manager.h"
#include "background_image.h"
#include "unit.h"
#include "monster.h"
#include "bullet.h"
#include "../../ZeeCore/public/math/matrix.h"//테스트용
#include "../../ZeeCore/public/shape/intersect.h"

namespace zee {
	enum background_type {
		loop, scroll
	};
	//@param background_type ==> background 종류 변경
	static const int background_type_ = loop;
	//@param background_direction_ ==> background loop 방향 조정 0~3
	static const int background_direction_ = 2;

	enum class obj_type {
		player,
		bullet,
		monster,
		bomb,
		max
	};
	enum class obj_state {
		idle,
		hit
	};
	enum class obj_spawn {
		mon_left,
		mon_middle,
		mon_right
	};

	enum const_var {
		back_destroy_zone_x = 2048,			back_destroy_zone_y = 2048,
		back_min_size_x = -200,				back_min_size_y = -200,
		back_loop_max_size_x = 800,			back_loop_max_size_y = 1200,
		back_scroll_max_size_x = 1152,		back_scroll_max_size_y = 2048,
		back_scroll_default_unit_pos_x = 0,	back_scroll_default_unit_pos_y = 1100,
		back_scroll_max_x = 390,			back_scroll_max_y = 1152,
		back_scroll_unit_max_move_x = 705, back_scroll_unit_max_move_y = 770,

		unit_size_x = 31,					unit_size_y = 48,
		unit_default_pos_x = 350,			unit_default_pos_y = 650,
		unit_default_frame_x = 60,			unit_default_frame_y = 0,
		unit_max_move_size_x = 720,			unit_max_move_size_y = 820,
		unit_bullet_size_x = 10,			unit_bullet_size_y = 10,
		unit_bullet_frame_x = 0,			unit_bullet_frame_y = 0,

		monster_size_x = 32,				monster_size_y = 32,
		monster_default_frame_x = 0,		monster_default_frame_y = 0,
		monster_default_left_pos_x = -50, monster_default_left_pos_y = -50,
		monster_default_middle_pos_x = 350,	monster_default_middle_pos_y = -50,
		monster_default_right_pos_x = 750,	monster_default_right_pos_y = -50,
		monster_bullet_size_x = 10,			monster_bullet_size_y = 10,
		monster_bullet_frame_x = 10,		monster_bullet_frame_y = 0,

		effect_bomb_size_x = 32,			effect_bomb_size_y = 32,

		unit_max_bullet_num = 20,
		monster_max_bullet_num = 20,
		effect_bomb_final_frame_x = 128,
		
		//나중에 enum hack
	};

	class stage : public interfaces::tickable, public std::enable_shared_from_this<stage> {
	public:
		stage() noexcept;
		virtual ~stage() noexcept;

		void on_app_started();
		void game_init();
		void spawn_monster(const int& i);
		void on_resize(const math::vec2i& client_size);
		void tick(float delta_time) override;
		void render(win32gdi::device_context_base& dest_dc);

		const math::vec2f get_background_src_pos() const;
		const math::vec2f get_background_src_size() const;
		void set_background_src_pos(const math::vec2f& src_pos);
		void set_background_src_size(const math::vec2f& size);

	private:
		win32gdi::device_context_dynamic back_buffer_;
		std::vector<std::shared_ptr<unit>> units_;
		std::vector<std::shared_ptr<monster>> monsters_;

		math::vec2f background_src_pos_;
		math::vec2i background_src_size_;
	};
}

