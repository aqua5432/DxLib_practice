#pragma once
#include "ProjConf.h"

/*** Colorクラス ***/
class
{
public:
	int Black;
	int Red;
	int Green;
	int Blue;
	int White;

	void Read()
	{
		White = GetColor(255, 255, 255);
		Red = GetColor(255, 0, 0);
		Green = GetColor(0, 255, 0);
		Blue = GetColor(0, 0, 255);
		Black = GetColor(0, 0, 0);
	}
private:

}Col;

#ifdef DEF_SOUND_VALID
/*** Soundクラス ***/
class
{
public:
	int BgmSound;
	int JumpSound;

	void Read()
	{
		BgmSound = LoadSoundMem("./sound/bgm.mp3");
		JumpSound = LoadSoundMem("./sound/jump.mp3");
		ChangeVolumeSoundMem(255 * 50 / 100, BgmSound);		// 音量を50%に調整
		ChangeVolumeSoundMem(255 * 100 / 100, JumpSound);	// 音量を100%に調整
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

/*** FPSクラス ***/
class
{
public:
	LONGLONG FirsttakeTime = 0;		// 1フレーム目の計測時間
	LONGLONG NowtakeTime = 0;		// 現在の計測時間
	LONGLONG OldtakeTime = 0;		// 以前の計測時間

	float Deltatime = 0.000001f;	// デルタタイム（経過時間）
	int FrameCount = 1;				// 現在のフレーム数（1フレーム目からMAXフレーム目まで）
	float Average = 0.0f;			// 平均のFPS値

	void FPSInit() {
		FirsttakeTime = GetNowHiPerformanceCount();

		NowtakeTime = FirsttakeTime;
		OldtakeTime = FirsttakeTime;
		Deltatime = 0.000001f;
		FrameCount = 1;
		Average = 0.0f;

		return;
	}

	void FPSCheck() {
		NowtakeTime = GetNowHiPerformanceCount();
		Deltatime = (NowtakeTime - OldtakeTime) / MicroSecond;
		OldtakeTime = NowtakeTime;

		if (FrameCount == GameFPS)
		{
			LONGLONG TotalFrameTIme = NowtakeTime - FirsttakeTime;
			float CalcAverage = static_cast<float>(TotalFrameTIme) / GameFPS;
			Average = MicroSecond / CalcAverage;
			FirsttakeTime = GetNowHiPerformanceCount();
			FrameCount = 1;
		}
		else
		{
			FrameCount++;
		}
		return;
	}

	void FPSWait() {
		int wait = 0;
		wait = static_cast<int>(((MicroSecond / GameFPS * FrameCount) - (NowtakeTime - FirsttakeTime)) / MillSecond);	/* wait時間(msec) = 理想の時間 - 実際の時間 */

		if (wait > 0 && wait <= WaitTimeMill) {
			WaitTimer(wait);
		}
		return;
	}
private:

}Fps;
