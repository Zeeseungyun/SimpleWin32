#pragma once
#include "../win32gdi/device_context.h"
#include "../win32helper/win32helper.h"

namespace zee {
	//상수 (enum hack)
	const enum const_var_stage {
		TIMER_CYCLE = 1000,	//시간 표시
		TIME_LIMIT = 50,	//제한시간
		KILL_REMAIN = 3,	//단계별 남은 처치 수
		KILL_REMAIN_CLEAR = 36,	//클리어까지 남은 처치 수
		SPAWN_MONSTER_MAX = 2,	//동시 스폰할 몬스터 수
		SPAWN_CYCLE_1 = 1000,	//몬1 최초 스폰 주기
		SPAWN_CYCLE_2 = 1500,	//몬2 최초 스폰 주기
		DIG_NUM = 6		//두더지 구멍 개수
	};

	//게임 상태
	const enum e_game_state {
		TITLE,
		INGAME,
		ENDING,
	};

	//좌표
	const enum e_coord {
		//구덩이
		lefttop_lt, lefttop_rb, middletop_lt, middletop_rb, righttop_lt, righttop_rb,
		leftmiddle_lt, leftmiddle_rb, middlemiddle_lt, middlemiddle_rb, rightmiddle_lt, rightmiddle_rb,
		DigMax,	//랜덤범위에 쓰임
		//타이틀
		title_st, title_st2, title_st3, title_st4, title_st5,
		title_ending_box_lt, title_ending_box_rb,
		//인게임
		stagebox_lt, stagebox_rb,
		time_st, score_st, stage_st, time_limit_st,
		time_num, score_num, stage_num, time_limit_num,
		kill_remain_clear_st, kill_remain_st, spawn_1_speed_st, spawn_2_speed_st,
		kill_remain_clear_num, kill_remain_num, spawn_1_speed_num, spawn_2_speed_num,
		//엔딩
		ending_st, ending_st2, time_taken, time_taken_num, score_final, score_final_num,
		Max
	};
	extern math::vec2i coord_list[];	//좌표랑 짝

	//전역 변수(점수, 시간 등)
	extern int game_state;
	extern int timer;
	extern int score;
	extern int now_stage;
	extern int time_limit;
	extern int kill_remain;
	extern int kill_remain_clear;
	extern float spawn_coef;	//단계별 더 빠른 스폰 위함. 현재 스폰 주기 * 계수
	extern float spawn_cycle[];
	extern math::vec2i mouse_position;

	//함수
	void init_game();
	void set_timer(const HWND& hWnd, const int& num, const float& time_cycle);	//타이머
	void kill_timer(const HWND& hWnd, const int& num);
	void change_var_ingame(const HWND& hWnd);
	void change_var_ending(const HWND& hWnd);
}