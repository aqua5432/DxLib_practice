#include "Title.h"
#include "../system/Key.h"
#include "../system/Font.h"
#include "../system/Color.h"
#include "../config/ProjConf.h"
#include "../system/Picture.h"
#include "DxLib.h"
#include "../game/Stage.h"

TitleClass Tit;

int TitleClass::Out() {
	int ret = SCE::SCE_00_TIT;

	/*** タイトル画面の描画 ***/
	DrawGraph(0, 0, Pic.Title, TRUE);

	/*** Press Enterの描画 ***/
	DrawFormatStringFToHandle(TIT_ENTER_POS_X, TIT_ENTER_POS_Y, Col.White, Fon.FH[30], "Press Enter");

	/*** クリアタイムの描画 ***/
	for (int i = 0; i < RANK_DISP_NUM; i++)
	{
		DrawFormatStringFToHandle(RANK_POS_X, static_cast<float>(RANK_POS_Y + i * 10), Col.White, Fon.FH[10], "No.%d:%6.2f s", i + 1, Sta.Rank[i]);
	}

	/*** アクションシーンに移行 ***/
	if ((Key.input[KEY_INPUT_RETURN] > 0))	// Enter押下
	{
		Sta.StartCount = GetNowCount(); // 開始カウント設定
		ret = SCE::SCE_01_ACT; // アクションシーンに移行
	}
	return ret;
}