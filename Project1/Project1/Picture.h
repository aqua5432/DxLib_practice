#pragma once

/*** PictureƒNƒ‰ƒX ***/
class
{
public:
	int Back;
	int Block1;
	int Block2;
	int Block3;
	int Block4;
	int Block5;
	int MChara;
	int Enemy1;
	int Title;

	void Read()
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

private:

}Pic;
#pragma once
