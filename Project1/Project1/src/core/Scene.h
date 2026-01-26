#pragma once

/*** Sceneクラス ***/
class
{
public:
	/* シーン出力 */
	void Out()
	{
		switch (now)
		{
		case SCE::SCE_00_TIT:
			/* タイトルシーン出力 */
			now = Tit.Out();
			break;
		case SCE::SCE_01_ACT:
			/* アクションシーン出力 */
			now = Act.Out();
			break;
		default:
			break;
		}
	}

private:
	// 現在のシーン
	int now = SCE::SCE_00_TIT;

}Sce;
