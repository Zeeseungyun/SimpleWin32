#pragma once
#include <util/helper_macros.h>
#include <core/core_base.h>
#include <core/string.h>
#include <limits>

namespace zee {
	struct key {
		typedef byte element_type;
		element_type value = 0;
		constexpr operator element_type() const { return value; }
	};

namespace keys {
	//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	constexpr key invalid							= {0x00};// Undefined
	constexpr key mouse_button_left					= {0x01};//	Left mouse button
	constexpr key mouse_button_right				= {0x02};//	Right mouse button
	constexpr key control_break						= {0x03};//	Control-break processing
	constexpr key mouse_button_middle				= {0x04};//	Middle mouse button (three-button mouse)
	constexpr key mouse_button_x1					= {0x05};//	X1 mouse button
	constexpr key mouse_button_x2					= {0x06};//	X2 mouse button
	constexpr key undefined0						= {0x07};//	Undefined
	constexpr key backspace							= {0x08};//	BACKSPACE key
	constexpr key tab   							= {0x09};//	TAB key
	constexpr key reserved0   						= {0x0A};//	Reserved
	constexpr key reserved1   						= {0x0B};//	Reserved
	constexpr key clear   							= {0x0C};//	CLEAR key
	constexpr key enter    							= {0x0D};//	ENTER key
	constexpr key undefined1    					= {0x0E};//	Undefined key
	constexpr key undefined2						= {0x0F};//	Undefined key
	constexpr key shift   							= {0x10};//	SHIFT key
	constexpr key control   						= {0x11};//	CTRL key
	constexpr key alt    							= {0x12};//	ALT key
	constexpr key pause   							= {0x13};//	PAUSE key
	constexpr key caps_lock  						= {0x14};//	CAPS LOCK key
	constexpr key ime_mode_hanguel					= {0x15};//	IME Hanguel mode(maintained for compatibility; use VK_HANGUL)
	constexpr key ime_mode_kana						= {0x15};//	IME Kana mode
	constexpr key ime_mode_hangul					= {0x15};//	IME Hangul mode
	constexpr key ime_on   							= {0x16};//	IME On
	constexpr key ime_mode_junja					= {0x17};//	IME Junja mode
	constexpr key ime_mode_final					= {0x18};//	IME final mode
	constexpr key ime_mode_hanja					= {0x19};//	IME Hanja mode
	constexpr key ime_mode_kanji					= {0x19};//	IME Kanji mode
	constexpr key ime_off							= {0x1A};//	IME Off
	constexpr key escape   							= {0x1B};//	ESC key
	constexpr key ime_convert						= {0x1C};//	IME convert
	constexpr key ime_nonconvert					= {0x1D};//	IME nonconvert
	constexpr key ime_accept						= {0x1E};//	IME accept
	constexpr key ime_mode_change_request			= {0x1F};//	IME mode change request
	constexpr key space   							= {0x20};//	SPACEBAR
	constexpr key page_up 							= {0x21};//	PAGE UP key
	constexpr key page_down							= {0x22};//	PAGE DOWN key
	constexpr key end   							= {0x23};//	END key
	constexpr key home								= {0x24};//	HOME key
	constexpr key arrow_left						= {0x25};//	LEFT ARROW key
	constexpr key arrow_up							= {0x26};//	UP ARROW key
	constexpr key arrow_right						= {0x27};//	RIGHT ARROW key
	constexpr key arrow_down						= {0x28};//	DOWN ARROW key
	constexpr key select   							= {0x29};//	SELECT key
	constexpr key print   							= {0x2A};//	PRINT key
	constexpr key execute   						= {0x2B};//	EXECUTE key
	constexpr key print_screen						= {0x2C};//	PRINT SCREEN key
	constexpr key insert   							= {0x2D};//	INS key
	constexpr key delete_key 						= {0x2E};//	DEL key
	constexpr key help   							= {0x2F};//	HELP key
	constexpr key zero								= {0x30};//	0 key
	constexpr key one								= {0x31};//	1 key
	constexpr key two								= {0x32};//	2 key
	constexpr key three								= {0x33};//	3 key
	constexpr key four								= {0x34};//	4 key
	constexpr key five								= {0x35};//	5 key
	constexpr key six								= {0x36};//	6 key
	constexpr key seven								= {0x37};//	7 key
	constexpr key eight								= {0x38};//	8 key
	constexpr key nine								= {0x39};//	9 key
	constexpr key undefined3						= {0x3A};//	Undefined
	constexpr key undefined4						= {0x3B};//	Undefined
	constexpr key undefined5						= {0x3C};//	Undefined
	constexpr key undefined6						= {0x3D};//	Undefined
	constexpr key undefined7						= {0x3E};//	Undefined
	constexpr key undefined8						= {0x3F};//	Undefined
	constexpr key undefined9						= {0x40};//	Undefined
	constexpr key A									= {0x41};//	A key
	constexpr key B									= {0x42};//	B key
	constexpr key C									= {0x43};//	C key
	constexpr key D									= {0x44};//	D key
	constexpr key E									= {0x45};//	E key
	constexpr key F									= {0x46};//	F key
	constexpr key G									= {0x47};//	G key
	constexpr key H									= {0x48};//	H key
	constexpr key I									= {0x49};//	I key
	constexpr key J									= {0x4A};//	J key
	constexpr key K									= {0x4B};//	K key
	constexpr key L									= {0x4C};//	L key
	constexpr key M									= {0x4D};//	M key
	constexpr key N									= {0x4E};//	N key
	constexpr key O									= {0x4F};//	O key
	constexpr key P									= {0x50};//	P key
	constexpr key Q									= {0x51};//	Q key
	constexpr key R									= {0x52};//	R key
	constexpr key S									= {0x53};//	S key
	constexpr key T									= {0x54};//	T key
	constexpr key U									= {0x55};//	U key
	constexpr key V									= {0x56};//	V key
	constexpr key W									= {0x57};//	W key
	constexpr key X									= {0x58};//	X key
	constexpr key Y									= {0x59};//	Y key
	constexpr key Z									= {0x5A};//	Z key
	constexpr key windows_left						= {0x5B};//	Left Windows key (Natural keyboard)
	constexpr key windows_right						= {0x5C};//	Right Windows key (Natural keyboard)
	constexpr key applications						= {0x5D};//	Applications key (Natural keyboard)	
	constexpr key reserved2							= {0x5E};//	Reserved
	constexpr key sleep 							= {0x5F};//	Computer Sleep key
	constexpr key num0								= {0x60};//	Numeric keypad 0 key
	constexpr key num1								= {0x61};//	Numeric keypad 1 key
	constexpr key num2								= {0x62};//	Numeric keypad 2 key
	constexpr key num3								= {0x63};//	Numeric keypad 3 key
	constexpr key num4								= {0x64};//	Numeric keypad 4 key
	constexpr key num5								= {0x65};//	Numeric keypad 5 key
	constexpr key num6								= {0x66};//	Numeric keypad 6 key
	constexpr key num7								= {0x67};//	Numeric keypad 7 key
	constexpr key num8								= {0x68};//	Numeric keypad 8 key
	constexpr key num9								= {0x69};//	Numeric keypad 9 key
	constexpr key multiply							= {0x6A};//	Multiply key
	constexpr key add								= {0x6B};//	Add key
	constexpr key separator							= {0x6C};//	Separator key
	constexpr key subtract							= {0x6D};//	Subtract key
	constexpr key decimal							= {0x6E};//	Decimal key
	constexpr key divide							= {0x6F};//	Divide key
	constexpr key F1								= {0x70};//	F1 key
	constexpr key F2								= {0x71};//	F2 key
	constexpr key F3								= {0x72};//	F3 key
	constexpr key F4								= {0x73};//	F4 key
	constexpr key F5								= {0x74};//	F5 key
	constexpr key F6								= {0x75};//	F6 key
	constexpr key F7								= {0x76};//	F7 key
	constexpr key F8								= {0x77};//	F8 key
	constexpr key F9								= {0x78};//	F9 key
	constexpr key F10								= {0x79};//	F10 key
	constexpr key F11								= {0x7A};//	F11 key
	constexpr key F12								= {0x7B};//	F12 key
	constexpr key F13								= {0x7C};//	F13 key
	constexpr key F14								= {0x7D};//	F14 key
	constexpr key F15								= {0x7E};//	F15 key
	constexpr key F16								= {0x7F};//	F16 key
	constexpr key F17								= {0x80};//	F17 key
	constexpr key F18								= {0x81};//	F18 key
	constexpr key F19								= {0x82};//	F19 key
	constexpr key F20								= {0x83};//	F20 key
	constexpr key F21								= {0x84};//	F21 key
	constexpr key F22								= {0x85};//	F22 key
	constexpr key F23								= {0x86};//	F23 key
	constexpr key F24								= {0x87};//	F24 key
	constexpr key unassigned0						= {0x88};//	Unassigned
	constexpr key unassigned1						= {0x89};//	Unassigned
	constexpr key unassigned2						= {0x8A};//	Unassigned
	constexpr key unassigned3						= {0x8B};//	Unassigned
	constexpr key unassigned4						= {0x8C};//	Unassigned
	constexpr key unassigned5						= {0x8D};//	Unassigned
	constexpr key unassigned6						= {0x8E};//	Unassigned
	constexpr key unassigned7						= {0x8F};//	Unassigned
	constexpr key num_lock							= {0x90};//	NUM LOCK key
	constexpr key scroll_lock						= {0x91};//	SCROLL LOCK key
	constexpr key oem_spec_0						= {0x92};//	OEM specific
	constexpr key oem_spec_1						= {0x93};//	OEM specific
	constexpr key oem_spec_2						= {0x94};//	OEM specific
	constexpr key oem_spec_3						= {0x95};//	OEM specific
	constexpr key oem_spec_4						= {0x96};//	OEM specific
	constexpr key unassigned8						= {0x97};//	Unassigned
	constexpr key unassigned9						= {0x98};//	Unassigned
	constexpr key unassigned10						= {0x99};//	Unassigned
	constexpr key unassigned11						= {0x9A};//	Unassigned
	constexpr key unassigned12						= {0x9B};//	Unassigned
	constexpr key unassigned13						= {0x9C};//	Unassigned
	constexpr key unassigned14						= {0x9D};//	Unassigned
	constexpr key unassigned15						= {0x9E};//	Unassigned
	constexpr key unassigned16						= {0x9F};//	Unassigned
	constexpr key shift_left						= {0xA0};//	Left SHIFT key
	constexpr key shift_right						= {0xA1};//	Right SHIFT key
	constexpr key control_left						= {0xA2};//	Left CONTROL key
	constexpr key control_right						= {0xA3};//	Right CONTROL key
	constexpr key alt_left							= {0xA4};//	Left MENU key
	constexpr key alt_right							= {0xA5};//	Right MENU key
	constexpr key browser_back						= {0xA6};//	Browser Back key
	constexpr key browser_forward					= {0xA7};//	Browser Forward key
	constexpr key browser_refresh					= {0xA8};//	Browser Refresh key
	constexpr key browser_stop						= {0xA9};//	Browser Stop key
	constexpr key browser_search					= {0xAA};//	Browser Search key
	constexpr key browser_favorites					= {0xAB};//	Browser Favorites key
	constexpr key browser_home						= {0xAC};//	Browser Start and Home key
	constexpr key volume_mute						= {0xAD};//	Volume Mute key
	constexpr key volume_down						= {0xAE};//	Volume Down key
	constexpr key volume_up							= {0xAF};//	Volume Up key
	constexpr key media_next_track					= {0xB0};//	Next Track key
	constexpr key media_prev_track					= {0xB1};//	Previous Track key
	constexpr key media_stop						= {0xB2};//	Stop Media key
	constexpr key media_play_pause					= {0xB3};//	Play/Pause Media key
	constexpr key launch_start_mail					= {0xB4};//	Start Mail key
	constexpr key launch_select_media				= {0xB5};//	Select Media key
	constexpr key launch_start_app1					= {0xB6};//	Start Application 1 key
	constexpr key launch_start_app2					= {0xB7};//	Start Application 2 key
	constexpr key reserved4							= {0xB8};//	Reserved
	constexpr key reserved5							= {0xB9};//	Reserved
	constexpr key oem_misc0							= {0xBA};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
	constexpr key oen_plus							= {0xBB};//	For any country/region, the '+' key
	constexpr key oen_comma							= {0xBC};//	For any country/region, the ',' key
	constexpr key oen_minus							= {0xBD};//	For any country/region, the '-' key
	constexpr key oen_period						= {0xBE};//	For any country/region, the '.' key
	constexpr key oem_misc1							= {0xBF};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
	constexpr key oem_misc2							= {0xC0};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
	constexpr key reserved6							= {0xC1};// Reserved
	constexpr key reserved7							= {0xC2};// Reserved
	constexpr key reserved8							= {0xC3};// Reserved
	constexpr key reserved9							= {0xC4};// Reserved
	constexpr key reserved10						= {0xC5};// Reserved
	constexpr key reserved11						= {0xC6};// Reserved
	constexpr key reserved12						= {0xC7};// Reserved
	constexpr key reserved13						= {0xC8};// Reserved
	constexpr key reserved14						= {0xC9};// Reserved
	constexpr key reserved15						= {0xCA};// Reserved
	constexpr key reserved16						= {0xCB};// Reserved
	constexpr key reserved17						= {0xCC};// Reserved
	constexpr key reserved18						= {0xCD};// Reserved
	constexpr key reserved19						= {0xCE};// Reserved
	constexpr key reserved20						= {0xCF};// Reserved
	constexpr key reserved21						= {0xD0};// Reserved
	constexpr key reserved22						= {0xD1};// Reserved
	constexpr key reserved23						= {0xD2};// Reserved
	constexpr key reserved24						= {0xD3};// Reserved
	constexpr key reserved25						= {0xD4};// Reserved
	constexpr key reserved26						= {0xD5};// Reserved
	constexpr key reserved27						= {0xD6};// Reserved
	constexpr key reserved28						= {0xD7};// Reserved
	constexpr key unassigned17						= {0xD8};// Unassigned
	constexpr key unassigned18						= {0xD9};// Unassigned
	constexpr key unassigned19						= {0xDA};// Unassigned
	constexpr key oem_misc3							= {0xDB};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
	constexpr key oem_misc4							= {0xDC};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
	constexpr key oem_misc5							= {0xDD};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
	constexpr key oem_misc6							= {0xDE};//	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
	constexpr key oem_misc7							= {0xDF};//	Used for miscellaneous characters; it can vary by keyboard.
	constexpr key reserved29						= {0xE0};// Reserved
	constexpr key oem_spec_5						= {0xE1};//	OEM specific
	constexpr key oem_angle_bracket_102				= {0xE2};//	Either the angle bracket key or the backslash key on the RT 102-key keyboard
	constexpr key oem_spec_6						= {0xE3};//	OEM specific
	constexpr key oem_spec_7						= {0xE4};//	OEM specific
	constexpr key ime_process						= {0xE5};//	IME PROCESS key
	constexpr key oem_spec_8						= {0xE6};//	OEM specific
	constexpr key packet							= {0xE7};//	Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
	constexpr key unassigned20						= {0xE8};//	Unassigned
	constexpr key oem_spec_9						= {0xE9};//	OEM specific
	constexpr key oem_spec_10						= {0xEA};//	OEM specific
	constexpr key oem_spec_11						= {0xEB};//	OEM specific
	constexpr key oem_spec_12						= {0xEC};//	OEM specific
	constexpr key oem_spec_13						= {0xED};//	OEM specific
	constexpr key oem_spec_14						= {0xEE};//	OEM specific
	constexpr key oem_spec_15						= {0xEF};//	OEM specific
	constexpr key oem_spec_16						= {0xF0};//	OEM specific
	constexpr key oem_spec_17						= {0xF1};//	OEM specific
	constexpr key oem_spec_18						= {0xF2};//	OEM specific
	constexpr key oem_spec_19						= {0xF3};//	OEM specific
	constexpr key oem_spec_20						= {0xF4};//	OEM specific
	constexpr key oem_spec_21						= {0xF5};//	OEM specific
	constexpr key attn								= {0xF6};//	Attn key
	constexpr key cr_sel							= {0xF7};//	CrSel key
	constexpr key ex_sel							= {0xF8};//	ExSel key
	constexpr key erase_eof							= {0xF9};//	Erase EOF key
	constexpr key play								= {0xFA};//	Play key
	constexpr key zoom								= {0xFB};//	Zoom key
	constexpr key PA1								= {0xFD};//	PA1 key
	constexpr key oem_clear							= {0xFE};//	Clear key
	constexpr key undefined10						= {0xFF};//	Undefined

