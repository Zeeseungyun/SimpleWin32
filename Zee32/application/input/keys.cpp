#include "keys.h"
#include <unordered_map>

namespace zee {
namespace input {
namespace keys {
#define ZEE_DEFINE_KEY(name, value) const key name = { 0x##value };
	//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	ZEE_DEFINE_KEY(none, 0);

	ZEE_DEFINE_KEY(lbutton,		01);
	ZEE_DEFINE_KEY(rbutton,		02);
	ZEE_DEFINE_KEY(cancel,		03);
	ZEE_DEFINE_KEY(mbutton,		04);
	ZEE_DEFINE_KEY(xbutton1,	05);
	ZEE_DEFINE_KEY(xbutton2,	06);

	ZEE_DEFINE_KEY(backspace,	08);
	ZEE_DEFINE_KEY(tab,			09);

	ZEE_DEFINE_KEY(clear, 0C);
	ZEE_DEFINE_KEY(enter, 0D);

	ZEE_DEFINE_KEY(shift,	10);
	ZEE_DEFINE_KEY(control,	11);
	ZEE_DEFINE_KEY(alt,		12);
	ZEE_DEFINE_KEY(pause,	13);

	ZEE_DEFINE_KEY(escape,		1B);

	ZEE_DEFINE_KEY(spacebar,	20);
	ZEE_DEFINE_KEY(page_up,		21);
	ZEE_DEFINE_KEY(page_down,	22);
	ZEE_DEFINE_KEY(end,			23);
	ZEE_DEFINE_KEY(home,		24);
	ZEE_DEFINE_KEY(left,		25);
	ZEE_DEFINE_KEY(up,			26);
	ZEE_DEFINE_KEY(right,		27);
	ZEE_DEFINE_KEY(down,		28);
	ZEE_DEFINE_KEY(select,		29);
	ZEE_DEFINE_KEY(print_text,		2A);
	ZEE_DEFINE_KEY(execute,		2B);
	ZEE_DEFINE_KEY(print_screen,2C);
	ZEE_DEFINE_KEY(insert,		2D);
	ZEE_DEFINE_KEY(del,			2E);
	ZEE_DEFINE_KEY(help,		2F);

	ZEE_DEFINE_KEY(key0, 30);	ZEE_DEFINE_KEY(key1, 31);	ZEE_DEFINE_KEY(key2, 32);
	ZEE_DEFINE_KEY(key3, 33);	ZEE_DEFINE_KEY(key4, 34);	ZEE_DEFINE_KEY(key5, 35);
	ZEE_DEFINE_KEY(key6, 36);	ZEE_DEFINE_KEY(key7, 37);	ZEE_DEFINE_KEY(key8, 38);
	ZEE_DEFINE_KEY(key9, 39);

	ZEE_DEFINE_KEY(A, 41);	ZEE_DEFINE_KEY(B, 42);	ZEE_DEFINE_KEY(C, 43);	ZEE_DEFINE_KEY(D, 44);
	ZEE_DEFINE_KEY(E, 45);	ZEE_DEFINE_KEY(F, 46);	ZEE_DEFINE_KEY(G, 47);	ZEE_DEFINE_KEY(H, 48);
	ZEE_DEFINE_KEY(I, 49);	ZEE_DEFINE_KEY(J, 4A);	ZEE_DEFINE_KEY(K, 4B);	ZEE_DEFINE_KEY(L, 4C);
	ZEE_DEFINE_KEY(M, 4D);	ZEE_DEFINE_KEY(N, 4E);	ZEE_DEFINE_KEY(O, 4F);	ZEE_DEFINE_KEY(P, 50);
	ZEE_DEFINE_KEY(Q, 51);	ZEE_DEFINE_KEY(R, 52);	ZEE_DEFINE_KEY(S, 53);	ZEE_DEFINE_KEY(T, 54);
	ZEE_DEFINE_KEY(U, 55);	ZEE_DEFINE_KEY(V, 56);	ZEE_DEFINE_KEY(W, 57);	ZEE_DEFINE_KEY(X, 58);
	ZEE_DEFINE_KEY(Y, 59);	ZEE_DEFINE_KEY(Z, 5A);

	ZEE_DEFINE_KEY(left_win,	5B);
	ZEE_DEFINE_KEY(right_win,	5C);
	ZEE_DEFINE_KEY(app,			5D);

	ZEE_DEFINE_KEY(sleep, 5F);

