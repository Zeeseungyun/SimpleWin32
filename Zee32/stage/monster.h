#pragma once
#include "stage.h"
#include "../stage/image_data.h"
#include "image_data.h"

namespace zee {
	//몬스터 상태
	enum e_monster_state {
		none,
		spawn,
		alive,
		die
	};
	//몬스터 클래스
	class monster {
	public:
		monster() noexcept;
		virtual ~monster() noexcept;
		void spawn();
		void attacked(const HWND& hWnd);
		void render_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer);
		void spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const;
		void attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const;
		void set_state(const int& other_state);
		void set_rand_pos(const int& other_rand_pos);
		void set_rand_rect(const shape::recti& other_rand_rect);
		const int& get_state() const;
		const int& get_rand_pos() const;
		const shape::recti& get_rand_rect() const;

	private:
		int state;
		int rand_pos;	//랜덤 번째
		shape::recti rand_rect;		//랜덤 번째로 생성한 클릭 범위 (히트박스)
	};
	//몬스터 전역
	extern std::vector<monster> monsters;

	//함수
	void init_monster();
	void change_monster_ending();
}