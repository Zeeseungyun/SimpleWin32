
// https://kiffblog.tistory.com/151
#pragma once
#pragma comment (lib, "winmm.lib") //음악
#include "../../YjjCore/public/core/core.h"
#include <windows.h> //키보드 입력
#include <mmsystem.h> //음악
#include <Digitalv.h> //음악

namespace yjj {
	class sound {
	public:
		MCI_OPEN_PARMS mci_open_bgm;
		MCI_PLAY_PARMS mci_play_bgm;
		MCI_OPEN_PARMS mci_open_sound_effect;
		MCI_PLAY_PARMS mci_play_sound_effect;
		int dw_ID;

		static sound& get();

		//const TCHAR* sound_bgm = TEXT("../assets/bgm/bgm.mp3"); //BGM 경로 지정
		//const TCHAR* sound_explosion = TEXT("../assets/sound_effect/Explosion5.wav"); //BGM 경로 지정
		//const TCHAR* sound_hit = TEXT("../assets/sound_effect/Hit_Hurt14.wav"); //BGM 경로 지정
		//const TCHAR* sound_shoot = TEXT("../assets/sound_effect/Laser_Shoot5.wav"); //BGM 경로 지정
		//const TCHAR* sound_item = TEXT("../assets/sound_effect/Pickup_Coin2.wav"); //BGM 경로 지정

		void play_bgm(const WCHAR* const_var_name);

		void play_sound_effect(const WCHAR* const_var_name);

		sound() = default;

	private:

	};
}