	ZEE_DEFINE_KEY(num0, 60); 	ZEE_DEFINE_KEY(num1, 61); 	ZEE_DEFINE_KEY(num2, 62); 
	ZEE_DEFINE_KEY(num3, 63); 	ZEE_DEFINE_KEY(num4, 64); 	ZEE_DEFINE_KEY(num5, 65); 
	ZEE_DEFINE_KEY(num6, 66); 	ZEE_DEFINE_KEY(num7, 67); 	ZEE_DEFINE_KEY(num8, 68); 
	ZEE_DEFINE_KEY(num9, 69); 
	
	ZEE_DEFINE_KEY(multiply,	6A);
	ZEE_DEFINE_KEY(add,			6B);
	ZEE_DEFINE_KEY(separator,	6C);
	ZEE_DEFINE_KEY(subtract,	6D);
	ZEE_DEFINE_KEY(decimal,		6E);
	ZEE_DEFINE_KEY(divide,		6F);

	ZEE_DEFINE_KEY(F1 , 70); ZEE_DEFINE_KEY(F2 , 71); 	ZEE_DEFINE_KEY(F3 , 72); ZEE_DEFINE_KEY(F4 , 73);
	ZEE_DEFINE_KEY(F5 , 74); ZEE_DEFINE_KEY(F6 , 75); 	ZEE_DEFINE_KEY(F7 , 76); ZEE_DEFINE_KEY(F8 , 77);
	ZEE_DEFINE_KEY(F9 , 78); ZEE_DEFINE_KEY(F10, 79); 	ZEE_DEFINE_KEY(F11, 7A); ZEE_DEFINE_KEY(F12, 7B);
	ZEE_DEFINE_KEY(F13, 7C); ZEE_DEFINE_KEY(F14, 7D); 	ZEE_DEFINE_KEY(F15, 7E); ZEE_DEFINE_KEY(F16, 7F);
	ZEE_DEFINE_KEY(F17, 80); ZEE_DEFINE_KEY(F18, 81); 	ZEE_DEFINE_KEY(F19, 82); ZEE_DEFINE_KEY(F20, 83);
	ZEE_DEFINE_KEY(F21, 84); ZEE_DEFINE_KEY(F22, 85); 	ZEE_DEFINE_KEY(F23, 86); ZEE_DEFINE_KEY(F24, 87);

	ZEE_DEFINE_KEY(num_lock, 90); ZEE_DEFINE_KEY(scroll_lock, 91);

	ZEE_DEFINE_KEY(left_shift,	A0);	ZEE_DEFINE_KEY(right_shift,		A1);
	ZEE_DEFINE_KEY(left_control,A2);	ZEE_DEFINE_KEY(right_control,	A3);
	ZEE_DEFINE_KEY(left_alt,	A4);	ZEE_DEFINE_KEY(right_alt,		A5);

	ZEE_DEFINE_KEY(browser_back,		A6);
	ZEE_DEFINE_KEY(browser_forward,		A7);
	ZEE_DEFINE_KEY(browser_refresh,		A8);
	ZEE_DEFINE_KEY(browser_stop,		A9);
	ZEE_DEFINE_KEY(browser_search,		AA);
	ZEE_DEFINE_KEY(browser_favorites,	AB);
	ZEE_DEFINE_KEY(browser_home,		AC);

	ZEE_DEFINE_KEY(volume_mute,	AD);
	ZEE_DEFINE_KEY(volume_down,	AE);
	ZEE_DEFINE_KEY(volume_up,	AF);

	ZEE_DEFINE_KEY(media_next_track,	B0);
	ZEE_DEFINE_KEY(media_prev_track,	B1);
	ZEE_DEFINE_KEY(media_stop,			B2);
	ZEE_DEFINE_KEY(media_play_pause,	B3);

	ZEE_DEFINE_KEY(launch_mail,			B4);
	ZEE_DEFINE_KEY(launch_media_select, B5);
	ZEE_DEFINE_KEY(launch_media_app1,	B6);
	ZEE_DEFINE_KEY(launch_media_app2,	B7);

	ZEE_DEFINE_KEY(play, FA);
	ZEE_DEFINE_KEY(zoom, FB);

#undef ZEE_DEFINE_KEY
namespace impl {
	struct key_hash {
		size_t operator()(const key value) const noexcept {
			return std_default_hash.operator()(value.value);
		}
	private:
		static const std::hash<key::value_type> std_default_hash;
	};

