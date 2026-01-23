#pragma once

#define ON		1				// ON
#define OFF		0				// OFF

/*** システム設定 ***/
#define DEF_SOUND_VALID			// サウンド有効
//#define DEF_JOYPAD_VALID		// JoyPad（DUALSHOCK4）有効

/*** ウィンドウ設定 ***/
#define WIN_MAX_X 660			// ウィンドウのX最大値
#define WIN_MAX_Y 450			// ウィンドウのY最大値
#define WIN_MIN_X 0				// ウィンドウのX最小値
#define WIN_MIN_Y 0				// ウィンドウのY最小値
#define WIN_POS_X 0				// ウィンドウの初期位置X
#define WIN_POS_Y 0				// ウィンドウの初期位置Y

/*** キーボード設定 ***/
#define DEF_KEY_PRESS_TIME 100	// キーボード長押し回数

/*** FPS設定 ***/
#define MicroSecond 1000000.0f	// 1マイクロ秒
#define MillSecond 1000.0f		// 1ミリ秒
#define WaitTimeMill 3000		// 最大で待てるミリ数
#define GameFPS 60				// 設定したいFPS

/*** シーン設定 ***/
enum SCE						// シーン種類
{
	SCE_00_TIT,					// タイトルシーン
	SCE_01_ACT,					// アクションシーン
	SCE_02_MAX
};

/*** タイトル設定 ***/
#define TIT_ENTER_POS_X 220		// 'Press Enter' X位置
#define TIT_ENTER_POS_Y 400		// 'Press Enter' Y位置

/*** ステージ設定 ***/
#define PIC_BACK 0								// Back
#define PIC_BLO1 1								// BLOCK1
#define PIC_BLO2 2								// BLOCK2
#define PIC_BLO3 3								// BLOCK3
#define PIC_BLO4 4								// BLOCK4
#define PIC_BLO5 5								// BLOCK5
#define PIC_MCHAR 6								// MChara
#define PIC_ENEM 7								// Enemy

#define CELL 30									// 1ブロックのサイズ[pix]
#define STG_X_MAX 6600							// ステージ最大X位置[pix]
#define STG_Y_MAX 450							// ステージ最大Y位置[pix]
#define STG_X_MIN 0								// ステージ最小X位置[pix]
#define STG_Y_MIN 0								// ステージ最小Y位置[pix]
#define STG_BLOCK_X_MAX (STG_X_MAX / CELL) 		// ステージ最大X位置[ブロック]
#define STG_BLOCK_Y_MAX (STG_Y_MAX / CELL) 		// ステージ最大Y位置[ブロック]
#define STG_FALL_Y (STG_Y_MAX - CELL)			// ステージ落下判定位置（ステージ最大Y位置-1CELL）[pix]

#define CHA_POS_X_INI (STG_X_MIN + 2 * CELL)	// キャラ初期位置X
#define CHA_POS_Y_INI (STG_Y_MAX - 3 * CELL)	// キャラ初期位置Y（下から3ブロック目）

#define ENEMY_HIT_RANGE	5						// 敵当たり判定閾値[pix]
#define ENE1_POS_X_INI	120						// 敵1初期位置X
#define ENE1_POS_Y_INI	(STG_Y_MAX - 3 * CELL)	// 敵1初期位置Y（下から3ブロック目）
#define ENE2_POS_X_INI	360						// 敵2初期位置X
#define ENE2_POS_Y_INI	(STG_Y_MAX - 3 * CELL)	// 敵2初期位置Y（下から3ブロック目）

#define DIR_NONE	0x00000000					// 移動方向なし
#define DIR_RI		0x00000001					// 右方向
#define DIR_LE		0x00000002					// 左方向
#define DIR_UP		0x00000004					// 上方向
#define DIR_DO		0x00000008					// 下方向

#define JUMP_OFF		0						// ジャンプなし
#define JUMP_UP			1						// ジャンプ上昇中
#define JUMP_GRAVITY	2						// 自由落下

#define MOVEX		(CELL / 10)					// X方向移動量（通常時）
#define MOVEX_D		(CELL / 5)					// X方向移動量（ダッシュ時）
#define MOVEY_H_MAX	(4 * CELL)					// ジャンプ時の最大高さ（4ブロック目）

#define GOAL_POS_X (2900 - WIN_MAX_X / 2)		// GOAL X位置(5400→2700に変更)

/*** クリアタイム ***/
enum RANK										// シーン種類
{
	RANK_1ST,
	RANK_2ND,
	RANK_3RD,
	RANK_4TH,
	RANK_MAX
};
#define RANK_TIME_INI	100.0f					// クリアタイム初期値[s]
#define RANK_POS_X		(WIN_MAX_X - 90)		// クリアタイム表示位置X
#define RANK_POS_Y		10						// クリアタイム表示位置Y
#define RANK_DISP_NUM	3						// クリアタイム表示数
#define TIME_POS_X		RANK_POS_X				// タイム表示位置X
#define TIME_POS_Y		(RANK_POS_Y + 30)		// タイム表示位置Y

#define FPS_POS_X		10						// FPS表示位置X
#define FPS_POS_Y		10						// FPS表示位置Y
#define COMD_POS_X		FPS_POS_X				// コマンド説明表示位置X
#define COMD_POS_Y		(FPS_POS_Y + 20)		// コマンド説明表示位置Y
