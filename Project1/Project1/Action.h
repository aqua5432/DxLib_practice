#pragma once

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
		Cou = 0;

		Enem1.Pos.X = ENE1_POS_X_INI;
		Enem1.Pos.Y = ENE1_POS_Y_INI;
		Enem1.Dir = DIR_RI;
		Enem1.Touch = DIR_NONE;

		Enem2.Pos.X = ENE2_POS_X_INI;
		Enem2.Pos.Y = ENE2_POS_Y_INI;
		Enem2.Dir = DIR_RI;
		Enem2.Touch = DIR_NONE;

		Goal = FALSE;
		EndFlag = FALSE;
	}

	/*** 更新用関数 ***/
	void Update()
	{
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
		}

		/*** 終了判定 ***/
		if ((Key.input[KEY_INPUT_ESCAPE] > 0) ||	// Esc押下
			(MainChar.Fall == TRUE) ||				// 落下判定
			(Enem1.Touch != DIR_NONE) ||			// 敵1当たり判定
			(Enem2.Touch != DIR_NONE) ||			// 敵2当たり判定
			(Goal == TRUE)							// ゴール判定
#ifdef DEF_JOYPAD_VALID
			|| (JPad.input.Buttons[12] == 128)		// JoyPad「PS」押下
#endif /* DEF_JOYPAD_VALID */
			)
		{
			EndFlag = TRUE;
		}
	}

	/*** 移動計算 ***/
	void Cal()
	{
		/*** 敵1移動 ***/
		if (Enem1.Dir == DIR_RI)
		{
			//Enem1.Pos.X = Enem1.Pos.X + Mov.X; /* この敵はメインキャラのダッシュしたい気分に合わせて速度を変える */
			//Enem1.Pos.X = Enem1.Pos.X + Enem1.SpeedX; // 変更点
			/*if (Enem1.Pos.X >= WIN_MAX_X - CELL)
			{
				Enem1.Dir = DIR_LE;
			}*/
			float dist = abs(MainChar.Pos.X - Enem1.Pos.X);
			float speed = 1.5f + dist * 0.001f;
			speed = min(speed, 4.0f); // 上限

			Enem1.Pos.X += Enem1.SpeedX * speed;

		}
		else
		{
			//Enem1.Pos.X = Enem1.Pos.X - Mov.X; /* この敵はメインキャラのダッシュしたい気分に合わせて速度を変える */
			//Enem1.Pos.X = Enem1.Pos.X - Enem1.SpeedX; // 変更点
			/*if (Enem1.Pos.X <= WIN_MIN_X)
			{
				Enem1.Dir = DIR_RI;
			}*/
			float dist = abs(MainChar.Pos.X - Enem1.Pos.X);
			float speed = 1.5f + dist * 0.001f;
			speed = min(speed, 4.0f); // 上限

			Enem1.Pos.X -= Enem1.SpeedX * speed;
		}
		
		int camLeft = -Sta_PosX;
		int camRight = -Sta_PosX + WIN_MAX_X;

		if (Enem1.Pos.X <= camLeft)
			Enem1.Dir = DIR_RI;

		if (Enem1.Pos.X >= camRight - CELL)
			Enem1.Dir = DIR_LE;

		/*** 敵1接触判定右 ***/
		if ((MainChar.Pos.X - Sta_PosX + CELL >= Enem1.Pos.X + ENEMY_HIT_RANGE) &&
			(MainChar.Pos.X - Sta_PosX + CELL < Enem1.Pos.X + CELL) &&
			(MainChar.Pos.Y >= Enem1.Pos.Y) &&
			(MainChar.Pos.Y < Enem1.Pos.Y + CELL))
		{
			Enem1.Touch = DIR_RI;
		}
		/*** 敵1接触判定左 ***/
		else if ((MainChar.Pos.X - Sta_PosX >= Enem1.Pos.X) &&
			(MainChar.Pos.X - Sta_PosX < Enem1.Pos.X + CELL - ENEMY_HIT_RANGE) &&
			(MainChar.Pos.Y >= Enem1.Pos.Y) &&
			(MainChar.Pos.Y < Enem1.Pos.Y + CELL))
		{
			Enem1.Touch = DIR_LE;
		}

		/*** 敵2移動 ***/
		if (Enem2.Dir == DIR_RI)
		{
			//Enem2.Pos.X = Enem2.Pos.X + Mov.X; /* この敵はメインキャラのダッシュしたい気分に合わせて速度を変える */
			//Enem2.Pos.X = Enem2.Pos.X + Enem2.SpeedX; // 変更点
			/*if (Enem2.Pos.X >= WIN_MAX_X - CELL)
			{
				Enem2.Dir = DIR_LE;
			}*/
			float dist = abs(MainChar.Pos.X - Enem2.Pos.X);
			float speed = 1.5f + dist * 0.001f;
			speed = min(speed, 4.0f); // 上限

			Enem2.Pos.X += Enem2.SpeedX * speed;
		}
		else
		{
			//Enem2.Pos.X = Enem2.Pos.X - Mov.X; /* この敵はメインキャラのダッシュしたい気分に合わせて速度を変える */
			//Enem2.Pos.X = Enem2.Pos.X - Enem2.SpeedX; // 変更点
			/*if (Enem2.Pos.X <= WIN_MIN_X)
			{
				Enem2.Dir = DIR_RI;
			}*/
			float dist = abs(MainChar.Pos.X - Enem2.Pos.X);
			float speed = 1.5f + dist * 0.001f;
			speed = min(speed, 4.0f); // 上限

			Enem2.Pos.X -= Enem2.SpeedX * speed;
		}

		int camLeft2 = -Sta_PosX;
		int camRight2 = -Sta_PosX + WIN_MAX_X;

		if (Enem2.Pos.X <= camLeft2)
			Enem2.Dir = DIR_RI;

		if (Enem2.Pos.X >= camRight2 - CELL)
			Enem2.Dir = DIR_LE;

		if (abs(Enem1.Pos.X - Enem2.Pos.X) < CELL)
		{
			if (Enem1.Dir == DIR_RI) {
				Enem1.Dir = DIR_LE;
				Enem2.Dir = DIR_RI;
			}
			else {
				Enem1.Dir = DIR_RI;
				Enem2.Dir = DIR_LE;
			}
		}

		/*** 敵2接触判定右 ***/
		if ((MainChar.Pos.X - Sta_PosX + CELL >= Enem2.Pos.X + ENEMY_HIT_RANGE) &&
			(MainChar.Pos.X - Sta_PosX + CELL < Enem2.Pos.X + CELL) &&
			(MainChar.Pos.Y >= Enem2.Pos.Y) &&
			(MainChar.Pos.Y < Enem2.Pos.Y + CELL))
		{
			Enem2.Touch = DIR_RI;
		}
		/*** 敵2接触判定左 ***/
		else if ((MainChar.Pos.X - Sta_PosX >= Enem2.Pos.X) &&
			(MainChar.Pos.X - Sta_PosX < Enem2.Pos.X + CELL - ENEMY_HIT_RANGE) &&
			(MainChar.Pos.Y >= Enem2.Pos.Y) &&
			(MainChar.Pos.Y < Enem2.Pos.Y + CELL))
		{
			Enem2.Touch = DIR_LE;
		}

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

	/*** メインキャラ描画 ***/
	void Cha()
	{
		if (MainChar.PicDir == DIR_RI)		// 右向きの場合
		{
			DrawGraph(MainChar.Pos.X, MainChar.Pos.Y, Pic.MChara, TRUE);		// 右向きの描画
		}
		else if (MainChar.PicDir == DIR_LE)	// 左向きの場合
		{
			DrawTurnGraph(MainChar.Pos.X, MainChar.Pos.Y, Pic.MChara, TRUE);	// 左向きの描画
		}
	}

	/*** 敵描画 ***/
	void Ene()
	{
		//DrawGraph(Enem1.Pos.X, Enem1.Pos.Y, Pic.Enemy1, TRUE); /*** 敵1描画 ***/
		//DrawGraph(Enem2.Pos.X, Enem2.Pos.Y, Pic.Enemy1, TRUE); /*** 敵2描画 ***/

		DrawGraph(Enem1.Pos.X + Sta_PosX, Enem1.Pos.Y, Pic.Enemy1, TRUE);// 変更点
		DrawGraph(Enem2.Pos.X + Sta_PosX, Enem2.Pos.Y, Pic.Enemy1, TRUE);// 変更点
	}

	/*** 表示系描画 ***/
	void Disp()
	{
		/*** クリアタイムの描画 ***/
		for (int i = 0; i < RANK_DISP_NUM; i++)
		{
			DrawFormatStringFToHandle(RANK_POS_X, static_cast<float>(RANK_POS_Y + i * 10), Col.Black, Fon.FH[10], "No.%d:%6.2f s", i + 1, Sta.Rank[i]);
		}
		/*** 現在タイムの描画 ***/
		DrawFormatStringFToHandle(TIME_POS_X, TIME_POS_Y, Col.Black, Fon.FH[10], "Time:%6.2f s", static_cast<float>((GetNowCount() - Sta.StartCount) / MillSecond));

		/*** コマンド説明の描画 ***/
		DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y, Col.Black, Fon.FH[10], "右移動：[→]or[D]");
		DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 10, Col.Black, Fon.FH[10], "左移動：[←]or[A]");
		DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 20, Col.Black, Fon.FH[10], "ジャンプ：[↑]or[W]");
		DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 30, Col.Black, Fon.FH[10], "ダッシュ：[F]");
		DrawFormatStringFToHandle(COMD_POS_X, COMD_POS_Y + 40, Col.Black, Fon.FH[10], "Titleに戻る：[Esc]");
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
		Cou = 0;

		Enem1.Pos.X = ENE1_POS_X_INI;
		Enem1.Pos.Y = ENE1_POS_Y_INI;
		Enem1.Dir = DIR_RI;
		Enem1.Touch = DIR_NONE;

		Enem2.Pos.X = ENE2_POS_X_INI;
		Enem2.Pos.Y = ENE2_POS_Y_INI;
		Enem2.Dir = DIR_RI;
		Enem2.Touch = DIR_NONE;

		Goal = FALSE;
		EndFlag = FALSE;
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

		/*** ステージ描画 ***/
		Sta.Out(&Sta_PosX);

		/*** メインキャラ描画 ***/
		Cha();

		/*** 敵描画 ***/
		Ene();

		/*** 表示系描画 ***/
		Disp();

		/*** ENDフラグ有効時、タイトルシーンに移行 ***/
		if (EndFlag == TRUE)
		{

			if (Goal == TRUE)
			{
				ClearWait++;

				DrawString(300, 200, "GAME CLEAR", GetColor(0, 255, 0));

				if (ClearWait < 120) return ret;  // 2秒待つ
			}
			/*** クリアタイム更新 ***/
			if (Goal == TRUE)
			{
				Sta.UpdateTime();
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

	// メインキャラ構造体
	struct
	{
		struct
		{
			int X = CHA_POS_X_INI;
			int Y = CHA_POS_Y_INI;
			int Yin = Y;			// Y方向移動前初期値
		}Pos;
		struct
		{
			struct
			{
				int Ri = 0;
				int Up = 0;
				int Ce = 0;
			}RiUp;
			struct
			{
				int Le = 0;
				int Up = 0;
				int Ce = 0;
			}LeUp;
			struct
			{
				int Ri = 0;
				int Do = 0;
				int Ce = 0;
			}RiDo;
			struct
			{
				int Le = 0;
				int Do = 0;
				int Ce = 0;
			}LeDo;
		}Cor;

		int Dir = DIR_NONE;
		int PicDir = DIR_RI;
		int Touch = DIR_NONE;
		int Fall = FALSE;
	}MainChar;

	// 敵キャラ構造体
	struct ENEMY
	{
		struct
		{
			int X = 0;
			int Y = STG_X_MAX - 3 * CELL;
		}Pos;
		struct
		{
			struct
			{
				int Ri = 0;
				int Up = 0;
				int Ce = 0;
			}RiUp;
			struct
			{
				int Le = 0;
				int Up = 0;
				int Ce = 0;
			}LeUp;
			struct
			{
				int Ri = 0;
				int Do = 0;
				int Ce = 0;
			}RiDo;
			struct
			{
				int Le = 0;
				int Do = 0;
				int Ce = 0;
			}LeDo;
		}Cor;

		int Dir = DIR_NONE;
		int Touch = DIR_NONE;
		int SpeedX = 2;  //敵固有の速度(改良点)
	};

	// 移動用構造体
	struct
	{
		int X = MOVEX;				// X方向移動量
		int Y = 0;					// Y方向移動量
		int JumpState = JUMP_OFF;	// ジャンプ状態
		bool Dash = OFF;			// ダッシュ
	}Mov;

	ENEMY Enem1;
	ENEMY Enem2;
private:
	int Goal = FALSE;
	int EndFlag = FALSE;

	// 2次関数ジャンプ用変数
	int Cou = 0;
	double T = 0.0;
	const double T_k = 20.0;
	int ClearWait = 0;

}Act;
