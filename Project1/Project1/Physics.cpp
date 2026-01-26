#include "Physics.h"
#include "ProjConf.h"
#include "Stage.h"
#include <cmath>

void Physics::ResetCou()
{
    Cou = 0;
}

void Physics::Update(MainCharacter& MainChar,
    MoveState& Mov,
    int& Sta_PosX,
    Stage& Sta)
{
    /*** X•ûŒüˆÚ“®ŒvŽZ ***/
    if (MainChar.Dir == DIR_RI)
    {
        if ((MainChar.Touch & DIR_RI) != DIR_RI)
        {
            if (MainChar.Pos.X < WIN_MAX_X / 2)
            {
                MainChar.Pos.X += Mov.X;
            }
            else if ((MainChar.Pos.X >= WIN_MAX_X / 2) &&
                (abs(Sta_PosX) < STG_X_MAX - WIN_MAX_X))
            {
                Sta_PosX -= Mov.X;
            }
        }
    }
    else if (MainChar.Dir == DIR_LE)
    {
        if ((MainChar.Touch & DIR_LE) != DIR_LE)
        {
            if (MainChar.Pos.X > STG_X_MIN)
            {
                MainChar.Pos.X -= Mov.X;
            }
            else if ((MainChar.Pos.X <= STG_X_MIN) &&
                (Sta_PosX < STG_X_MIN))
            {
                Sta_PosX += Mov.X;
            }
        }
    }

    /*** Y•ûŒüˆÚ“®ŒvŽZ ***/
    if (Mov.JumpState == JUMP_UP)
    {
        Cou++;
        T = T_k * ((double)Cou / 60.0);
        Mov.Y = (int)(pow(T, 2.0));

        for (int y = 0; y < Mov.Y; y++)
        {
            if ((MOVEY_H_MAX == MainChar.Pos.Yin - MainChar.Pos.Y) ||
                (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y - 1] != PIC_BACK) ||
                (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y - 1] != PIC_BACK))
            {
                Mov.JumpState = JUMP_GRAVITY;
                Cou = 0;
                break;
            }
            MainChar.Pos.Y--;
        }
    }
    else if ((Mov.JumpState == JUMP_GRAVITY) ||
        (MainChar.Touch & DIR_DO) != DIR_DO)
    {
        if (MainChar.Pos.Y < STG_FALL_Y)
        {
            Cou++;
            T = T_k * ((double)Cou / 60.0);
            Mov.Y = (int)(pow(T, 2.0));

            for (int y = 0; y < Mov.Y; y++)
            {
                if ((Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX][MainChar.Pos.Y + CELL] != PIC_BACK) ||
                    (Sta.Cood.Pix[MainChar.Pos.X - Sta_PosX + CELL - 1][MainChar.Pos.Y + CELL] != PIC_BACK))
                {
                    MainChar.Pos.Yin = MainChar.Pos.Y;
                    Mov.JumpState = JUMP_OFF;
                    Cou = 0;
                    break;
                }
                MainChar.Pos.Y++;
            }
        }
        else
        {
            MainChar.Fall = TRUE;
        }
    }
}
