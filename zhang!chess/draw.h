#pragma once
#include "common.h"

int getType(int x, int y, int xNum, int yNum);

void drawButton(COLORREF bkColor, COLORREF buttonColor, int leftD, int topD, int rightD, int bottomD, bool borderButton, int borderWidth, bool hasText, wchar_t buttonText[]);

void drawBoard();

void drawCheckQuit();

struct buttonPos {
	int leftP, topP, rightP, bottomP;
};

struct startButton {
	buttonPos button1, button2;
};

struct boardPoint {
	int type, resource = -1, planeOn = -1;
	int planetType = -1; // 星球类型
	int reFound = 0;
	bool clicked = false, hover = false, found = false;
};

struct boardType {
	COLORREF color;
	int planetType[20], planetNum = 3;
	int resourceType[20], resourceNum = 6;

	int planetSum() {
		int sum = 0;
		for (int i = 0; i < planetNum; i++)
			sum += planetType[i];
		return sum;
	}
	int resourceSum() {
		int sum = 0;

		for (int i = 0; i < resourceNum; i++)
			sum += resourceType[i];
		return sum;
	}
}; //棋盘类型

struct planeS {
	int x, y;
	int health = 100, foot = 1, shiled = 0;
	bool alive = true;
	int gun[10] = { 0, 0 };
}; //飞船信息

struct allPlane {
	planeS plane[8];
}; //飞船组

struct textS {
	int textNum = 0;
	wchar_t text[10][100];
};