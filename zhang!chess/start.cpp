#include "start.h"
#include "common.h"
#include "draw.h"
#include "check.h"

extern int winWidth, winHeight;

const int titlePosWidth = winWidth / 2 - 65;
const int titlePosHeight = winHeight / 5;
const int startButtonsSpace = 40; // ��ť���
const int startButtonWidth = 200; // ��ť���
const int startButtonHeight = 70; //��ť�߶�
const int startButtonBorderWidth = 3;

startButton initStartPage(wchar_t gameTitle[])
{

	settextcolor(BLACK);
	settextstyle(40, 0, L"黑体");
	outtextxy(titlePosWidth, titlePosHeight, gameTitle);

	buttonPos beginButton = {
		winWidth / 2 - startButtonWidth / 2, 70 + titlePosHeight, winWidth / 2 + startButtonWidth / 2, 70 + titlePosHeight + startButtonHeight
	};
	buttonPos quitButton = {
		winWidth / 2 - startButtonWidth / 2, 70 + titlePosHeight + startButtonsSpace + startButtonHeight, winWidth / 2 + startButtonWidth / 2, 70 + titlePosHeight + startButtonsSpace + startButtonHeight + startButtonHeight
	};
	startButton buttonGroup = { beginButton , quitButton };

	wchar_t buttonText0[] = L"开始";
	drawButton(WHITE, BLACK, beginButton.leftP, beginButton.topP, startButtonWidth, startButtonHeight, true, startButtonBorderWidth, true, buttonText0);

	wchar_t buttonText1[] = L"退出";
	drawButton(WHITE, BLACK, quitButton.leftP, quitButton.topP, startButtonWidth, startButtonHeight, true, startButtonBorderWidth, true, buttonText1);

	return buttonGroup;

}
