#include "sound.h"

namespace yjj {
	sound& sound::get() {
		static std::unique_ptr<sound> inst;
		if (!inst) {
			inst = std::unique_ptr<sound>(new sound());
		}
		return *inst;
	}

	void sound::play_bgm(const WCHAR* const_var_name) {
		mci_open_bgm.lpstrElementName = const_var_name; //파일 오픈
		mci_open_bgm.lpstrDeviceType = TEXT("waveaudio"); //mp3 형식 mpegvideo
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mci_open_bgm);
		dw_ID = mci_open_bgm.wDeviceID;
		mciSendCommand(dw_ID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mci_open_bgm); //음악 반복 재생
	}


	void sound::play_sound_effect(const WCHAR* const_var_name) {
		mci_open_sound_effect.lpstrElementName = const_var_name;
		mci_open_sound_effect.lpstrDeviceType = TEXT("waveaudio");
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mci_open_sound_effect);
  		dw_ID = mci_open_sound_effect.wDeviceID;
		mciSendCommand(dw_ID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mci_open_sound_effect); //음악을 한 번 재생
		//Sleep(0); //효과음이 재생될 때까지 정지했다가
		mciSendCommand(dw_ID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL); //음원 재생 위치를 처음으로 초기화
	}
}