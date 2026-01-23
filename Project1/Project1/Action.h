#pragma once
#include <vector>
#include "Entity.h"
#include "InputSystem.h"
#include "EnemySystem.h"
#include "Physics.h"
#include "Renderer.h"

/*
自身の加えた改良点

・元コードでは敵の移動速度がプレイヤー入力に依存していたため、
　空中移動時に敵挙動が不安定になる問題があった。

・これを敵自身の速度パラメータを持たせる設計に変更し、
　プレイヤー状態に依存しない独立したAI移動に修正した。

・その際、Playerがジャンプ左右移動をしたとき、敵もその流れに合わせて移動していく不具合が発生した。
・敵がスクリーン座標で管理されており、ステージスクロールがワールド座標として別管理されているため、座標系の混在によって発生していました。
　本来は全オブジェクトをワールド座標で持ち、描画時にのみカメラ変換を行うべき設計です。

・ワールド、スクリーン、カメラ座標を適切に戻した時、床をすり抜ける／空中で止まる／突然落下死する不具合が発生
画面座標とステージ座標が混在していた。

描画：画面座標
当たり判定：ステージ座標

なのに、判定で

Sta.Cood.Pix[MainChar.Pos.X][MainChar.Pos.Y]
のように 変換せず直接参照していた。

→画面座標 → ワールド座標へ変換してから判定する

・敵が画面から置き去りにされる

プレイヤーが進むと敵が画面外に消える
追いかけてこない

原因

敵の座標を ワールド固定に変更したが、行動範囲が固定のままだった。

元コードでは：敵の移動範囲 = [0, WIN_MAX_X]

→敵はワールド座標のまま、行動範囲だけをカメラ基準に変更。

*/

