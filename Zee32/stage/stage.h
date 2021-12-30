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
#include "../../ZeeCore/public/math/matrix.h"//�׽�Ʈ��
#include "../../ZeeCore/public/shape/intersect.h"

namespace zee {
	enum background_type {
		loop, scroll
	};
	//@param background_type ==> background ���� ����
	static const int background_type_ = loop;
	//@param background_direction_ ==> background loop ���� ���� 0~3
	static const int background_direction_ = 2;

	enum class obj_type {
		unit,
		monster,
		bullet_unit,
		bullet_follow_unit,
		bullet_monster,
		bullet_follow_monster,
		bomb,
		max
	};
	enum class obj_state {
		idle,
		hit
	};
	enum class obj_shoot_type {
		straight,
		circle,
		follow,
		max
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
		unit_max_move_size_x = 760,			unit_max_move_size_y = 940,
		unit_bullet_size_x = 10,			unit_bullet_size_y = 10,
		unit_bullet_follow_size_x = 12,		unit_bullet_follow_size_y = 20,
		unit_bullet_frame_x = 0,			unit_bullet_frame_y = 0,
		unit_max_bullet_num = 20,
		bullet_next_frame_x = 28,

		monster_size_x = 32,				monster_size_y = 32,
		monster_default_frame_x = 0,		monster_default_frame_y = 0,
		monster_min_pos_x = -100,			monster_min_pos_y = -100,
		monster_max_pos_x = 800,			monster_max_pos_y = 900,
		monster_bullet_size_x = 10,			monster_bullet_size_y = 10,
		monster_bullet_follow_size_x = 11,	monster_bullet_follow_size_y = 17,
		monster_bullet_frame_x = 10,		monster_bullet_frame_y = 0,
		monster_bullet_max_num = 100,
		monster_spawn_num = 4,

		effect_bomb_size_x = 32,			effect_bomb_size_y = 32,
		effect_bomb_final_frame_x = 128
	};

	class stage : public interfaces::tickable, public std::enable_shared_from_this<stage> {
	public:
		stage() noexcept;
		virtual ~stage() noexcept;

		void on_app_started();
		void init_game();
		void spawn_monster();
		void on_resize(const math::vec2i& client_size);
		void tick(float delta_time) override;
		void render(win32gdi::device_context_base& dest_dc, const float& g_fps);

		const math::vec2f get_background_src_pos() const;
		const math::vec2f get_background_src_size() const;
		const std::vector<std::shared_ptr<unit>> get_units() const;
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

