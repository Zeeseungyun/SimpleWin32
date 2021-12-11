#include "stage.h"

namespace zee {
	//��ǥ
	math::vec2i coord_list[max] = {
		//������
		{100, 100}, {200, 200},	{300, 100}, {400, 200},	{500, 100}, {600, 200},
		{100, 300}, {200, 400},	{300, 300}, {400, 400},	{500, 300}, {600, 400},
		{0, 0},
		//Ÿ��Ʋ
		{240, 310}, {234, 290}, {234, 330}, {264, 360}, {294, 380},
		{220, 250}, {480, 450},
		//�ΰ���
		{230, 470}, {460, 650},
		{240, 480}, {240, 500}, {240, 520}, {240, 540},
		{296, 480}, {296, 500}, {326, 520}, {326, 540},
		{240, 560}, {240, 580}, {240, 600}, {240, 620},
		{410, 560}, {432, 580}, {370, 600}, {370, 620},
		//����
		{300, 300}, {262, 400}, {290, 340}, {400, 340}, {290, 360}, {340, 360},
	};

	//���� ���� ����
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

	//���� �������� ����
	std::vector<monster> monsters(6);



	//Ÿ��Ʋ���� ���� �ʱ�ȭ
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

	//������������ ���� �迭 ����
	void init_monster() {
		for (monster& mon : monsters) {
			mon.set_state(none);
		}
		//��Ʈ�ڽ� ���� ����
		monsters[0].set_hitbox({ coord_list[lefttop_lt], coord_list[lefttop_rb] });
		monsters[1].set_hitbox({ coord_list[middletop_lt], coord_list[middletop_rb] });
		monsters[2].set_hitbox({ coord_list[righttop_lt], coord_list[righttop_rb] });
		monsters[3].set_hitbox({ coord_list[leftmiddle_lt], coord_list[leftmiddle_rb] });
		monsters[4].set_hitbox({ coord_list[middlemiddle_lt], coord_list[middlemiddle_rb] });
		monsters[5].set_hitbox({ coord_list[rightmiddle_lt], coord_list[rightmiddle_rb] });
	}

	//Ÿ�̸�
	void set_timer(const HWND& hWnd, const int& num, const float& time_cycle) {
		SetTimer(hWnd, num, static_cast<int>(time_cycle), nullptr);
	}
	void kill_timer(const HWND& hWnd, const int& num) {
		KillTimer(hWnd, num);
	}

	//���� �ڸ� ����
	void spawn_stage() {
		int rand_pos = rand(0, dig_num - 1);
		//�̹� ������ �ڸ��� �ݴ��� ����
		if (spawn_pos[rand_pos]) {
			rand_pos = dig_num - 1 - rand_pos;
		}
		spawn_pos[rand_pos] = true;
		monsters[rand_pos].spawn();
	}


	//�δ��� ��� �� �������� ���� ����
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
			//�������� ��� �� : ���� ����Ŭ * ���
			for (int i = 0; i != spawn_same_time; i++) {
				if (spawn_cycle[i] * spawn_coef > 0) {
					spawn_cycle[i] *= spawn_coef;
				}
			}

			//���� ���� �ð� ���������� �ٽ� ����
			for (int i = 0; i != spawn_same_time; i++) {
				kill_timer(hWnd, i + 2);
				set_timer(hWnd, i + 2, spawn_cycle[i]);
			}
		}
	}
	//���� ���� �� (���ѽð� ����, �δ��� óġ�� ����)
	void change_var_ending(const HWND& hWnd) {
		game_state = ending;
		//ų Ÿ�̸�
		kill_timer(hWnd, 1);
		for (int i = 0; i != spawn_same_time; i++) {
			kill_timer(hWnd, i + 2);
		}
	}
}