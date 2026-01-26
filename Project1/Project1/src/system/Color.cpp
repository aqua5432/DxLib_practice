#include "Color.h"
#include "DxLib.h"
#include "../config/ProjConf.h"

ColorClass Col;

void ColorClass::Read()
{
	White = GetColor(255, 255, 255);
	Red = GetColor(255, 0, 0);
	Green = GetColor(0, 255, 0);
	Blue = GetColor(0, 0, 255);
	Black = GetColor(0, 0, 0);
}