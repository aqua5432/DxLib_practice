#include "Sound.h"

#ifdef DEF_SOUND_VALID

#include "DxLib.h"

SoundClass Snd;

void SoundClass::Read() {
	BgmSound = LoadSoundMem("./sound/bgm.mp3");
	JumpSound = LoadSoundMem("./sound/jump.mp3");
	ChangeVolumeSoundMem(255 * 50 / 100, BgmSound);		// Й╣Ч╩ВЁ50%В╔Т▓Ро
	ChangeVolumeSoundMem(255 * 100 / 100, JumpSound);	// Й╣Ч╩ВЁ100%В╔Т▓Ро
}

void SoundClass::PlayBGMSound() {
	PlaySoundMem(BgmSound, DX_PLAYTYPE_LOOP);
}

void SoundClass::PlayJumpSound() {
	PlaySoundMem(JumpSound, DX_PLAYTYPE_BACK);
}

#endif /* DEF_SOUND_VALID */