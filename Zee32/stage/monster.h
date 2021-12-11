#pragma once
#include "../win32gdi/device_context.h"
#include "../win32helper/win32helper.h"
#include "image_data.h"

namespace zee {
	//몬스터 상태
	enum e_monster_state {
		none,
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
		void spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer, win32gdi::device_context_dynamic& image) const;
		void attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
			std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer, win32gdi::device_context_dynamic& image) const;
		const int& get_state() const;
		const shape::recti& get_hitbox() const;
		void set_state(const int& other_state);
		void set_hitbox(const shape::recti& other_hitbox);

	private:
		int state_;
		shape::recti hitbox_;
	};

}