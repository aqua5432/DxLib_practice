#include "FPS.h"
#include "DxLib.h"
#include "ProjConf.h"

FPSClass Fps;
void FPSClass::FPSInit()
{
	FirsttakeTime = GetNowHiPerformanceCount();

	NowtakeTime = FirsttakeTime;
	OldtakeTime = FirsttakeTime;
	Deltatime = 0.000001f;
	FrameCount = 1;
	Average = 0.0f;

	return;
}

void FPSClass::FPSCheck() {
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

void FPSClass::FPSWait() {
	int wait = 0;
	wait = static_cast<int>(((MicroSecond / GameFPS * FrameCount) - (NowtakeTime - FirsttakeTime)) / MillSecond);	/* waitŽžŠÔ(msec) = —‘z‚ÌŽžŠÔ - ŽÀÛ‚ÌŽžŠÔ */

	if (wait > 0 && wait <= WaitTimeMill) {
		WaitTimer(wait);
	}
	return;
}
