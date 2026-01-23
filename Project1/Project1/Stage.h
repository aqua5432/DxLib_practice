#pragma once

/*** Stageクラス ***/
class
{
public:
	void Read()
	{
		FILE* fp_stage_1_1;			// ステージ位置情報のファイル
		FILE* fp_stage_1_1_rslt;	// ステージクリアタイムのファイル

		/*** ファイルオープン ***/
		fp_stage_1_1 = fopen("./stage/Stage_1_1.txt", "r");
		if (fp_stage_1_1 == NULL)
		{
			printf("ファイルオープンエラー");
			exit(EXIT_FAILURE);
		}

		fp_stage_1_1_rslt = fopen("./stage/Stage_1_1_result.txt", "r");
		if (fp_stage_1_1_rslt == NULL)
		{
			printf("ファイルオープンエラー");
			exit(EXIT_FAILURE);
		}

		/*** ステージ座標読み込み ***/
		for (int y = 0; y < STG_BLOCK_Y_MAX; y++)
		{
			for (int x = 0; x < STG_BLOCK_X_MAX; x++)
			{
				(void)fscanf(fp_stage_1_1, "%d", &Cood.Blo[x][y]);
				for (int y_exp = 0; y_exp < CELL; y_exp++)
				{
					for (int x_exp = 0; x_exp < CELL; x_exp++)
					{
						Cood.Pix[x_exp + x * CELL][y_exp + y * CELL] = Cood.Blo[x][y];
					}
				}
			}
		}

		/***クリアタイム読み込み ***/
		for (int x = 0; x < RANK_MAX; x++)
		{
			(void)fscanf(fp_stage_1_1_rslt, "%f", &Rank[x]);
		}

		/*** クローズ ***/
		fclose(fp_stage_1_1);
		fclose(fp_stage_1_1_rslt);
	}

	void UpdateTime()
	{
		float tmp_rank = static_cast<float>((GetNowCount() - StartCount) / MillSecond);

		/* クリアタイムをソート */
		Sta.Rank[RANK_4TH] = tmp_rank;
		std::sort(Sta.Rank, Sta.Rank + RANK_MAX);

		/* クリアタイムがランクインしていたら、クリアタイム更新 */
		if (Sta.Rank[RANK_4TH] != tmp_rank)
		{
			FILE* fp_stage_1_1_rslt;	// ステージクリアタイムのファイル

			/*** ファイルオープン ***/
			fp_stage_1_1_rslt = fopen("./stage/Stage_1_1_result.txt", "w");
			if (fp_stage_1_1_rslt == NULL)
			{
				printf("ファイルオープンエラー");
				exit(EXIT_FAILURE);
			}

			/***クリアタイム書き込み ***/
			for (int x = 0; x < RANK_MAX; x++)
			{
				fprintf(fp_stage_1_1_rslt, "%f\n", Sta.Rank[x]);
			}

			/*** クローズ ***/
			fclose(fp_stage_1_1_rslt);
		}
	}

	void Out(int* PosX)
	{
		int pic = 0;
		for (int x = 0; x < STG_BLOCK_X_MAX; x++)
		{
			for (int y = 0; y < STG_BLOCK_Y_MAX; y++)
			{
				switch (Cood.Blo[x][y])
				{
				case 0:
					pic = Pic.Back;
					break;
				case 1:
					pic = Pic.Block1;
					break;
				case 2:
					pic = Pic.Block2;
					break;
				case 3:
					pic = Pic.Block3;
					break;
				case 4:
					pic = Pic.Block4;
					break;
				case 5:
					pic = Pic.Block5;
					break;
				default:
					break;
				}
				DrawGraph(CELL * x + *PosX, CELL * y, pic, TRUE);
			}
		}
	}

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

private:

}Sta;
