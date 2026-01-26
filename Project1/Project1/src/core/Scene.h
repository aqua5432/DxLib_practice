#pragma once
#include "../config/ProjConf.h"
#include "../game/Action.h"

/*** Sceneクラス ***/
class SceneClass
{
public:
	/* シーン出力 */
	void Out();
private:
	// 現在のシーン
	int now = SCE::SCE_00_TIT;
	Action Act;

};

extern SceneClass Sce;
