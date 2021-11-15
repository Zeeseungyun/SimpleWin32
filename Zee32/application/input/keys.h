#pragma once
#include "core/string.h"

namespace zee {
namespace input {
	struct key;
	const tstring& to_string(const key& key) noexcept;
	key string_to_key(const tstring& str) noexcept;

	struct key {
		typedef short value_type;
		value_type value = 0;
		const tstring& to_string() const noexcept { return input::to_string(*this); }
	};

	bool is_valid(const key& check_key) noexcept;
	bool operator==(const key& l, const key& r) noexcept;
	bool operator!=(const key& l, const key& r) noexcept;

namespace keys {
	//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	extern const key none;

	extern const key lbutton;
	extern const key rbutton;
	extern const key cancel;
	extern const key mbutton;
	extern const key xbutton1;
	extern const key xbutton2;
	extern const key backspace;
	extern const key tab;
	extern const key clear;
	extern const key enter;
	extern const key shift;
	extern const key control;
	extern const key alt;
	extern const key pause;

	extern const key escape;
	extern const key spacebar;
	extern const key page_up;
	extern const key page_down;
	extern const key end;
	extern const key home;
	extern const key left;
	extern const key up;
	extern const key right;
	extern const key down;
	extern const key select;
	extern const key print_text;
	extern const key execute;
	extern const key print_screen;
	extern const key insert;
	extern const key del;
	extern const key help;

	extern const key key0;	extern const key key1;	extern const key key2;
	extern const key key3;	extern const key key4;	extern const key key5;
	extern const key key6;	extern const key key7;	extern const key key8;
	extern const key key9;

	extern const key A;	extern const key B;	extern const key C;	extern const key D;
	extern const key E;	extern const key F;	extern const key G;	extern const key H;
	extern const key I;	extern const key J;	extern const key K;	extern const key L;
	extern const key M;	extern const key N;	extern const key O;	extern const key P;
	extern const key Q;	extern const key R;	extern const key S;	extern const key T;
	extern const key U;	extern const key V;	extern const key W;	extern const key X;
	extern const key Y;	extern const key Z;
	extern const key left_win;
	extern const key right_win;
	extern const key app;

	extern const key sleep;

	extern const key num0;	extern const key num1;	extern const key num2;
	extern const key num3;	extern const key num4;	extern const key num5;
	extern const key num6;	extern const key num7;	extern const key num8;
	extern const key num9;

	extern const key multiply;
	extern const key add;
	extern const key separator;
	extern const key subtract;
	extern const key decimal;
	extern const key divide;

	extern const key F1; extern const key F2; extern const key F3; extern const key F4;
	extern const key F5; extern const key F6; extern const key F7; extern const key F8;
	extern const key F9; extern const key F10; extern const key F11; extern const key F12;
	extern const key F13; extern const key F14; extern const key F15; extern const key F16;
	extern const key F17; extern const key F18; extern const key F19; extern const key F20;
	extern const key F21; extern const key F22; extern const key F23; extern const key F24;

	extern const key num_lock;
	extern const key scroll_lock;

	extern const key left_shift;
	extern const key right_shift;

	extern const key left_control;
	extern const key right_control;

	extern const key left_alt;
	extern const key right_alt;

	extern const key browser_back;
	extern const key browser_forward;
	extern const key browser_refresh;
	extern const key browser_stop;
	extern const key browser_search;
	extern const key browser_favorites;
	extern const key browser_home;

	extern const key volume_mute;
	extern const key volume_down;
	extern const key volume_up;

	extern const key media_next_track;
	extern const key media_prev_track;
	extern const key media_stop;
	extern const key media_play_pause;

	extern const key launch_mail;
	extern const key launch_media_select;
	extern const key launch_media_app1;
	extern const key launch_media_app2;

	extern const key play;
	extern const key zoom;

}//namespace zee::input::keys
}//namespace zee::input
}//namespace zee