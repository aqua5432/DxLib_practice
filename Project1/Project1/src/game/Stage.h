#pragma once
#pragma warning(disable:4996)
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "DxLib.h"
#include "../config/ProjConf.h"
#include "../system/Picture.h"

/*** Stageクラス ***/
class Stage
{
public:
	void Read();
	void UpdateTime();
	void Out(int* PosX) const;

	// ステージ座標
	struct
	{
		int Blo[STG_BLOCK_X_MAX][STG_BLOCK_Y_MAX];
		int Pix[STG_X_MAX][STG_Y_MAX];
	}Cood;

	// ステージクリアタイム
	float Rank[RANK_MAX] = { RANK_TIME_INI, RANK_TIME_INI, RANK_TIME_INI, RANK_TIME_INI };

	// 開始カウント
	int StartCount = 0;
};
extern Stage Sta;
