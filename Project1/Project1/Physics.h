#pragma once
#include "Entity.h"

class Physics { // 物理挙動(移動・ジャンプ)を担当
	// 2次関数ジャンプ用変数
	int Cou = 0;
	double T = 0;
	const double T_k = 20.0;
public:
	void ResetCou() {
		Cou = 0;
	}
	void Update(MainCharacter& MainChar, MoveState& Mov, int& Sta_PosX, Stage& Sta) {
		/*** X方向移動計算 ***/
		/* 右移動 */
		if (MainChar.Dir == DIR_RI)
		{
			if ((MainChar.Touch & DIR_RI) != DIR_RI) // 右に行こうとしても接触しないか
			{
				if (MainChar.Pos.X < WIN_MAX_X / 2)														// キャラのX位置がウィンドウ中央より左の場合
				{
					MainChar.Pos.X = MainChar.Pos.X + Mov.X;											// キャラを右に動かす
				}
				else if ((MainChar.Pos.X >= WIN_MAX_X / 2) && (abs(Sta_PosX) < STG_X_MAX - WIN_MAX_X))	// キャラのX位置がウィンドウ中央以上右で、ステージの一番右に到達していない場合
				{
					Sta_PosX = Sta_PosX - Mov.X;														// ステージを左に動かす
				}
			}
		}
		/* 左移動 */
		else if (MainChar.Dir == DIR_LE)
		{
			if ((MainChar.Touch & DIR_LE) != DIR_LE) // 左に行こうとしても接触しないか
			{
				if (MainChar.Pos.X > STG_X_MIN)										// キャラが一番左にいない場合（キャラのX位置がステージの一番左より右）
				{
					MainChar.Pos.X = MainChar.Pos.X - Mov.X;						// キャラを左に動かす
				}
				else if ((MainChar.Pos.X <= STG_X_MIN) && (Sta_PosX < STG_X_MIN))	// キャラが一番左にいて、ステージも一番左ではない場合（キャラのX位置がステージの一番左以下、かつステージ位置も一番左ではない）
				{
					Sta_PosX = Sta_PosX + Mov.X;									// ステージを右に動かす
				}
			}
		}

		/*** Y方向移動計算 ***/
		/* ジャンプ上昇 */
		if (Mov.JumpState == JUMP_UP)
		{
			Cou = Cou + 1;
			T = T_k * ((double)Cou / 60.0);
			// HUMAN PosY cal
			Mov.Y = (int)(pow(T, 2.0));
			for (int y = 1; y <= Mov.Y; y++)
			{
				if ((MOVEY_H_MAX == MainChar.Pos.Yin - MainChar.Pos.Y) ||									// ジャンプの最大高さか
					(Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1] != PIC_BACK) ||			// 左上に何かある
					(Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1] != PIC_BACK))	// 右上に何かある
				{
					Mov.JumpState = JUMP_GRAVITY;				// 自由落下に移行
					Cou = 0;
					break;
				}
				else
				{
					MainChar.Pos.Y = MainChar.Pos.Y - 1;
				}
			}
		}
		/* 自由落下または接触判定（下）なし */
		else if ((Mov.JumpState == JUMP_GRAVITY) || (MainChar.Touch & DIR_DO) != DIR_DO)
		{
			/* 落下していないかどうか判定 */
			if (MainChar.Pos.Y < STG_FALL_Y)
			{
				Cou = Cou + 1;
				T = T_k * ((double)Cou / 60.0);
				// HUMAN PosY cal
				Mov.Y = (int)(pow(T, 2.0));
				for (int y = 1; y <= Mov.Y; y++)
				{
					if ((Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL] != PIC_BACK) ||				// 左下に何かある
						(Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL] != PIC_BACK))	// 右下に何かある
					{
						MainChar.Pos.Yin = MainChar.Pos.Y;	// このY位置を初期Y位置に設定しなおす（ジャンプ終了後の初期位置設定）
						Mov.JumpState = JUMP_OFF;
						Cou = 0;
						break;
					}
					else // 何もなければ移動できるのでY位置を更新
					{
						MainChar.Pos.Y = MainChar.Pos.Y + 1;
					}
				}
			}
			else // 落下
			{
				MainChar.Fall = TRUE;
			}
		}
	}
};