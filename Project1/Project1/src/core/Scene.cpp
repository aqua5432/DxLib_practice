#include "Scene.h"
#include "../config/ProjConf.h"
#include "../game/Action.h"
#include "../game/Title.h"

SceneClass Sce;

void SceneClass::Out() {
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