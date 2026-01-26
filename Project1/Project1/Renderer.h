#pragma once
#include "Entity.h"
#include "Font.h"

class Renderer {
public:
    /*** ÉÅÉCÉìÉLÉÉÉâï`âÊ ***/
    void DrawPlayer(const MainCharacter& player)
    {
        if (player.PicDir == DIR_RI)
            DrawGraph(player.Pos.X, player.Pos.Y, Pic.MChara, TRUE);
        else
            DrawTurnGraph(player.Pos.X, player.Pos.Y, Pic.MChara, TRUE);
    }

    /*** ìGï`âÊ ***/
    void DrawEnemies(const std::vector<Enemy>& enemies, int Sta_PosX)
    {
        for (auto& e : enemies)
            DrawGraph(e.Pos.X + Sta_PosX, e.Pos.Y, Pic.Enemy1, TRUE);
    }

    /*** UIï`âÊ ***/
    void DrawUI(const Stage& sta)
    {
        for (int i = 0; i < RANK_DISP_NUM; i++)
        {
            DrawFormatStringFToHandle(
                RANK_POS_X,
                static_cast<float>(RANK_POS_Y + i * 10),
                Col.Black,
                Fon.FH[10],
                "No.%d:%6.2f s",
                i + 1,
                sta.Rank[i]
            );
        }

        DrawFormatStringFToHandle(
            TIME_POS_X, TIME_POS_Y,
            Col.Black, Fon.FH[10],
            "Time:%6.2f s",
            static_cast<float>((GetNowCount() - sta.StartCount) / MillSecond)
        );
    }

    /*** ÉXÉeÅ[ÉWÇÃï`âÊ ***/
    void DrawStage(const Stage& stage, int Sta_PosX)
    {
        stage.Out(&Sta_PosX);
    }
};
