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
		unit_bullet_next_frame,

		monster_size,
		monster_default_frame,
		monster_min_pos,
		monster_max_pos,
		monster_bullet_size,
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
		{800, 1200},	//back_loop_max_size
		{1152, 2048},	//back_scroll_max_size
		{0, 1100},		//back_scroll_default_unit_pos
		{390, 1152},	//back_scroll_max
		{705, 770},		//back_scroll_unit_max_move

		{31, 48},		//unit_size
		{350, 650},		//unit_default_pos
		{60, 0},		//unit_default_frame
		{760, 940},		//unit_max_move_size
		{10, 10},		//unit_bullet_size
		{12, 20},		//unit_bullet_follow_size
		{0, 0},			//unit_bullet_frame
		{28, 0},		//unit_bullet_next_frame
						
		{32, 32},		//monster_size
		{0, 0},			//monster_default_frame
		{0, -100},		//monster_min_pos
		{800, 900},		//monster_max_pos
		{10, 10},		//monster_bullet_size
		{11, 17},		//monster_bullet_follow_size
		{10, 0},		//monster_bullet_frame
						
		{32, 32},		//effect_bomb_size
		{128, 0},		//effect_bomb_final_frame

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

