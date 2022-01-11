#pragma once
#include "../win32helper/win32helper.h"
#include "../application/application.h"
#include "../application/application_delegates.h"
#include <memory>
#include <interfaces/tickable.h>
#include "../application/key_state.h"
#include "../application/tick_manager.h"
#include "../../ZeeCore/public/math/matrix.h"
#include "../../ZeeCore/public/shape/intersect.h"
#include "background_image.h"
#include "frame_image.h"

/*
1. 상속 구조 개편 (좀 더 예쁘게 해보자!)
2. 이동 - > 충돌 - > 후처리 -> 렌더
3. 전방 선언
4. 풀링


* TODO:: 전방선언활용이 안됐음. 
* 해당 헤더파일에서 player, monster, item, bullet 언급이 없음.
* 그리고 unit도 전방선언을 써야했음.
*/

/*
* 1. enum다 치운다. 2개 봐줌 obj_type, background_type
* 2. 상속 활용 해본다. (*다형성 포함)
* 3. 전역변수 정적변수들 최대한 신경써서 작성한다.
* 4. 매트릭스 다시!
*
*/

/*
* 퍼포먼스가 떨어질때 확인하는 방법은
* 코드로 보고 판단하기도 하는데 
* 그러긴 존나게 힘들고
* 시간을 잽니다
* 어디서 시간을 제일 많이 잡아 먹나 이걸 봐여
* 넥 그거 배우고싶었어여
*/

namespace zee {
	class unit;
	class player;
	class monster;
	class bullet;
	class effect;
	class item;

	//전역 enum
	enum background_type {
		loop, scroll
	};
	//@param background_type ==> background 종류 변경
	constexpr int background_type = loop;
	//@param background_direction_ ==> background loop 방향 조정 0~3
	constexpr int background_direction = 2;
	//constexpr: 자동 인라인 처리 (c++ 14이상) (언리얼에선 extern const T&, extern const T*)

	class stage : public interfaces::tickable, public std::enable_shared_from_this<stage> {
	public:
		enum const_var {
			back_destroy_zone,
			back_min_size,
			back_max_size,
			back_loop_max_size,
			back_scroll_max_size,
			back_scroll_default_unit_pos,
			back_scroll_max,
			back_scroll_unit_max_move,

			monster_spawn_num = 3,
		};
		//이건 이미 피듭개 했으니까..
		//흐름을 파악하기 쉽게 
		//이동 -> 충돌처리 -> 렌더
		//식으로 하는게 좋을거구.
		const std::vector<math::vec2i> coords{
			{2048, 2048},	//back_destroy_zone
			{-150, -200},	//back_min_size
			{720, 1000},	//back_max_size
			{720, 1280},	//back_loop_max_size
			{1152, 2048},	//back_scroll_max_size
			{0, 1100},		//back_scroll_default_unit_pos
			{390, 1152},	//back_scroll_max
			{705, 770},		//back_scroll_unit_max_move
		};

		stage() noexcept = default;
		virtual ~stage() noexcept = default;

		void on_app_started();
		void on_resize(const math::vec2i& client_size);

		void init_game();

		void tick(float delta_time) override;
		void spawn_bomb(std::shared_ptr<unit> other);
		void spawn_item(std::shared_ptr<unit> other);
		void render(win32gdi::device_context_base& dest_dc, const float g_fps);

		const math::vec2f get_background_src_pos() const;
		const math::vec2f get_background_src_size() const;
		void set_background_src_pos(const math::vec2f& src_pos);
		void set_background_src_size(const math::vec2f& size);
		//여기 frame image를 쓰는 곳이 없쥬? 네
		//그럼? 필요 없겠쥬? 네
	private:
		win32gdi::device_context_dynamic back_buffer_;
		math::vec2f background_src_pos_;
		math::vec2i background_src_size_;
		std::vector<std::shared_ptr<player>> players_;
		std::vector<std::shared_ptr<monster>> monsters_;
		std::vector<std::shared_ptr<effect>> bombs_;
		std::vector<std::shared_ptr<item>> items_;
		float game_time_;
	};
}

