#pragma once
#include "DxLib.h"

/*** FPSクラス ***/
class FPSClass
{
public:
	LONGLONG FirsttakeTime = 0;		// 1フレーム目の計測時間
	LONGLONG NowtakeTime = 0;		// 現在の計測時間
	LONGLONG OldtakeTime = 0;		// 以前の計測時間

	float Deltatime = 0.000001f;	// デルタタイム（経過時間）
	int FrameCount = 1;				// 現在のフレーム数（1フレーム目からMAXフレーム目まで）
	float Average = 0.0f;			// 平均のFPS値

	void FPSInit();
	void FPSCheck();
	void FPSWait();
};

extern FPSClass Fps;
