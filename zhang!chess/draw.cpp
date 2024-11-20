#include "common.h"
#include "draw.h"
#include "output.h"
#undef UNICODE
#undef _UNICODE

extern int xNum, yNum;
extern textS movInfo;
extern textS planeInfo;
extern int type[10];
extern int winWidth, winHeight;
extern const int boardLeft = 20, boardTop = 20, boardWidth = 1200, boardHeight = 600, pointWidth = 100;
extern boardPoint boardPoints[30][20];
extern boardType typeGroup[4];

extern allPlane planeGroup[8];

int getType(int x, int y, int xNum, int yNum) {
	if (x >= 1 && x <= xNum / 2 && y >= 1 && y <= yNum / 2)
		return type[0];
	else if (x > xNum / 2 && x <= xNum && y >= 1 && y <= yNum / 2)
		return type[1];
	else if (x >= 1 && x <= xNum / 2 && y > yNum / 2 && y <= yNum)
		return type[2];
	else
		return type[3];
}

void setPlanePos() {
	for (int p = 0; p <= 1; p++)
		for (int i = 0; i <= 0; i++)
			boardPoints[planeGroup[p].plane[i].x][planeGroup[p].plane[i].y].planeOn = p * 10 + i;

}

void drawButton(COLORREF bkColor, COLORREF buttonColor, int leftD, int topD, int buttonWidth, int buttonHeight, bool borderButton, int borderWidth, bool hasText, wchar_t buttonText[]) {
	setfillcolor(buttonColor);
	solidrectangle(leftD, topD, leftD + buttonWidth, topD + buttonHeight);
	if (borderButton) {
		setfillcolor(bkColor);
		solidrectangle(leftD + borderWidth, topD + borderWidth, leftD + buttonWidth - borderWidth, topD + buttonHeight - borderWidth);
	}
	if (hasText) {
		LOGFONT buttonFont;
		gettextstyle(&buttonFont);
		const int textWidth = buttonFont.lfHeight;
		const int textHeight = textWidth;
		int strWidth = 0;
		for (int i = 0; i < wcslen(buttonText); i++) {
			strWidth += buttonText[i] >= 127 ? textWidth : textWidth / 2;
		}
		settextcolor(buttonColor);
		outtextxy(leftD + (buttonWidth - strWidth) / 2, topD + (buttonHeight - textHeight) / 2, buttonText);
	}
}

void drawBoard() {
	IMAGE img;
	wchar_t noText[] = L"";

	// 绘制棋盘边框
	drawButton(WHITE, BLACK, boardLeft - 3, boardTop - 3, boardWidth + 3, boardHeight + 3, true, 3, false, noText);
	drawButton(WHITE, BLACK, boardLeft, boardTop, pointWidth, pointWidth, true, 2, false, noText);

	//初始化飞船位置

	setPlanePos();
	//绘制移动信息
	drawButton(BLACK, WHITE, winWidth - 40 - 200, 20, 200, 150, true, 1, false, noText);
	if (movInfo.textNum > 0) {
		int textX = winWidth - 40 - 200, textY = 20;

		for (int i = 0; i < movInfo.textNum; i++) {
			settextcolor(WHITE);
			settextstyle(20, 0, L"黑体");
			outtextxy(textX + 20, textY + 3, movInfo.text[i]);
			outMovInform(movInfo.text[i], textX, textY, WHITE);
			textY += 22;
		}
	}
	//绘制信息
	drawButton(BLACK, WHITE, winWidth - 40 - 200, 20 + 200, 200, 200, true, 1, false, noText);
	if (planeInfo.textNum > 0) {
		int textX = winWidth - 40 - 200, textY = 20 + 200;

		for (int i = 0; i < planeInfo.textNum; i++) {
			settextcolor(WHITE);
			settextstyle(20, 0, L"黑体");
			outtextxy(textX + 20, textY + 3, planeInfo.text[i]);
			outMovInform(planeInfo.text[i], textX, textY, WHITE);
			textY += 22;
		}
	}
	// 绘制棋盘
	int nowX = boardLeft, nowY = boardTop;
	for (int i = 1; i <= xNum; i++) {
		for (int j = 1; j <= yNum; j++) {
			int pointType = getType(i, j, xNum, yNum);

			drawButton(typeGroup[boardPoints[i][j].type].color, BLACK, nowX, nowY, pointWidth, pointWidth, true, 1, false, noText);

			setfillcolor(boardPoints[i][j].found ? WHITE : BLACK);
			solidcircle(nowX + pointWidth / 2, nowY + pointWidth / 2, pointWidth / 2 - 2);
			setbkmode(TRANSPARENT);
			if (boardPoints[i][j].planetType == 0) {

				setfillcolor(typeGroup[boardPoints[i][j].type].color);
				solidcircle(nowX + pointWidth / 2, nowY + pointWidth / 2, pointWidth / 3.5);
			}
			if (boardPoints[i][j].planeOn != -1) {
				int group = boardPoints[i][j].planeOn / 10, planecode = boardPoints[i][j].planeOn % 10;


				setlinecolor(WHITE);
				setlinestyle(PS_SOLID, 2);
				drawButton(WHITE, BLACK, nowX + pointWidth / 6, nowY + pointWidth / 6, pointWidth * 4 / 6, pointWidth * 4 / 6, true, 2, false, noText);
				drawButton(BLUE, group == 0 ? RGB(238, 115, 101) : RGB(2, 94, 103), nowX + pointWidth / 6, nowY + pointWidth / 6, pointWidth * 4 / 6, pointWidth * 4 / 6 * (planeGroup[group].plane[planecode].health * 1.0 / 100), false, 2, false, noText);
			}
			if (boardPoints[i][j].planetType == 1) {
				int r = 20, X = nowX + pointWidth / 2, Y = nowY + pointWidth / 2;
				COLORREF color = BLACK;
				for (int i = 0; i < 5; i++) {
					color = color == BLACK ? WHITE : BLACK;
					setfillcolor(color);
					solidcircle(X, Y, r);
					r -= 3;
				}
			}

			if (boardPoints[i][j].hover) {
				POINT pts[] = { {nowX + 2, nowY + 2}, {nowX + pointWidth - 2, nowY + 2}, {nowX + pointWidth - 2, nowY + pointWidth - 2}, {nowX + 2, nowY + pointWidth - 2}, {nowX + 2, nowY + 2} };
				setlinestyle(PS_SOLID, 2);
				setlinecolor(WHITE);
				polyline(pts, 5);
			}
			if (boardPoints[i][j].clicked) {
				setlinestyle(PS_SOLID, 2);
				setlinecolor(RGB(255, 0, 255));
				POINT pts[] = { {nowX + 2, nowY + 2}, {nowX + pointWidth - 2, nowY + 2}, {nowX + pointWidth - 2, nowY + pointWidth - 2}, {nowX + 2, nowY + pointWidth - 2}, {nowX + 2, nowY + 2} };
				polyline(pts, 5);
			}

			nowY += pointWidth;

		}
		nowY = 20;
		nowX += pointWidth;
	}
	//绘制退出按钮
	wchar_t buttonText[] = L"退出";
	drawButton(WHITE, BLACK, winWidth - 30 - 100, winHeight - 30 - 50, 100, 50, true, 3, true, buttonText);

}

void drawCheckQuit() {

}