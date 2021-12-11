#include "stage.h"

namespace zee {
	//좌표
	math::vec2i coord_list[max] = {
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
	float spawn_cycle[spawn_same_time] = { spawn_cycle_1, spawn_cycle_2 };
	bool spawn_pos[dig_num] { 0, };
	math::vec2i mouse_position = { 0, 0 };

	//몬스터 전역변수 정의
	std::vector<monster> monsters(6);



	//타이틀에서 게임 초기화
	void init_ingame() {
		game_state = title;
		timer = 0;
		score = 0;
		stage_now = 1;
		time_limit = timer_limit;
		kill_remain_now = kill_remain;
		kill_remain_clear_now = kill_remain_clear;
		spawn_cycle[0] = spawn_cycle_1;
		spawn_cycle[1] = spawn_cycle_2;
		mouse_position = { 0, 0 };
	}

	//스테이지에서 몬스터 배열 관리
	void init_monster() {
		for (monster& mon : monsters) {
			mon.set_state(none);
		}
		//히트박스 먼저 세팅
		monsters[0].set_hitbox({ coord_list[lefttop_lt], coord_list[lefttop_rb] });
		monsters[1].set_hitbox({ coord_list[middletop_lt], coord_list[middletop_rb] });
		monsters[2].set_hitbox({ coord_list[righttop_lt], coord_list[righttop_rb] });
		monsters[3].set_hitbox({ coord_list[leftmiddle_lt], coord_list[leftmiddle_rb] });
		monsters[4].set_hitbox({ coord_list[middlemiddle_lt], coord_list[middlemiddle_rb] });
		monsters[5].set_hitbox({ coord_list[rightmiddle_lt], coord_list[rightmiddle_rb] });
	}

	//타이머
	void set_timer(const HWND& hWnd, const int& num, const float& time_cycle) {
		SetTimer(hWnd, num, static_cast<int>(time_cycle), nullptr);
	}
	void kill_timer(const HWND& hWnd, const int& num) {
		KillTimer(hWnd, num);
	}

	//스폰 자리 결정
	void spawn_stage() {
		int rand_pos = rand(0, dig_num - 1);
		//이미 스폰된 자리면 반대편 스폰
		if (spawn_pos[rand_pos]) {
			rand_pos = dig_num - 1 - rand_pos;
		}
		spawn_pos[rand_pos] = true;
		monsters[rand_pos].spawn();
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
			for (int i = 0; i != spawn_same_time; i++) {
				if (spawn_cycle[i] * spawn_coef > 0) {
					spawn_cycle[i] *= spawn_coef;
				}
			}

			//몬스터 스폰 시간 가속했으니 다시 갱신
			for (int i = 0; i != spawn_same_time; i++) {
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
		for (int i = 0; i != spawn_same_time; i++) {
			kill_timer(hWnd, i + 2);
		}
	}
}