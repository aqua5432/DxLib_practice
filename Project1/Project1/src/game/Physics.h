#pragma once
#include "Entity.h"
#include "Stage.h"

class Physics {
private:
    int Cou = 0;
    double T = 0;
    const double T_k = 20.0;

public:
    void ResetCou();
    void Update(MainCharacter& MainChar,
        MoveState& Mov,
        int& Sta_PosX,
        Stage& Sta);
};
