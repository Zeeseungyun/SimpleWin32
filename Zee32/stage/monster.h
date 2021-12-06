#pragma once
#include "stage.h"

namespace zee {
	//몬스터 상태
	const enum e_monster_state {
		NONE,
		SPAWN,
		ALIVE,
		DIE
	};
	//몬스터 클래스
	class monster {
	public:
		explicit monster() noexcept : state(NONE), rand_pos(0), rand_rect({ 2000,2000,2000,2000 }) {}	//mouse_position과 최초 안 겹치게 하기 위함
		virtual ~monster() noexcept {}
		void spawn();
		void attacked(const HWND& hWnd);
		void render_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer);
		void spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const;
		void attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
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