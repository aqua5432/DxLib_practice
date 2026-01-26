#include "Font.h"
#include "DxLib.h"

FontClass Fon;

void FontClass::Read() {
	for (int i = 0; i < 30 + 1; i++)
	{
		FH[i] = CreateFontToHandle("‚l‚r@ƒSƒVƒbƒN", i, 6, DX_FONTTYPE_NORMAL);
	}
}