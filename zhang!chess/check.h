#pragma once
#include "common.h"
#include "start.h"
#include "draw.h"

void checkMovInfo(int groupCode, wchar_t a[], wchar_t b[]);

int checkChoice(int randNum, int nums[]);

bool checkMouse(buttonPos button, ExMessage msg);

void checkAlive(planeS& bAPlane);

bool checkPos(ExMessage msg, int* pos, int pointWidth);

bool checkGameQuit(ExMessage msg);

bool checkMoveAv(int planeCode, int pos[]);

int checkPlaneMov(int clickPos[], ExMessage msg);

void checkNewPos(int groupCode, int planeCode);