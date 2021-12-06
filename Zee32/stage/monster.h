#pragma once
#include "stage.h"

namespace zee {
	//���� ����
	const enum e_monster_state {
		NONE,
		SPAWN,
		ALIVE,
		DIE
	};
	//���� Ŭ����
	class monster {
	public:
		explicit monster() noexcept : state(NONE), rand_pos(0), rand_rect({ 2000,2000,2000,2000 }) {}	//mouse_position�� ���� �� ��ġ�� �ϱ� ����
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
		int rand_pos;	//���� ��°
		shape::recti rand_rect;		//���� ��°�� ������ Ŭ�� ���� (��Ʈ�ڽ�)
	};
	//���� ����
	extern std::vector<monster> monsters;

	//�Լ�
	void init_monster();
	void change_monster_ending();
}