	constexpr const char* key_to_raw_str[] = {
		"Undefined"
		,"Left mouse button"
		,"Right mouse button"
		,"Control-break processing"
		,"Middle mouse button (three-button mouse)"
		,"X1 mouse button"
		,"X2 mouse button"
		,"Undefined"
		,"BACKSPACE key"
		,"TAB key"
		,"Reserved"
		,"Reserved"
		,"CLEAR key"
		,"ENTER key"
		,"Undefined key"
		,"Undefined key"
		,"SHIFT key"
		,"CTRL key"
		,"ALT key"
		,"PAUSE key"
		,"CAPS LOCK key"
		,"IME Hanguel mode(maintained for compatibility; use VK_HANGUL) or IME Kana mode or IME Hangul mode"
		//,"IME Kana mode"
		//,"IME Hangul mode"
		,"IME On"
		,"IME Junja mode"
		,"IME final mode"
		,"IME Hanja mode or IME Kanji mode"
		//,"IME Kanji mode"
		,"IME Off"
		,"ESC key"
		,"IME convert"
		,"IME nonconvert"
		,"IME accept"
		,"IME mode change request"
		,"SPACEBAR"
		,"PAGE UP key"
		,"PAGE DOWN key"
		,"END key"
		,"HOME key"
		,"LEFT ARROW key"
		,"UP ARROW key"
		,"RIGHT ARROW key"
		,"DOWN ARROW key"
		,"SELECT key"
		,"PRINT key"
		,"EXECUTE key"
		,"PRINT SCREEN key"
		,"INS key"
		,"DEL key"
		,"HELP key"
		,"0 key"
		,"1 key"
		,"2 key"
		,"3 key"
		,"4 key"
		,"5 key"
		,"6 key"
		,"7 key"
		,"8 key"
		,"9 key"
		,"Undefined"
		,"Undefined"
		,"Undefined"
		,"Undefined"
		,"Undefined"
		,"Undefined"
		,"Undefined"
		,"A key"
		,"B key"
		,"C key"
		,"D key"
		,"E key"
		,"F key"
		,"G key"
		,"H key"
		,"I key"
		,"J key"
		,"K key"
		,"L key"
		,"M key"
		,"N key"
		,"O key"
		,"P key"
		,"Q key"
		,"R key"
		,"S key"
		,"T key"
		,"U key"
		,"V key"
		,"W key"
		,"X key"
		,"Y key"
		,"Z key"
		,"Left Windows key (Natural keyboard)"
		,"Right Windows key (Natural keyboard)"
		,"Applications key (Natural keyboard)"	
		,"Reserved"
		,"Computer Sleep key"
		,"Numeric keypad 0 key"
		,"Numeric keypad 1 key"
		,"Numeric keypad 2 key"
		,"Numeric keypad 3 key"
		,"Numeric keypad 4 key"
		,"Numeric keypad 5 key"
		,"Numeric keypad 6 key"
		,"Numeric keypad 7 key"
		,"Numeric keypad 8 key"
		,"Numeric keypad 9 key"
		,"Multiply key"
		,"Add key"
		,"Separator key"
		,"Subtract key"
		,"Decimal key"
		,"Divide key"
		,"F1 key"
		,"F2 key"
		,"F3 key"
		,"F4 key"
		,"F5 key"
		,"F6 key"
		,"F7 key"
		,"F8 key"
		,"F9 key"
		,"F10 key"
		,"F11 key"
		,"F12 key"
		,"F13 key"
		,"F14 key"
		,"F15 key"
		,"F16 key"
		,"F17 key"
		,"F18 key"
		,"F19 key"
		,"F20 key"
		,"F21 key"
		,"F22 key"
		,"F23 key"
		,"F24 key"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"NUM LOCK key"
		,"SCROLL LOCK key"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Left SHIFT key"
		,"Right SHIFT key"
		,"Left CONTROL key"
		,"Right CONTROL key"
		,"Left MENU key"
		,"Right MENU key"
		,"Browser Back key"
		,"Browser Forward key"
		,"Browser Refresh key"
		,"Browser Stop key"
		,"Browser Search key"
		,"Browser Favorites key"
		,"Browser Start and Home key"
		,"Volume Mute key"
		,"Volume Down key"
		,"Volume Up key"
		,"Next Track key"
		,"Previous Track key"
		,"Stop Media key"
		,"Play/Pause Media key"
		,"Start Mail key"
		,"Select Media key"
		,"Start Application 1 key"
		,"Start Application 2 key"
		,"Reserved"
		,"Reserved"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key"
		,"For any country/region, the '+' key"
		,"For any country/region, the ',' key"
		,"For any country/region, the '-' key"
		,"For any country/region, the '.' key"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Reserved"
		,"Unassigned"
		,"Unassigned"
		,"Unassigned"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\\|' key"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key"
		,"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key"
		,"Used for miscellaneous characters; it can vary by keyboard."
		,"Reserved"
		,"OEM specific"
		,"Either the angle bracket key or the backslash key on the RT 102-key keyboard"
		,"OEM specific"
		,"OEM specific"
		,"IME PROCESS key"
		,"OEM specific"
		,"Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM"
		,"Unassigned"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"OEM specific"
		,"Attn key"
		,"CrSel key"
		,"ExSel key"
		,"Erase EOF key"
		,"Play key"
		,"Zoom key"
		,"PA1 key"
		,"Clear key"
		,"Undefined"
	};
	
