#include "Key.h"
#include "DxLib.h"
#include "../config/ProjConf.h"

KeyClass Key;

int KeyClass::GetKey()
{
    char allkey[256];
    GetHitKeyStateAll(allkey);

    for (int i = 0; i < 256; i++)
    {
        if (allkey[i] == 1)
        {
            if (input[i] < DEF_KEY_PRESS_TIME)
            {
                input[i]++;
            }
        }
        else
        {
            input[i] = 0;
        }
    }
    return 0;
}
