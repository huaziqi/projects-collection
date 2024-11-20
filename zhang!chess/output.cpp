#include "output.h"

void outMovInform(wchar_t text[], int posX, int posY, COLORREF textColor) {
	settextcolor(textColor);
	//setbkmode(TRANSPARENT);
	outtextxy(posX, posY, textColor);
}