	constexpr const wchar_t* key_to_raw_wstr[] = {
		L"Undefined"
		,L"Left mouse button"
		,L"Right mouse button"
		,L"Control-break processing"
		,L"Middle mouse button (three-button mouse)"
		,L"X1 mouse button"
		,L"X2 mouse button"
		,L"Undefined"
		,L"BACKSPACE key"
		,L"TAB key"
		,L"Reserved"
		,L"Reserved"
		,L"CLEAR key"
		,L"ENTER key"
		,L"Undefined key"
		,L"Undefined key"
		,L"SHIFT key"
		,L"CTRL key"
		,L"ALT key"
		,L"PAUSE key"
		,L"CAPS LOCK key"
		,L"IME Hanguel mode(maintained for compatibility; use VK_HANGUL) or IME Kana mode or IME Hangul mode"
		//,L"IME Kana mode"
		//,L"IME Hangul mode"
		,L"IME On"
		,L"IME Junja mode"
		,L"IME final mode"
		,L"IME Hanja mode or IME Kanji mode"
		//,L"IME Kanji mode"
		,L"IME Off"
		,L"ESC key"
		,L"IME convert"
		,L"IME nonconvert"
		,L"IME accept"
		,L"IME mode change request"
		,L"SPACEBAR"
		,L"PAGE UP key"
		,L"PAGE DOWN key"
		,L"END key"
		,L"HOME key"
		,L"LEFT ARROW key"
		,L"UP ARROW key"
		,L"RIGHT ARROW key"
		,L"DOWN ARROW key"
		,L"SELECT key"
		,L"PRINT key"
		,L"EXECUTE key"
		,L"PRINT SCREEN key"
		,L"INS key"
		,L"DEL key"
		,L"HELP key"
		,L"0 key"
		,L"1 key"
		,L"2 key"
		,L"3 key"
		,L"4 key"
		,L"5 key"
		,L"6 key"
		,L"7 key"
		,L"8 key"
		,L"9 key"
		,L"Undefined"
		,L"Undefined"
		,L"Undefined"
		,L"Undefined"
		,L"Undefined"
		,L"Undefined"
		,L"Undefined"
		,L"A key"
		,L"B key"
		,L"C key"
		,L"D key"
		,L"E key"
		,L"F key"
		,L"G key"
		,L"H key"
		,L"I key"
		,L"J key"
		,L"K key"
		,L"L key"
		,L"M key"
		,L"N key"
		,L"O key"
		,L"P key"
		,L"Q key"
		,L"R key"
		,L"S key"
		,L"T key"
		,L"U key"
		,L"V key"
		,L"W key"
		,L"X key"
		,L"Y key"
		,L"Z key"
		,L"Left Windows key (Natural keyboard)"
		,L"Right Windows key (Natural keyboard)"
		,L"Applications key (Natural keyboard)"	
		,L"Reserved"
		,L"Computer Sleep key"
		,L"Numeric keypad 0 key"
		,L"Numeric keypad 1 key"
		,L"Numeric keypad 2 key"
		,L"Numeric keypad 3 key"
		,L"Numeric keypad 4 key"
		,L"Numeric keypad 5 key"
		,L"Numeric keypad 6 key"
		,L"Numeric keypad 7 key"
		,L"Numeric keypad 8 key"
		,L"Numeric keypad 9 key"
		,L"Multiply key"
		,L"Add key"
		,L"Separator key"
		,L"Subtract key"
		,L"Decimal key"
		,L"Divide key"
		,L"F1 key"
		,L"F2 key"
		,L"F3 key"
		,L"F4 key"
		,L"F5 key"
		,L"F6 key"
		,L"F7 key"
		,L"F8 key"
		,L"F9 key"
		,L"F10 key"
		,L"F11 key"
		,L"F12 key"
		,L"F13 key"
		,L"F14 key"
		,L"F15 key"
		,L"F16 key"
		,L"F17 key"
		,L"F18 key"
		,L"F19 key"
		,L"F20 key"
		,L"F21 key"
		,L"F22 key"
		,L"F23 key"
		,L"F24 key"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"NUM LOCK key"
		,L"SCROLL LOCK key"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Left SHIFT key"
		,L"Right SHIFT key"
		,L"Left CONTROL key"
		,L"Right CONTROL key"
		,L"Left MENU key"
		,L"Right MENU key"
		,L"Browser Back key"
		,L"Browser Forward key"
		,L"Browser Refresh key"
		,L"Browser Stop key"
		,L"Browser Search key"
		,L"Browser Favorites key"
		,L"Browser Start and Home key"
		,L"Volume Mute key"
		,L"Volume Down key"
		,L"Volume Up key"
		,L"Next Track key"
		,L"Previous Track key"
		,L"Stop Media key"
		,L"Play/Pause Media key"
		,L"Start Mail key"
		,L"Select Media key"
		,L"Start Application 1 key"
		,L"Start Application 2 key"
		,L"Reserved"
		,L"Reserved"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key"
		,L"For any country/region, the '+' key"
		,L"For any country/region, the ',' key"
		,L"For any country/region, the '-' key"
		,L"For any country/region, the '.' key"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Reserved"
		,L"Unassigned"
		,L"Unassigned"
		,L"Unassigned"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\\|' key"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key"
		,L"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key"
		,L"Used for miscellaneous characters; it can vary by keyboard."
		,L"Reserved"
		,L"OEM specific"
		,L"Either the angle bracket key or the backslash key on the RT 102-key keyboard"
		,L"OEM specific"
		,L"OEM specific"
		,L"IME PROCESS key"
		,L"OEM specific"
		,L"Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM"
		,L"Unassigned"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"OEM specific"
		,L"Attn key"
		,L"CrSel key"
		,L"ExSel key"
		,L"Erase EOF key"
		,L"Play key"
		,L"Zoom key"
		,L"PA1 key"
		,L"Clear key"
		,L"Undefined"
	};

