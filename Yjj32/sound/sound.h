
// https://kiffblog.tistory.com/151
#pragma once
#pragma comment (lib, "winmm.lib") //����
#include "../../YjjCore/public/core/core.h"
#include <windows.h> //Ű���� �Է�
#include <mmsystem.h> //����
#include <Digitalv.h> //����

namespace yjj {
	class sound {
	public:
		MCI_OPEN_PARMS mci_open_bgm;
		MCI_PLAY_PARMS mci_play_bgm;
		MCI_OPEN_PARMS mci_open_sound_effect;
		MCI_PLAY_PARMS mci_play_sound_effect;
		int dw_ID;

		static sound& get();

		//const TCHAR* sound_bgm = TEXT("../assets/bgm/bgm.mp3"); //BGM ��� ����
		//const TCHAR* sound_explosion = TEXT("../assets/sound_effect/Explosion5.wav"); //BGM ��� ����
		//const TCHAR* sound_hit = TEXT("../assets/sound_effect/Hit_Hurt14.wav"); //BGM ��� ����
		//const TCHAR* sound_shoot = TEXT("../assets/sound_effect/Laser_Shoot5.wav"); //BGM ��� ����
		//const TCHAR* sound_item = TEXT("../assets/sound_effect/Pickup_Coin2.wav"); //BGM ��� ����

		void play_bgm(const WCHAR* const_var_name);

		void play_sound_effect(const WCHAR* const_var_name);

		sound() = default;

	private:

	};
}