#include "stage.h"

namespace zee {
	//좌표
	math::vec2i coord_list[Max] = {
		//구덩이
		{100, 100}, {200, 200},	{300, 100}, {400, 200},	{500, 100}, {600, 200},
		{100, 300}, {200, 400},	{300, 300}, {400, 400},	{500, 300}, {600, 400},
		{0, 0},
		//타이틀
		{240, 310}, {234, 290}, {234, 330}, {264, 360}, {294, 380},
		{220, 250}, {480, 450},
		//인게임
		{230, 470}, {460, 650},
		{240, 480}, {240, 500}, {240, 520}, {240, 540},
		{296, 480}, {296, 500}, {326, 520}, {326, 540},
		{240, 560}, {240, 580}, {240, 600}, {240, 620},
		{410, 560}, {432, 580}, {370, 600}, {370, 620},
		//엔딩
		{300, 300}, {262, 400}, {290, 340}, {400, 340}, {290, 360}, {340, 360},
	};

	//전역 변수 정의
	int game_state = title;
	int timer = 0;
	int score = 0;
	int stage_now = 1;
	int time_limit = timer_limit;
	int kill_remain_now = kill_remain;
	int kill_remain_clear_now = kill_remain_clear;
	float spawn_coef = 0.8f;
	float spawn_cycle[spawn_monster_max] = { spawn_cycle_1, spawn_cycle_2 };
	math::vec2i mouse_position = { 0, 0 };

	image_data images;


	//타이틀에서 게임 초기화
	void init_game() {
		game_state = title;
		stage_now = 1;
		timer = 0;
		score = 0;
		mouse_position = { 0, 0 };
		kill_remain_now = kill_remain;
		kill_remain_clear_now = kill_remain_clear;
		spawn_cycle[0] = spawn_cycle_1;
		spawn_cycle[1] = spawn_cycle_2;
		time_limit = timer_limit;
	}


	//타이머
	void set_timer(const HWND& hWnd, const int& num, const float& time_cycle) {
		SetTimer(hWnd, num, static_cast<int>(time_cycle), nullptr);
	}
	void kill_timer(const HWND& hWnd, const int& num) {
		KillTimer(hWnd, num);
	}

	//두더쥐 사망 시 스테이지 변수 변동
	void change_var_ingame(const HWND& hWnd) {
		score++;
		kill_remain_now--;
		kill_remain_clear_now--;
		if (kill_remain_clear_now <= 0) {
			change_var_ending(hWnd);
		}
		else if (kill_remain_now <= 0) {
			stage_now++;
			kill_remain_now = kill_remain * stage_now;
			//스테이지 상승 시 : 현재 사이클 * 계수
			for (int i = 0; i != spawn_monster_max; i++) {
				if (spawn_cycle[i] * spawn_coef > 0) {
					spawn_cycle[i] *= spawn_coef;
				}
			}

			//몬스터 스폰 시간 가속해서 갱신
			for (int i = 0; i != spawn_monster_max; i++) {
				kill_timer(hWnd, i + 2);
				set_timer(hWnd, i + 2, spawn_cycle[i]);
			}
		}
	}
	//게임 종료 시 (제한시간 종료, 두더쥐 처치수 만족)
	void change_var_ending(const HWND& hWnd) {
		game_state = ending;
		//킬 타이머
		kill_timer(hWnd, 1);
		for (int i = 0; i != spawn_monster_max; i++) {
			kill_timer(hWnd, i + 2);
		}
	}
}