#pragma once
#include <vector>
#include "Entity.h"
#include "InputSystem.h"
#include "EnemySystem.h"
#include "Physics.h"
#include "Renderer.h"

class ACTION
{
public:
	ACTION()
	{
		Init();
	}

	/*** Actシーン本処理 ***/
	int Out()
	{
		int ret = SCE::SCE_01_ACT;

		Update();
		Judge();
		Cal();
		Draw();
		return NextScene(ret);
	}

	void Update()
	{
		if (IsClearing) return;

		MainChar.Dir = DIR_NONE;
		MainChar.Touch = DIR_NONE;

		/*** メインキャラの四角座標情報更新 ***/
		MainChar.Cor.RiUp.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y];
		MainChar.Cor.RiDo.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL - 1];
		MainChar.Cor.LeUp.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y];
		MainChar.Cor.LeDo.Ce = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL - 1];
	}

	void Judge()
	{
		if (IsClearing) return;

		InputState in = inputSystem.Update();

		/*** ダッシュ判定 ***/
		if ((in.dash) )// F押下（長押しも有効）
		{
			Mov.Dash = ON;
			Mov.X = MOVEX_D;	// X方向移動量（ダッシュ時）
		}
		else
		{
			Mov.Dash = OFF;
			Mov.X = MOVEX;		// X方向移動量（通常時）
		}

		/*** 接触ブロック予測判定 ***/
		CheckBlockCollision();

		/*** 移動方向判定 ***/
		UpdateDirection(in);

		/*** ゴール判定 ***/
		if (abs(Sta_PosX) > GOAL_POS_X)
		{
			Goal = TRUE;
			IsClearing = true;
		}

		/*** 終了判定 ***/
		if ((in.exit) ||	// Esc押下
			(MainChar.Fall == TRUE) ||				// 落下判定
			(Goal == TRUE)							// ゴール判定
			)
		{
			EndFlag = TRUE;
		}

		/*敵との衝突判定による終了判定*/
		for (auto& e : Enemies)
		{
			if (e.Touch != DIR_NONE)
				EndFlag = TRUE;
		}

	}

	/*** 移動計算 ***/
	void Cal()
	{
		if (IsClearing) return;

		enemySystem.Update(Enemies, MainChar, Sta_PosX);
		physics.Update(MainChar, Mov, Sta_PosX, Sta);
	}

	//描画計算
	void Draw() 
	{
		renderer.DrawStage(Sta, Sta_PosX);
		renderer.DrawPlayer(MainChar);
		renderer.DrawEnemies(Enemies, Sta_PosX);
		renderer.DrawUI(Sta);
	}

	int NextScene(int ret) {
		/*** ENDフラグ有効時、タイトルシーンに移行 ***/
		if (EndFlag == TRUE)
		{

			if (Goal == TRUE)
			{
				if (ClearWait == 0) {
					/*** クリアタイム更新 ***/
					Sta.UpdateTime();
				}
				ClearWait++;

				DrawString(300, 200, "GAME CLEAR", Col.Green);

				if (ClearWait < 120) return ret;  // 2秒待つ
			}

			/*** Actシーン終了時初期化 ***/
			Init();

			/*** タイトルシーンに移行 ***/
			ret = SCE::SCE_00_TIT;
		}
		return ret;
	}

	void UpdateDirection(const InputState& in) {
		if ((in.jump)	// W(or↑)押下（長押しは無効）
			)
		{
			if ((Mov.JumpState == JUMP_OFF) && ((MainChar.Touch & DIR_DO) == DIR_DO)) // ジャンプしていないとき、かつ地面に接してるとき
			{
				Mov.JumpState = JUMP_UP; // ジャンプする

#ifdef DEF_SOUND_VALID
				/*** Jump音再生 ***/
				Snd.PlayJumpSound();
#endif /*  DEF_SOUND_VALID */
			}
		}
		else if ((in.moveRight) // D(or→)押下（長押しも有効）
			)
		{
			MainChar.Dir = DIR_RI;		// メインキャラ右向き
			MainChar.PicDir = DIR_RI;	// メインキャラ右向き（描画用）
		}
		else if ((in.moveLeft) // A(or←)押下（長押しも有効）
			)
		{
			MainChar.Dir = DIR_LE;		// メインキャラ左向き
			MainChar.PicDir = DIR_LE;	// メインキャラ左向き（描画用）
		}
	}

	void CheckBlockCollision() {
		/* メインキャラの四角の左右は、Mov.Xをもとに接触予測 */
		MainChar.Cor.RiUp.Ri = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1 + Mov.X][MainChar.Pos.Y];
		MainChar.Cor.RiDo.Ri = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1 + Mov.X][MainChar.Pos.Y + CELL - 1];
		MainChar.Cor.LeUp.Le = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX - Mov.X][MainChar.Pos.Y];
		MainChar.Cor.LeDo.Le = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX - Mov.X][MainChar.Pos.Y + CELL - 1];

		/* メインキャラの四角の上下は、Y位置の1pixel移動した場合の接触予測 */
		MainChar.Cor.RiUp.Up = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1];
		MainChar.Cor.LeUp.Up = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1];
		MainChar.Cor.RiDo.Do = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL];
		MainChar.Cor.LeDo.Do = Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL];

		/*** 接触予測判定 ***/
		// ブロック接触判定 右
		if (MainChar.Cor.RiUp.Ri != PIC_BACK || MainChar.Cor.RiDo.Ri != PIC_BACK)	// 右に行こうとした際、右上、右下の右側の属性がBack以外になるか
		{
			MainChar.Touch |= DIR_RI;
		}
		// ブロック接触判定 左
		if (MainChar.Cor.LeUp.Le != PIC_BACK || MainChar.Cor.LeDo.Le != PIC_BACK)	// 左に行こうとした際、左上、左下の左側の属性がBack以外になるか
		{
			MainChar.Touch |= DIR_LE;
		}
		// ブロック接触判定 上
		if (MainChar.Cor.RiUp.Up != PIC_BACK || MainChar.Cor.LeUp.Up != PIC_BACK)	// 上に行こうとした際、右上、左上の上側の属性がBack以外になるか
		{
			MainChar.Touch |= DIR_UP;
		}
		// ブロック接触判定 下
		if (MainChar.Cor.RiDo.Do != PIC_BACK || MainChar.Cor.LeDo.Do != PIC_BACK)	// 下に行こうとした際、右下、左下の下側の属性がBack以外になるか
		{
			MainChar.Touch |= DIR_DO;
		}
	}

	/*** 初期化 ***/
	void Init()
	{
		Sta_PosX = STG_X_MIN;

		MainChar.Pos.X = CHA_POS_X_INI;
		MainChar.Pos.Y = CHA_POS_Y_INI;
		MainChar.Pos.Yin = CHA_POS_Y_INI;
		MainChar.Dir = DIR_NONE;
		MainChar.PicDir = DIR_RI;
		MainChar.Touch = DIR_NONE;
		MainChar.Fall = FALSE;
		Mov.JumpState = JUMP_OFF;
		Mov.Y = 0;
		physics.ResetCou();

		Enemies.clear();

		Enemy e1;
		e1.Pos.X = ENE1_POS_X_INI;
		e1.Pos.Y = ENE1_POS_Y_INI;
		e1.Dir = DIR_RI;
		e1.Touch = DIR_NONE;
		Enemies.push_back(e1);

		Enemy e2;
		e2.Pos.X = ENE2_POS_X_INI;
		e2.Pos.Y = ENE2_POS_Y_INI;
		e2.Dir = DIR_RI;
		e2.Touch = DIR_NONE;
		Enemies.push_back(e2);

		Goal = FALSE;
		EndFlag = FALSE;

		IsClearing = false;
		ClearWait = 0;
	}

	// ステージX座標
	int Sta_PosX = STG_X_MIN;

	MainCharacter MainChar;
	std::vector<Enemy> Enemies;
	MoveState Mov;
	Renderer renderer;
	Physics physics;
	EnemySystem enemySystem;
	InputSystem inputSystem;

private:
	int Goal = FALSE;
	int EndFlag = FALSE;
	int ClearWait = 0;
	bool IsClearing = false;

}Act;
