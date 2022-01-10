#pragma once
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include <interfaces/tickable.h>
#include "../application/key_state.h"
#include <vector>
#include "frame_image.h"

namespace zee {

	/*
	* TODO::
	* 이게 왜 여기있음?
	* 전역 속성을 따로 정의해야하는 것이 옳음.
	* 게다가 stage::const_var과 겹침.
	* 그리고 상수로 활용하는 것으로 보이는데 coords가 객체마다 만들어질 이유가 없음.
	*
	* bullet, effect, item, monster, player가 다 unit을 상속받음.
	* 계층 구조가 좀 더 구체적이면 좋을 듯.
	* ex)
	*			unit(effect와 합쳐도 될듯?)
	*				|
	*		 _____effect____
	*		|				|
	*	 _plane_		projectile
	*	|		|		|		|
	*	player	monster	bullet	item
	*
	* set_hp나 score와 같은 모든 유닛들이 기본적으로 갖는 프로퍼티는
	* 공통적으로 init이라는 함수를 통해서 설정해야 할 값들임.
	* 
	* hp_는 직접 참조해야 할때만 사용하는 것이 좋슴. (ex 수정)
	* 그리고 수정하는 함수로 set_hp를 만들어두었는데 그저 init에서만 사용된건 바람직하지 못함.
	* 
	* 값의 반환을 굳이 const int로 할 필요까지는 없음 코드가 길어져서 피곤해짐.
	* 어차피 레퍼런스가 아닌 타입으로 반환시 복사된 내용이 반환되므로 변하지가 않기 떄문.
	* 
	* 충돌처리가 아마 코스트가 클 것으로 예상이 됨.
	* 무지성으로 짜면 O(N*N)이 되기때문..
	* 그래서 충돌처리 로직은 따로 구성해서 그 부분만 보이도록 하는 것이 일반적.
	* 한눈에 들어오지 않음.
	* 충돌체 크기설정을 body라고 해놨는데 객체화시켜서 관리했으면 더 좋지 않았을까?
	*/

	class unit {
	public:
		enum const_var {
			back_destroy_zone,
			back_min_size,
			back_max_size,
		};

		const std::vector<math::vec2i> coords{
			{2048, 2048},	//back_destroy_zone
			{-150, -200},	//back_min_size
			{720, 1000},	//back_max_size
		};

		/*
		* TODO::
		* unit을 상속받을 객체가 이 이넘 클래스를 통해 정해진 듯한 형태임.
		* 이렇게 짜면 안됨.
		*/
		enum class obj_type {
			player_straight,

			monster_straight,
			monster_circle,
			monster_homing,
			monster_arround,
			monster_wave,

			player_bullet_straight,

			monster_bullet_straight,
			monster_bullet_circle,
			monster_bullet_homing,
			monster_bullet_arround,
			monster_bullet_wave,

			bomb,
			item,

			max
		};

		enum class obj_state {
			idle,
			hit,
			die
		};

		unit() noexcept = default;
		virtual ~unit() noexcept = default;

		virtual void load_image();
		virtual const bool in_screen() const;

		virtual void init();
		virtual void move(const float delta_time);
		virtual void hit_from(const std::shared_ptr<unit> other, const float delta_time);
		virtual void destroy(const float delta_time);
		virtual void render(win32gdi::device_context_dynamic& dest_dc);

		const math::vec2i& get_size() const;
		const math::vec2f& get_now_pos() const;
		const shape::circlef& get_body() const;
		const math::vec2i& get_frame_x() const;
		const math::vec2i& get_frame_y() const;
		
		const int get_hp() const;
		const int get_obj_type() const;
		const int get_state() const;
		const int get_my_score() const;
		const int get_high_score() const;

		void set_size(const math::vec2i& size);
		void set_now_pos_and_body(const math::vec2f& point);
		void set_body(const math::vec2f& origin, const float r);
		void set_max_move_size(const math::vec2i& size);
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
		shape::circlef body_;
		math::vec2i max_move_size_;
		math::vec2i frame_x_;
		math::vec2i frame_y_;
		int obj_type_;
		int hp_;
		int atk_;
		/*
		* TODO::
		* state를 굳이 int로 할 이유가 없음.
		*/
		int state_;
		float delay_hit_;
		int my_score_;
		int high_score_;
	};
}