	inline std::string to_string(key k) noexcept {
		return key_to_raw_str[k];
	}

	inline std::wstring to_wstring(const key k) noexcept {
		return key_to_raw_wstr[k];
	}

	// example using..
	//void test(key k) {
	//	switch (k) {
	//	case keys::arrow_down:
	//		break;
	//	case keys::arrow_right:
	//		break;
	//	}
	//}

}//namespace keys 

	class key_state {
	public:
		typedef float duration_type;
		static constexpr size_t key_count = 256;

	private:
		ZEE_DEFINE_REMOVE_MOVE_AND_COPY_FUNCTION(key_state);

	private:
		key_state() noexcept
			: keys_{}, durations_{} {
		}
	
		key keys_[key_count];
		duration_type durations_[key_count];
	
		static key_state inst_;
	
	public:
		static key_state& get() noexcept { return inst_; }
	
		static bool is_down(const key& k) noexcept {
			return get().keys_[k.value].value & 0xF0;
		}

		static bool is_up(const key& k) noexcept {
			return !is_down(k);
		}

		static bool is_pressed(const key& k) {
			return is_down(k) && key_duration(k) == 0.0f;
		}

		static bool is_released(const key& k) {
			return is_up(k) && key_duration(k) == 0.0f;
		}

		static bool is_toggle_on(const key& k) noexcept {
			return get().keys_[k.value].value & 0x0F;
		}
	
		static bool is_toggle_off(const key& k) noexcept {
			return !is_toggle_off(k);
		}
	
		static constexpr float max_duration = std::numeric_limits<duration_type>::max() * 0.001f;
	
		//ret == 0 ? (current key up or down) : (ret > 0 ? down duration : up duration)
		static duration_type key_duration(const key& k) noexcept {
			return get().durations_[k.value];
		}
	
		//Do not call this function directly.
		void internal_use_update(duration_type delta_time) noexcept;
	};

}//namespace zee 