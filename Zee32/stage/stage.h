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
		unit,
		monster_1,
		monster_2,
		monster_3,
		unit_bullet,
		monster_bullet,
		monster_bullet_circle,
		monster_bullet_follow,
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
		back_destroy_zone,
		back_min_size,
		back_loop_max_size,
		back_scroll_max_size,
		back_scroll_default_unit_pos,
		back_scroll_max,
		back_scroll_unit_max_move,

		unit_size,
		unit_default_pos,
		unit_default_frame,
		unit_max_move_size,
		unit_bullet_size,
		unit_bullet_follow_size,
		unit_bullet_frame,

		monster_1_size,
		monster_2_size,
		monster_3_size,
		monster_default_frame,
		monster_min_pos,
		monster_max_pos,
		monster_bullet_size,
		monster_bullet_circle_size,
		monster_bullet_follow_size,
		monster_bullet_frame,

		effect_bomb_size,
		effect_bomb_final_frame,

		unit_max_bullet_num = 20,
		monster_bullet_max_num = 50,
		monster_spawn_num = 4,
	};

	static std::vector<math::vec2i> coords {
		{2048, 2048},	//back_destroy_zone
		{-200, -200},	//back_min_size
		{720, 1280},	//back_loop_max_size
		{1152, 2048},	//back_scroll_max_size
		{0, 1100},		//back_scroll_default_unit_pos
		{390, 1152},	//back_scroll_max
		{705, 770},		//back_scroll_unit_max_move

		{64, 58},		//unit_size
		{350, 650},		//unit_default_pos
		{64, 0},		//unit_default_frame
		{660, 940},		//unit_max_move_size
		{16, 16},		//unit_bullet_size
		{13, 22},		//unit_bullet_follow_size
		{0, 0},			//unit_bullet_frame
						
		{64, 48},		//monster_1_size
		{52, 62},		//monster_2_size
		{60, 64},		//monster_3_size
		{0, 0},			//monster_default_frame
		{0, -100},		//monster_min_pos
		{660, 900},		//monster_max_pos
		{16, 16},		//monster_bullet_size
		{26, 26},		//monster_bullet_circle_size
		{32, 32},		//monster_bullet_follow_size
		{0, 0},			//monster_bullet_frame
						
		{32, 32},		//effect_bomb_size
		{96, 0},		//effect_bomb_final_frame

		{-1, -1},		//none
		{-1, -1},		//none
		{-1, -1},		//none
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

