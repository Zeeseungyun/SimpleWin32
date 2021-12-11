#include "monster.h"

namespace zee {
	//몬스터 클래스 멤버
	monster::monster() noexcept : state_(none), hitbox_({ 2000, 2000, 2000, 2000 }) {}
	monster::~monster() noexcept {}


	//스폰, 어택
	void monster::spawn() {
		state_ = alive;
	}
	void monster::attacked(const HWND& hWnd) {
		state_ = die;
		PlaySound(TEXT("./assets/Hit_Hurt14.wav"), nullptr, SND_ASYNC);
	}

	//스폰, 어택 애니 호출
	void monster::spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer, win32gdi::device_context_dynamic& image) const {
		
		image.transparent_blt(*temp_back_buffer, { hitbox_.left, hitbox_.top }, RGB(195, 195, 195));
	}
	void monster::attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer, win32gdi::device_context_dynamic& image) const {
		image.transparent_blt(*temp_back_buffer, { hitbox_.left, hitbox_.top }, RGB(195, 195, 195));
	}


	const int& monster::get_state() const {
		return state_;
	}
	const shape::recti& monster::get_hitbox() const {
		return hitbox_;
	}
	void monster::set_state(const int& other_state) {
		state_ = other_state;
	}
	void monster::set_hitbox(const shape::recti& other_hitbox) {
		hitbox_.left = other_hitbox.left;
		hitbox_.right = other_hitbox.right;
		hitbox_.top = other_hitbox.top;
		hitbox_.bottom = other_hitbox.bottom;
	}
}