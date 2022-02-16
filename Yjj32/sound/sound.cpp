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
		mci_open_bgm.lpstrElementName = const_var_name; //���� ����
		mci_open_bgm.lpstrDeviceType = TEXT("waveaudio"); //mp3 ���� mpegvideo
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mci_open_bgm);
		dw_ID = mci_open_bgm.wDeviceID;
		mciSendCommand(dw_ID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mci_open_bgm); //���� �ݺ� ���
	}


	void sound::play_sound_effect(const WCHAR* const_var_name) {
		mci_open_sound_effect.lpstrElementName = const_var_name;
		mci_open_sound_effect.lpstrDeviceType = TEXT("waveaudio");
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mci_open_sound_effect);
  		dw_ID = mci_open_sound_effect.wDeviceID;
		mciSendCommand(dw_ID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mci_open_sound_effect); //������ �� �� ���
		//Sleep(0); //ȿ������ ����� ������ �����ߴٰ�
		mciSendCommand(dw_ID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL); //���� ��� ��ġ�� ó������ �ʱ�ȭ
	}
}