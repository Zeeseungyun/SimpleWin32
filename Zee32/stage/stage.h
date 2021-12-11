#pragma once
#include "monster.h"
#include <vector>

namespace zee {
	//��� (enum hack)
	enum const_var_stage {
		timer_cycle = 1000,	//�ð� ǥ��
		timer_limit = 50,	//���ѽð�
		kill_remain = 3,	//�ܰ躰 ���� óġ ��
		kill_remain_clear = 36,	//Ŭ������� ���� óġ ��
		spawn_same_time = 2,
		spawn_cycle_1 = 1000,	//��1 ���� ���� �ֱ�
		spawn_cycle_2 = 1500,	//��2 ���� ���� �ֱ�
		dig_num = 6		//�δ��� ���� ����
	};

	//���� ����
	enum e_game_state {
		title,
		ingame,
		ending,
	};

	//��ǥ
	enum e_coord {
		//������
		lefttop_lt, lefttop_rb, middletop_lt, middletop_rb, righttop_lt, righttop_rb,
		leftmiddle_lt, leftmiddle_rb, middlemiddle_lt, middlemiddle_rb, rightmiddle_lt, rightmiddle_rb,
		dig_max,	//���������� ����
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
		max
	};
	extern math::vec2i coord_list[];	//��ǥ�� ¦

	//���� ����(����, �ð� ��)
	extern math::vec2i mouse_position;
	extern int game_state;
	extern int timer;
	extern int score;
	extern int stage_now;
	extern int time_limit;
	extern int kill_remain_now;
	extern int kill_remain_clear_now;
	extern float spawn_coef;	//�ܰ躰 �� ���� ���� ����. ���� ���� �ֱ� * ���
	extern float spawn_cycle[];
	extern bool spawn_pos[];

	//���� ����
	extern std::vector<monster> monsters;


	//�Լ�
	void init_ingame();
	void init_monster();
	void set_timer(const HWND& hWnd, const int& num, const float& time_cycle);	//Ÿ�̸�
	void kill_timer(const HWND& hWnd, const int& num);

	void spawn_stage();

	void change_var_ingame(const HWND& hWnd);
	void change_var_ending(const HWND& hWnd);
}