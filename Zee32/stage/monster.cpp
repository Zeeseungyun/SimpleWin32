#include "monster.h"

namespace zee {
	//몬스터 클래스 멤버
	const int& monster::get_state() const {
		return state;
	}
	const int& monster::get_rand_pos() const {
		return rand_pos;
	}
	const shape::recti& monster::get_rand_rect() const {
		return rand_rect;
	}
	void monster::set_state(const int& other_state) {
		state = other_state;
	}
	void monster::set_rand_pos(const int& other_rand_pos) {
		rand_pos = other_rand_pos;
	}
	void monster::set_rand_rect(const shape::recti& other_rand_rect) {
		rand_rect.left = other_rand_rect.left;
		rand_rect.top = other_rand_rect.top;
		rand_rect.right = other_rand_rect.right;
		rand_rect.bottom = other_rand_rect.bottom;
	}
	void monster::spawn() {
		rand_pos = rand(0, DigMax / 2 - 1);
		rand_rect = { coord_list[rand_pos * 2], coord_list[rand_pos * 2 + 1] }; //터치 영역 (마우스 포인터 점과 사각형 충돌)
		state = ALIVE;
	}
	void monster::attacked(const HWND& hWnd) {
		if (state == ALIVE) {
			state = DIE;
			PlaySound(TEXT("./assets/Hit_Hurt14.wav"), nullptr, SND_ASYNC);
			change_var_ingame(hWnd);
		}
	}
	//스폰, 어택 애니 호출
	void monster::render_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) {
		if (state == ALIVE) {
			spawn_ani(hWnd, temp_dc, temp_image, temp_back_buffer);
		}
		else if (state == DIE) {
			attacked_ani(hWnd, temp_dc, temp_image, temp_back_buffer);
		}
	}
	void monster::spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const {
		temp_image.load_image(TEXT("./assets/monster_spawn.bmp"));
		temp_image.transparent_blt(*temp_back_buffer, coord_list[rand_pos * 2], RGB(195, 195, 195));
		temp_image.clear();
	}
	void monster::attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const {
		temp_image.load_image(TEXT("./assets/monster_die.bmp"));
		temp_image.transparent_blt(*temp_back_buffer, coord_list[rand_pos * 2], RGB(195, 195, 195));
		temp_image.clear();
	}

	//몬스터 전역변수 정의
	std::vector<monster> monsters(2);

	void init_monster() {
		for (monster& mon : monsters) {
			monsters.pop_back();
		}
		monsters.resize(SPAWN_MONSTER_MAX);
	}
	void change_monster_ending() {
		for (monster& mon : monsters) {
			mon.set_rand_rect({ 2000, 2000, 2000, 2000 });
		}
	}
}