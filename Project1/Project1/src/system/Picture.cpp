#include "Picture.h"
#include "DxLib.h"

PictureClass Pic;

void PictureClass::Read()
{
    Back = LoadGraph("./picture/Back.png");
    Block1 = LoadGraph("./picture/Block1.png");
    Block2 = LoadGraph("./picture/Block2.png");
    Block3 = LoadGraph("./picture/Block3.png");
    Block4 = LoadGraph("./picture/Block4.png");
    Block5 = LoadGraph("./picture/Block5.png");
    MChara = LoadGraph("./picture/MainChara.png");
    Enemy1 = LoadGraph("./picture/Enemy_1.png");
    Title = LoadGraph("./picture/Title.png");
}
