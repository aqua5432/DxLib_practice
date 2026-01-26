#pragma once
#include "ProjConf.h"
#include "DxLib.h"

#ifdef DEF_SOUND_VALID
/*** SoundƒNƒ‰ƒX ***/
class SoundClass
{
public:
	int BgmSound;
	int JumpSound;

	void Read();
	void PlayBGMSound();
	void PlayJumpSound();
};

extern SoundClass Snd;
#endif /* DEF_SOUND_VALID */
