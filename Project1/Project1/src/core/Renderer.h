#pragma once
#include "../game/Entity.h"
#include "../system/Font.h"
#include "../system/Color.h"
#include "../game/Stage.h"

class Renderer {
public:
    /*** ƒƒCƒ“ƒLƒƒƒ‰•`‰æ ***/
    void DrawPlayer(const MainCharacter& player);

    /*** “G•`‰æ ***/
    void DrawEnemies(const std::vector<Enemy>& enemies, int Sta_PosX);

    /*** UI•`‰æ ***/
    void DrawUI(const Stage& sta);

    /*** ƒXƒe[ƒW‚Ì•`‰æ ***/
    void DrawStage(const Stage& stage, int& Sta_PosX);
};
