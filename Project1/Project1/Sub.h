#pragma once
#include "ProjConf.h"

#ifdef DEF_SOUND_VALID
/*** SoundÉNÉâÉX ***/
class
{
public:
	int BgmSound;
	int JumpSound;

	void Read()
	{
		BgmSound = LoadSoundMem("./sound/bgm.mp3");
		JumpSound = LoadSoundMem("./sound/jump.mp3");
		ChangeVolumeSoundMem(255 * 50 / 100, BgmSound);		// âπó Ç50%Ç…í≤êÆ
		ChangeVolumeSoundMem(255 * 100 / 100, JumpSound);	// âπó Ç100%Ç…í≤êÆ
	}

	void PlayBGMSound()
	{
		PlaySoundMem(BgmSound, DX_PLAYTYPE_LOOP);
	}

	void PlayJumpSound()
	{
		PlaySoundMem(JumpSound, DX_PLAYTYPE_BACK);
	}
private:

}Snd;
#endif /* DEF_SOUND_VALID */
