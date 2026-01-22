#pragma once

/*** Fontクラス ***/
class
{
public:
	int FH[30 + 1];

	void Read()
	{
		for (int i = 0; i < 30 + 1; i++)
		{
			FH[i] = CreateFontToHandle("ＭＳ　ゴシック", i, 6, DX_FONTTYPE_NORMAL);
		}
	}
private:

}Fon;

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
		BgmSound = LoadSoundMem("./sound/bgm.wav");
		JumpSound = LoadSoundMem("./sound/jump.wav");
		ChangeVolumeSoundMem(255 * 20 / 100, BgmSound);		// 音量を20%に調整
		ChangeVolumeSoundMem(255 * 20 / 100, JumpSound);	// 音量を20%に調整
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

/*** Keyクラス ***/
class
{
public:
	int input[256];		// キーボード入力情報

	int GetKey()
	{
		char allkey[256];
		GetHitKeyStateAll(allkey);
		for (int i = 0; i < 256; i++)
		{
			if (allkey[i] == 1) // 特定のキーは押されているか
			{
				if (input[i] < DEF_KEY_PRESS_TIME) // 長押し上限まで押されているかどうか
				{
					input[i] = input[i] + 1; // 保存
				}
			}
			else if (allkey[i] == 0) // 特定のキーは押されていないか
			{
				input[i] = 0;
			}
		}
		return 0;
	}
private:

}Key;

#ifdef DEF_JOYPAD_VALID
/*** Joypadクラス ***/
class
{
public:
	DINPUT_JOYSTATE input;				// JoyPad入力情報
	unsigned char input_X_Z1 = 0;		// JoyPad「×」前回値

private:

}JPad;
#endif /* DEF_JOYPAD_VALID */
