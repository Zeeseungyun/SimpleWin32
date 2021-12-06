#pragma once
#include "../win32gdi/device_context.h"
#include "../win32helper/win32helper.h"

namespace zee {
	//��� (enum hack)
	const enum const_var_stage {
		TIMER_CYCLE = 1000,	//�ð� ǥ��
		TIME_LIMIT = 50,	//���ѽð�
		KILL_REMAIN = 3,	//�ܰ躰 ���� óġ ��
		KILL_REMAIN_CLEAR = 36,	//Ŭ������� ���� óġ ��
		SPAWN_MONSTER_MAX = 2,	//���� ������ ���� ��
		SPAWN_CYCLE_1 = 1000,	//��1 ���� ���� �ֱ�
		SPAWN_CYCLE_2 = 1500,	//��2 ���� ���� �ֱ�
		DIG_NUM = 6		//�δ��� ���� ����
	};

	//���� ����
	const enum e_game_state {
		TITLE,
		INGAME,
		ENDING,
	};

	//��ǥ
	const enum e_coord {
		//������
		lefttop_lt, lefttop_rb, middletop_lt, middletop_rb, righttop_lt, righttop_rb,
		leftmiddle_lt, leftmiddle_rb, middlemiddle_lt, middlemiddle_rb, rightmiddle_lt, rightmiddle_rb,
		DigMax,	//���������� ����
		//Ÿ��Ʋ
		title_st, title_st2, title_st3, title_st4, title_st5,
		title_ending_box_lt, title_ending_box_rb,
		//�ΰ���
		stagebox_lt, stagebox_rb,
		time_st, score_st, stage_st, time_limit_st,
		time_num, score_num, stage_num, time_limit_num,
		kill_remain_clear_st, kill_remain_st, spawn_1_speed_st, spawn_2_speed_st,
		kill_remain_clear_num, kill_remain_num, spawn_1_speed_num, spawn_2_speed_num,
		//����
		ending_st, ending_st2, time_taken, time_taken_num, score_final, score_final_num,
		Max
	};
	extern math::vec2i coord_list[];	//��ǥ�� ¦

	//���� ����(����, �ð� ��)
	extern int game_state;
	extern int timer;
	extern int score;
	extern int now_stage;
	extern int time_limit;
	extern int kill_remain;
	extern int kill_remain_clear;
	extern float spawn_coef;	//�ܰ躰 �� ���� ���� ����. ���� ���� �ֱ� * ���
	extern float spawn_cycle[];
	extern math::vec2i mouse_position;

	//�Լ�
	void init_game();
	void set_timer(const HWND& hWnd, const int& num, const float& time_cycle);	//Ÿ�̸�
	void kill_timer(const HWND& hWnd, const int& num);
	void change_var_ingame(const HWND& hWnd);
	void change_var_ending(const HWND& hWnd);
}