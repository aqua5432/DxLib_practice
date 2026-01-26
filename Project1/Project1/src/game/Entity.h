#pragma once
#include <vector>
#include "../config/ProjConf.h"
#include <wtypes.h>
struct MainCharacter
{
    struct
    {
        int X = CHA_POS_X_INI;
        int Y = CHA_POS_Y_INI;
        int Yin = Y;
    }Pos;

    struct
    {
        struct { int Ri, Up, Ce; } RiUp;
        struct { int Le, Up, Ce; } LeUp;
        struct { int Ri, Do, Ce; } RiDo;
        struct { int Le, Do, Ce; } LeDo;
    }Cor;

    int Dir = DIR_NONE;
    int PicDir = DIR_RI;
    int Touch = DIR_NONE;
    int Fall = FALSE;
};

struct Enemy
{
    struct
    {
        int X = 0;
        int Y = STG_X_MAX - 3 * CELL;
    }Pos;

    struct
    {
        struct { int Ri, Up, Ce; } RiUp;
        struct { int Le, Up, Ce; } LeUp;
        struct { int Ri, Do, Ce; } RiDo;
        struct { int Le, Do, Ce; } LeDo;
    }Cor;

    int Dir = DIR_NONE;
    int Touch = DIR_NONE;
    int SpeedX = 2;
};

struct MoveState
{
    int X = MOVEX;
    int Y = 0;
    int JumpState = JUMP_OFF;
    bool Dash = OFF;
};