/*** Actionクラス ***/
class ACTION
{
public:
	ACTION()
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
		//Cou = 0;
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
	}

	/*** 更新用関数 ***/
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

	/*** 判定 ***/
	void Judge()
	{
		if (IsClearing) return;

		/*** ダッシュ判定 ***/
		if ((Key.input[KEY_INPUT_F] > 0) // F押下（長押しも有効）
#ifdef DEF_JOYPAD_VALID
			|| (JPad.input.Buttons[0] == 128) // JoyPad:「□」押下
#endif /* DEF_JOYPAD_VALID */
			)
		{
			Mov.Dash = ON;
			Mov.X = MOVEX_D;	// X方向移動量（ダッシュ時）
		}
		else
		{
			Mov.Dash = OFF;
			Mov.X = MOVEX;		// X方向移動量（通常時）
		}

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

		/*** 移動方向判定 ***/
		if ((Key.input[KEY_INPUT_W] == 1 || Key.input[KEY_INPUT_UP] == 1)	// W(or↑)押下（長押しは無効）
#ifdef DEF_JOYPAD_VALID
			|| ((JPad.input.Buttons[1] == 128) && (JPad.input_X_Z1 == 0)) // JoyPad「×」押下（JPad.input.Buttons[1]==128）
#endif /* DEF_JOYPAD_VALID */
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
		else if ((Key.input[KEY_INPUT_D] > 0 || Key.input[KEY_INPUT_RIGHT] > 0) // D(or→)押下（長押しも有効）
#ifdef DEF_JOYPAD_VALID
			|| (JPad.input.POV[0] > 0 && JPad.input.POV[0] < 18000) // JoyPad「→」押下
#endif /* DEF_JOYPAD_VALID */
			)
		{
			MainChar.Dir = DIR_RI;		// メインキャラ右向き
			MainChar.PicDir = DIR_RI;	// メインキャラ右向き（描画用）
		}
		else if ((Key.input[KEY_INPUT_A] > 0 || Key.input[KEY_INPUT_LEFT] > 0) // A(or←)押下（長押しも有効）
#ifdef DEF_JOYPAD_VALID
			|| (JPad.input.POV[0] > 18000 && JPad.input.POV[0] < 36000) // JoyPad「←」押下
#endif /* DEF_JOYPAD_VALID */
			)
		{
			MainChar.Dir = DIR_LE;		// メインキャラ左向き
			MainChar.PicDir = DIR_LE;	// メインキャラ左向き（描画用）
		}

		/*** ゴール判定 ***/
		if (abs(Sta_PosX) > GOAL_POS_X)
		{
			Goal = TRUE;
			IsClearing = true;
		}

		/*** 終了判定 ***/
		if ((Key.input[KEY_INPUT_ESCAPE] > 0) ||	// Esc押下
			(MainChar.Fall == TRUE) ||				// 落下判定
			(Goal == TRUE)							// ゴール判定
#ifdef DEF_JOYPAD_VALID
			|| (JPad.input.Buttons[12] == 128)		// JoyPad「PS」押下
#endif /* DEF_JOYPAD_VALID */
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

		for (auto& e : Enemies)
		{
			float dist = abs(MainChar.Pos.X - e.Pos.X);
			float speed = 1.5f + dist * 0.001f;
			speed = min(speed, 4.0f);

			if (e.Dir == DIR_RI)
				e.Pos.X += e.SpeedX * speed;
			else
				e.Pos.X -= e.SpeedX * speed;

			int camLeft = -Sta_PosX;
			int camRight = -Sta_PosX + WIN_MAX_X;

			if (e.Pos.X <= camLeft)
				e.Dir = DIR_RI;
			if (e.Pos.X >= camRight - CELL)
				e.Dir = DIR_LE;

			/*** 敵接触判定右 ***/
			if ((MainChar.Pos.X - Sta_PosX + CELL >= e.Pos.X + ENEMY_HIT_RANGE) &&
				(MainChar.Pos.X - Sta_PosX + CELL < e.Pos.X + CELL) &&
				(MainChar.Pos.Y >= e.Pos.Y) &&
				(MainChar.Pos.Y < e.Pos.Y + CELL))
			{
				e.Touch = DIR_RI;
			}
			/*** 敵接触判定左 ***/
			else if ((MainChar.Pos.X - Sta_PosX >= e.Pos.X) &&
				(MainChar.Pos.X - Sta_PosX < e.Pos.X + CELL - ENEMY_HIT_RANGE) &&
				(MainChar.Pos.Y >= e.Pos.Y) &&
				(MainChar.Pos.Y < e.Pos.Y + CELL))
			{
				e.Touch = DIR_LE;
			}
		}

		for (int i = 0; i < Enemies.size(); i++)
		{
			for (int j = i + 1; j < Enemies.size(); j++)
			{
				if (abs(Enemies[i].Pos.X - Enemies[j].Pos.X) < CELL)
				{
					std::swap(Enemies[i].Dir, Enemies[j].Dir);
				}
			}
		}

		physics.Update(MainChar, Mov, Sta_PosX, Sta);
	}

	/*** Actシーン終了時初期化 ***/
	void EndInit()
	{
		/*** 初期化 ***/
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
		//Cou = 0;
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

	/*** Actシーン本処理 ***/
	int Out()
	{
		int ret = SCE::SCE_01_ACT;

		/*** 更新 ***/
		Update();

		/*** 判定 ***/
		Judge();

		/*** 移動計算 ***/
		Cal();

		renderer.DrawStage(Sta, Sta_PosX);
		renderer.DrawPlayer(MainChar);
		renderer.DrawEnemies(Enemies, Sta_PosX);
		renderer.DrawUI(Sta);


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
			EndInit();

			/*** タイトルシーンに移行 ***/
			ret = SCE::SCE_00_TIT;
		}

		return ret;
	}

	// ステージX座標
	int Sta_PosX = STG_X_MIN;

	MainCharacter MainChar;
	std::vector<Enemy> Enemies;
	MoveState Mov;
	Renderer renderer;
	Physics physics;

private:
	int Goal = FALSE;
	int EndFlag = FALSE;
	int ClearWait = 0;
	bool IsClearing = false;

}Act;