	const std::hash<key::value_type> key_hash::std_default_hash;

}//namespace zee::input::keys::impl 
#define ZEE_MAKE_KEY_STRING(key_name) {key_name, _T(#key_name) }
	static std::unordered_map<key, tstring, impl::key_hash> key_map_str = {
		ZEE_MAKE_KEY_STRING( none),

		ZEE_MAKE_KEY_STRING( lbutton),
		ZEE_MAKE_KEY_STRING( rbutton),
		ZEE_MAKE_KEY_STRING( cancel),
		ZEE_MAKE_KEY_STRING( mbutton),
		ZEE_MAKE_KEY_STRING( xbutton1),
		ZEE_MAKE_KEY_STRING( xbutton2),
		ZEE_MAKE_KEY_STRING( backspace),
		ZEE_MAKE_KEY_STRING( tab),
		ZEE_MAKE_KEY_STRING( clear),
		ZEE_MAKE_KEY_STRING( enter),
		ZEE_MAKE_KEY_STRING( shift),
		ZEE_MAKE_KEY_STRING( control),
		ZEE_MAKE_KEY_STRING( alt),
		ZEE_MAKE_KEY_STRING( pause),

		ZEE_MAKE_KEY_STRING( escape),
		ZEE_MAKE_KEY_STRING( spacebar),
		ZEE_MAKE_KEY_STRING( page_up),
		ZEE_MAKE_KEY_STRING( page_down),
		ZEE_MAKE_KEY_STRING( end),
		ZEE_MAKE_KEY_STRING( home),
		ZEE_MAKE_KEY_STRING( left),
		ZEE_MAKE_KEY_STRING( up),
		ZEE_MAKE_KEY_STRING( right),
		ZEE_MAKE_KEY_STRING( down),
		ZEE_MAKE_KEY_STRING( select),
		ZEE_MAKE_KEY_STRING( print_text),
		ZEE_MAKE_KEY_STRING( execute),
		ZEE_MAKE_KEY_STRING( print_screen),
		ZEE_MAKE_KEY_STRING( insert),
		ZEE_MAKE_KEY_STRING( del),
		ZEE_MAKE_KEY_STRING( help),

		ZEE_MAKE_KEY_STRING( key0),	ZEE_MAKE_KEY_STRING( key1),	ZEE_MAKE_KEY_STRING( key2),
		ZEE_MAKE_KEY_STRING( key3),	ZEE_MAKE_KEY_STRING( key4),	ZEE_MAKE_KEY_STRING( key5),
		ZEE_MAKE_KEY_STRING( key6),	ZEE_MAKE_KEY_STRING( key7),	ZEE_MAKE_KEY_STRING( key8),
		ZEE_MAKE_KEY_STRING( key9),

		ZEE_MAKE_KEY_STRING( A),	ZEE_MAKE_KEY_STRING( B),	ZEE_MAKE_KEY_STRING( C),	ZEE_MAKE_KEY_STRING( D),
		ZEE_MAKE_KEY_STRING( E),	ZEE_MAKE_KEY_STRING( F),	ZEE_MAKE_KEY_STRING( G),	ZEE_MAKE_KEY_STRING( H),
		ZEE_MAKE_KEY_STRING( I),	ZEE_MAKE_KEY_STRING( J),	ZEE_MAKE_KEY_STRING( K),	ZEE_MAKE_KEY_STRING( L),
		ZEE_MAKE_KEY_STRING( M),	ZEE_MAKE_KEY_STRING( N),	ZEE_MAKE_KEY_STRING( O),	ZEE_MAKE_KEY_STRING( P),
		ZEE_MAKE_KEY_STRING( Q),	ZEE_MAKE_KEY_STRING( R),	ZEE_MAKE_KEY_STRING( S),	ZEE_MAKE_KEY_STRING( T),
		ZEE_MAKE_KEY_STRING( U),	ZEE_MAKE_KEY_STRING( V),	ZEE_MAKE_KEY_STRING( W),	ZEE_MAKE_KEY_STRING( X),
		ZEE_MAKE_KEY_STRING( Y),	ZEE_MAKE_KEY_STRING( Z),
		ZEE_MAKE_KEY_STRING( left_win),
		ZEE_MAKE_KEY_STRING( right_win),
		ZEE_MAKE_KEY_STRING( app),

		ZEE_MAKE_KEY_STRING( sleep),

		ZEE_MAKE_KEY_STRING( num0),	ZEE_MAKE_KEY_STRING( num1),	ZEE_MAKE_KEY_STRING( num2),
		ZEE_MAKE_KEY_STRING( num3),	ZEE_MAKE_KEY_STRING( num4),	ZEE_MAKE_KEY_STRING( num5),
		ZEE_MAKE_KEY_STRING( num6),	ZEE_MAKE_KEY_STRING( num7),	ZEE_MAKE_KEY_STRING( num8),
		ZEE_MAKE_KEY_STRING( num9),

		ZEE_MAKE_KEY_STRING( multiply),
		ZEE_MAKE_KEY_STRING( add),
		ZEE_MAKE_KEY_STRING( separator),
		ZEE_MAKE_KEY_STRING( subtract),
		ZEE_MAKE_KEY_STRING( decimal),
		ZEE_MAKE_KEY_STRING( divide),

		ZEE_MAKE_KEY_STRING( F1), ZEE_MAKE_KEY_STRING( F2), ZEE_MAKE_KEY_STRING( F3), ZEE_MAKE_KEY_STRING( F4),
		ZEE_MAKE_KEY_STRING( F5), ZEE_MAKE_KEY_STRING( F6), ZEE_MAKE_KEY_STRING( F7), ZEE_MAKE_KEY_STRING( F8),
		ZEE_MAKE_KEY_STRING( F9), ZEE_MAKE_KEY_STRING( F10), ZEE_MAKE_KEY_STRING( F11), ZEE_MAKE_KEY_STRING( F12),
		ZEE_MAKE_KEY_STRING( F13), ZEE_MAKE_KEY_STRING( F14), ZEE_MAKE_KEY_STRING( F15), ZEE_MAKE_KEY_STRING( F16),
		ZEE_MAKE_KEY_STRING( F17), ZEE_MAKE_KEY_STRING( F18), ZEE_MAKE_KEY_STRING( F19), ZEE_MAKE_KEY_STRING( F20),
		ZEE_MAKE_KEY_STRING( F21), ZEE_MAKE_KEY_STRING( F22), ZEE_MAKE_KEY_STRING( F23), ZEE_MAKE_KEY_STRING( F24),

		ZEE_MAKE_KEY_STRING( num_lock),
		ZEE_MAKE_KEY_STRING( scroll_lock),

		ZEE_MAKE_KEY_STRING( left_shift),
		ZEE_MAKE_KEY_STRING( right_shift),

		ZEE_MAKE_KEY_STRING( left_control),
		ZEE_MAKE_KEY_STRING( right_control),

		ZEE_MAKE_KEY_STRING( left_alt),
		ZEE_MAKE_KEY_STRING( right_alt),

		ZEE_MAKE_KEY_STRING( browser_back),
		ZEE_MAKE_KEY_STRING( browser_forward),
		ZEE_MAKE_KEY_STRING( browser_refresh),
		ZEE_MAKE_KEY_STRING( browser_stop),
		ZEE_MAKE_KEY_STRING( browser_search),
		ZEE_MAKE_KEY_STRING( browser_favorites),
		ZEE_MAKE_KEY_STRING( browser_home),

		ZEE_MAKE_KEY_STRING( volume_mute),
		ZEE_MAKE_KEY_STRING( volume_down),
		ZEE_MAKE_KEY_STRING( volume_up),

		ZEE_MAKE_KEY_STRING( media_next_track),
		ZEE_MAKE_KEY_STRING( media_prev_track),
		ZEE_MAKE_KEY_STRING( media_stop),
		ZEE_MAKE_KEY_STRING( media_play_pause),

		ZEE_MAKE_KEY_STRING( launch_mail),
		ZEE_MAKE_KEY_STRING( launch_media_select),
		ZEE_MAKE_KEY_STRING( launch_media_app1),
		ZEE_MAKE_KEY_STRING( launch_media_app2),

		ZEE_MAKE_KEY_STRING( play),
		ZEE_MAKE_KEY_STRING( zoom)
	};
#undef ZEE_MAKE_KEY_STRING

#define ZEE_MAKE_STRING_KEY(key_name)  { _T(#key_name), key_name }
	static std::unordered_map<tstring, key> str_map_key = {
		ZEE_MAKE_STRING_KEY( none),

		ZEE_MAKE_STRING_KEY( lbutton),
		ZEE_MAKE_STRING_KEY( rbutton),
		ZEE_MAKE_STRING_KEY( cancel),
		ZEE_MAKE_STRING_KEY( mbutton),
		ZEE_MAKE_STRING_KEY( xbutton1),
		ZEE_MAKE_STRING_KEY( xbutton2),
		ZEE_MAKE_STRING_KEY( backspace),
		ZEE_MAKE_STRING_KEY( tab),
		ZEE_MAKE_STRING_KEY( clear),
		ZEE_MAKE_STRING_KEY( enter),
		ZEE_MAKE_STRING_KEY( shift),
		ZEE_MAKE_STRING_KEY( control),
		ZEE_MAKE_STRING_KEY( alt),
		ZEE_MAKE_STRING_KEY( pause),

		ZEE_MAKE_STRING_KEY( escape),
		ZEE_MAKE_STRING_KEY( spacebar),
		ZEE_MAKE_STRING_KEY( page_up),
		ZEE_MAKE_STRING_KEY( page_down),
		ZEE_MAKE_STRING_KEY( end),
		ZEE_MAKE_STRING_KEY( home),
		ZEE_MAKE_STRING_KEY( left),
		ZEE_MAKE_STRING_KEY( up),
		ZEE_MAKE_STRING_KEY( right),
		ZEE_MAKE_STRING_KEY( down),
		ZEE_MAKE_STRING_KEY( select),
		ZEE_MAKE_STRING_KEY( print_text),
		ZEE_MAKE_STRING_KEY( execute),
		ZEE_MAKE_STRING_KEY( print_screen),
		ZEE_MAKE_STRING_KEY( insert),
		ZEE_MAKE_STRING_KEY( del),
		ZEE_MAKE_STRING_KEY( help),

		ZEE_MAKE_STRING_KEY( key0),	ZEE_MAKE_STRING_KEY( key1),	ZEE_MAKE_STRING_KEY( key2),
		ZEE_MAKE_STRING_KEY( key3),	ZEE_MAKE_STRING_KEY( key4),	ZEE_MAKE_STRING_KEY( key5),
		ZEE_MAKE_STRING_KEY( key6),	ZEE_MAKE_STRING_KEY( key7),	ZEE_MAKE_STRING_KEY( key8),
		ZEE_MAKE_STRING_KEY( key9),

		ZEE_MAKE_STRING_KEY( A),	ZEE_MAKE_STRING_KEY( B),	ZEE_MAKE_STRING_KEY( C),	ZEE_MAKE_STRING_KEY( D),
		ZEE_MAKE_STRING_KEY( E),	ZEE_MAKE_STRING_KEY( F),	ZEE_MAKE_STRING_KEY( G),	ZEE_MAKE_STRING_KEY( H),
		ZEE_MAKE_STRING_KEY( I),	ZEE_MAKE_STRING_KEY( J),	ZEE_MAKE_STRING_KEY( K),	ZEE_MAKE_STRING_KEY( L),
		ZEE_MAKE_STRING_KEY( M),	ZEE_MAKE_STRING_KEY( N),	ZEE_MAKE_STRING_KEY( O),	ZEE_MAKE_STRING_KEY( P),
		ZEE_MAKE_STRING_KEY( Q),	ZEE_MAKE_STRING_KEY( R),	ZEE_MAKE_STRING_KEY( S),	ZEE_MAKE_STRING_KEY( T),
		ZEE_MAKE_STRING_KEY( U),	ZEE_MAKE_STRING_KEY( V),	ZEE_MAKE_STRING_KEY( W),	ZEE_MAKE_STRING_KEY( X),
		ZEE_MAKE_STRING_KEY( Y),	ZEE_MAKE_STRING_KEY( Z),
		ZEE_MAKE_STRING_KEY( left_win),
		ZEE_MAKE_STRING_KEY( right_win),
		ZEE_MAKE_STRING_KEY( app),

		ZEE_MAKE_STRING_KEY( sleep),

		ZEE_MAKE_STRING_KEY( num0),	ZEE_MAKE_STRING_KEY( num1),	ZEE_MAKE_STRING_KEY( num2),
		ZEE_MAKE_STRING_KEY( num3),	ZEE_MAKE_STRING_KEY( num4),	ZEE_MAKE_STRING_KEY( num5),
		ZEE_MAKE_STRING_KEY( num6),	ZEE_MAKE_STRING_KEY( num7),	ZEE_MAKE_STRING_KEY( num8),
		ZEE_MAKE_STRING_KEY( num9),

		ZEE_MAKE_STRING_KEY( multiply),
		ZEE_MAKE_STRING_KEY( add),
		ZEE_MAKE_STRING_KEY( separator),
		ZEE_MAKE_STRING_KEY( subtract),
		ZEE_MAKE_STRING_KEY( decimal),
		ZEE_MAKE_STRING_KEY( divide),

		ZEE_MAKE_STRING_KEY( F1), ZEE_MAKE_STRING_KEY( F2), ZEE_MAKE_STRING_KEY( F3), ZEE_MAKE_STRING_KEY( F4),
		ZEE_MAKE_STRING_KEY( F5), ZEE_MAKE_STRING_KEY( F6), ZEE_MAKE_STRING_KEY( F7), ZEE_MAKE_STRING_KEY( F8),
		ZEE_MAKE_STRING_KEY( F9), ZEE_MAKE_STRING_KEY( F10), ZEE_MAKE_STRING_KEY( F11), ZEE_MAKE_STRING_KEY( F12),
		ZEE_MAKE_STRING_KEY( F13), ZEE_MAKE_STRING_KEY( F14), ZEE_MAKE_STRING_KEY( F15), ZEE_MAKE_STRING_KEY( F16),
		ZEE_MAKE_STRING_KEY( F17), ZEE_MAKE_STRING_KEY( F18), ZEE_MAKE_STRING_KEY( F19), ZEE_MAKE_STRING_KEY( F20),
		ZEE_MAKE_STRING_KEY( F21), ZEE_MAKE_STRING_KEY( F22), ZEE_MAKE_STRING_KEY( F23), ZEE_MAKE_STRING_KEY( F24),

		ZEE_MAKE_STRING_KEY( num_lock),
		ZEE_MAKE_STRING_KEY( scroll_lock),

		ZEE_MAKE_STRING_KEY( left_shift),
		ZEE_MAKE_STRING_KEY( right_shift),

		ZEE_MAKE_STRING_KEY( left_control),
		ZEE_MAKE_STRING_KEY( right_control),

		ZEE_MAKE_STRING_KEY( left_alt),
		ZEE_MAKE_STRING_KEY( right_alt),

		ZEE_MAKE_STRING_KEY( browser_back),
		ZEE_MAKE_STRING_KEY( browser_forward),
		ZEE_MAKE_STRING_KEY( browser_refresh),
		ZEE_MAKE_STRING_KEY( browser_stop),
		ZEE_MAKE_STRING_KEY( browser_search),
		ZEE_MAKE_STRING_KEY( browser_favorites),
		ZEE_MAKE_STRING_KEY( browser_home),

		ZEE_MAKE_STRING_KEY( volume_mute),
		ZEE_MAKE_STRING_KEY( volume_down),
		ZEE_MAKE_STRING_KEY( volume_up),

		ZEE_MAKE_STRING_KEY( media_next_track),
		ZEE_MAKE_STRING_KEY( media_prev_track),
		ZEE_MAKE_STRING_KEY( media_stop),
		ZEE_MAKE_STRING_KEY( media_play_pause),

		ZEE_MAKE_STRING_KEY( launch_mail),
		ZEE_MAKE_STRING_KEY( launch_media_select),
		ZEE_MAKE_STRING_KEY( launch_media_app1),
		ZEE_MAKE_STRING_KEY( launch_media_app2),

		ZEE_MAKE_STRING_KEY( play),
		ZEE_MAKE_STRING_KEY( zoom)
	};
#undef ZEE_MAKE_STRING_KEY

}//namespace zee::input::keys

	bool is_valid(const key& check_key) noexcept {
		if (check_key == keys::none) {
			return false;
		}

		return keys::key_map_str.find(check_key) != end(keys::key_map_str);
	}

	key string_to_key(const tstring& str) noexcept {
		auto found = keys::str_map_key.find(str);
		if (found != end(keys::str_map_key)) {
			return found->second;
		}

		return keys::none;
	}

	const tstring& to_string(const key& key) noexcept {
		auto found = keys::key_map_str.find(key);
		if (found != end(keys::key_map_str)) {
			return found->second;
		}

		return keys::key_map_str[keys::none];
	}

	bool operator==(const key& l, const key& r) noexcept {
		return l.value == r.value;
	}

	bool operator!=(const key& l, const key& r) noexcept {
		return l.value != r.value;
	}

}//namespace zee::input
}//namespace zee

