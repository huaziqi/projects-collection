#include "check.h"
#include"common.h"
#include "draw.h"
#include "generate.h"

extern textS movInfo;
extern textS planeInfo;
extern int nowPage;
extern int winWidth, winHeight;
extern allPlane planeGroup[8];
extern boardPoint boardPoints[30][20];
extern boardType typeGroup[4];
extern int xNum, yNum;

wchar_t pInfo[30], cInfo[30];

int lastHover[3] = { -1, -1 };
int lastClick[3] = { -1, -1 };
int nowClick[3] = { -1, -1 };

void checkMovInfo(int groupCode, wchar_t a[], wchar_t b[]) {
	if (groupCode == 0)
		wcscpy_s(movInfo.text[movInfo.textNum++], a);
	else if (groupCode == 1)
		wcscpy_s(movInfo.text[movInfo.textNum++], b);
}

int checkChoice(int randNum, int nums[]) {
	int i = 0;
	while (randNum > nums[i]) randNum -= nums[i++];
	return i;
}

bool checkMouse(buttonPos button, ExMessage msg)
{
	if (msg.x > button.leftP && msg.x < button.rightP && msg.y > button.topP && msg.y < button.bottomP)
		return true;
	else
		return false;
}

void checkAlive(planeS& bAPlane) {
	if (bAPlane.health < 0) {
		bAPlane.alive = false;
		boardPoints[bAPlane.x][bAPlane.y].planeOn = -1;
		nowPage = 1;
	}

}

void checkAttack(int aPos[], int bAPos[]) {
	planeS& aPlane = planeGroup[0].plane[boardPoints[aPos[0]][aPos[1]].planeOn % 10];
	planeS& bAPlane = planeGroup[1].plane[boardPoints[bAPos[0]][bAPos[1]].planeOn % 10];
	if (aPlane.gun[1] > 0) {
		wcscpy_s(pInfo, L"你使用用光子大炮"); wcscpy_s(cInfo, L"");
		checkMovInfo(0, pInfo, cInfo);
		aPlane.gun[1]--;
		if (bAPlane.shiled > 0) {
			bAPlane.health -= 20 - min(10, bAPlane.shiled);
			bAPlane.shiled = max(0, bAPlane.shiled - 10);
		}
		else {
			bAPlane.health -= 20;
		}
	}
	else if (aPlane.gun[0] > 0) {
		wcscpy_s(pInfo, L"你使用光子大炮"); wcscpy_s(cInfo, L"");
		checkMovInfo(0, pInfo, cInfo);
		aPlane.gun[0]--;
		bAPlane.health -= 15;
	}

	checkAlive(bAPlane);
}

bool checkPos(ExMessage msg, int* pos, int pointWidth) { //获取位置坐标
	const int boardLeft = 20, boardTop = 20, boardWidth = 1200, boardHeight = 600;
	buttonPos bP = { boardLeft, boardTop, boardLeft + boardWidth, boardTop + boardHeight };
	if (!checkMouse(bP, msg))
		return false;
	else {
		*(pos) = (msg.x - 20) / pointWidth + 1;
		*(pos + 1) = (msg.y - 20) / pointWidth + 1;
		return true;
	}
}

bool checkGameQuit(ExMessage msg) {
	buttonPos gameQuitButton = { winWidth - 30 - 100, winHeight - 30 - 50, winWidth - 30, winHeight - 30 };
	return checkMouse(gameQuitButton, msg);
}


bool checkMoveAv(int planeCode, int pos[]) { // 检查曼哈顿距离
	int x = planeGroup[0].plane[planeCode].x, y = planeGroup[0].plane[planeCode].y, dis = planeGroup[0].plane[planeCode].foot;
	int mdis = abs(x - pos[0]) + abs(y - pos[1]);
	return mdis <= dis;
}

int checkPlaneMov(int clickPos[], ExMessage msg) {
	if (lastHover[0] != -1)
		boardPoints[lastHover[0]][lastHover[1]].hover = false; // 取消之前的悬浮
	boardPoints[clickPos[0]][clickPos[1]].hover = true; // 确定当前的悬浮
	lastHover[0] = clickPos[0], lastHover[1] = clickPos[1]; //保存当前悬浮
	if (msg.message == WM_LBUTTONDOWN && boardPoints[clickPos[0]][clickPos[1]].planeOn != -1 && boardPoints[clickPos[0]][clickPos[1]].planeOn / 10 == 0) { // 选取当前格子
		generatePlaneInfo(boardPoints[clickPos[0]][clickPos[1]].planeOn % 10);
		boardPoints[clickPos[0]][clickPos[1]].clicked = true;
		nowClick[0] = clickPos[0], nowClick[1] = clickPos[1], nowClick[2] = boardPoints[clickPos[0]][clickPos[1]].planeOn % 10;;
		lastClick[0] = clickPos[0], lastClick[1] = clickPos[1], lastClick[2] = boardPoints[clickPos[0]][clickPos[1]].planeOn % 10;
	}
	if (msg.message == WM_RBUTTONDOWN && clickPos[0] == lastClick[0] && clickPos[1] == lastClick[1]) { //取消选取
		planeInfo.textNum = 0;
		boardPoints[clickPos[0]][clickPos[1]].clicked = false;
		lastClick[0] = lastClick[1] = -1;
	}
	if (msg.message == WM_LBUTTONDOWN && nowClick[0] != -1 && boardPoints[clickPos[0]][clickPos[1]].planeOn / 10 == 1) {

		checkAttack(nowClick, clickPos);
	}
	if (msg.message == WM_LBUTTONDOWN && nowClick[0] != -1 && boardPoints[clickPos[0]][clickPos[1]].planeOn == -1 && checkMoveAv(lastClick[2], clickPos)) { // �ƶ��ɴ�

		boardPoints[nowClick[0]][nowClick[1]].planeOn = -1;  // 更新位置
		boardPoints[nowClick[0]][nowClick[1]].clicked = false; //取消原有选取
		planeGroup[0].plane[nowClick[2]].x = clickPos[0], planeGroup[0].plane[nowClick[2]].y = clickPos[1];

		nowClick[0] = -1;  //���÷ɴ�ѡȡ
		return nowClick[2];
	}
	return -1;
}

void checkNewPos(int groupCode, int planeCode) {
	planeS& tPlane = planeGroup[groupCode].plane[planeCode];
	boardPoint nowPos = boardPoints[tPlane.x][tPlane.y];
	if (!nowPos.found) {
		boardPoints[tPlane.x][tPlane.y].found = true;
		int randNum = generateRondom(1, typeGroup[boardPoints[tPlane.x][tPlane.y].type].planetSum());

		int choiceI = checkChoice(randNum, typeGroup[nowPos.type].planetType);
		boardPoints[tPlane.x][tPlane.y].planetType = choiceI;

		if (choiceI == 0) {

			randNum = generateRondom(1, typeGroup[boardPoints[tPlane.x][tPlane.y].type].resourceSum());
			int choiceJ = checkChoice(randNum, typeGroup[nowPos.type].resourceType);
			boardPoints[tPlane.x][tPlane.y].resource = choiceJ;
			generatePoint(choiceJ, groupCode, planeCode);
		}
		else if (choiceI == 1) {


			boardPoints[tPlane.x][tPlane.y].planeOn = -1;

			int randX = generateRondom(1, xNum), randY = generateRondom(1, yNum);

			while (boardPoints[randX][randY].planeOn != -1 || randX == tPlane.x && randY == tPlane.y)
				randX = generateRondom(1, xNum), randY = generateRondom(1, yNum);
			tPlane.x = randX, tPlane.y = randY;

			checkNewPos(groupCode, planeCode);
		}
		else {
			wcscpy_s(pInfo, L"你没有发现"); wcscpy_s(cInfo, L"电脑没有发现");
			checkMovInfo(groupCode, pInfo, cInfo);
		}
	}
	else {
		if (nowPos.planetType == 1) {
			boardPoints[tPlane.x][tPlane.y].planeOn = -1;
			int randX = generateRondom(1, xNum), randY = generateRondom(1, yNum);
			while (boardPoints[randX][randY].planeOn != -1 && randX != tPlane.x && randY != tPlane.y)
				randX = generateRondom(1, xNum), randY = generateRondom(1, yNum);
			tPlane.x = randX, tPlane.y = randY;

			checkNewPos(groupCode, planeCode);
		}
		else if (nowPos.planetType == 0) {
			boardPoints[tPlane.x][tPlane.y].reFound += 10;
			typeGroup[boardPoints[tPlane.x][tPlane.y].type].resourceType[typeGroup[boardPoints[tPlane.x][tPlane.y].type].resourceNum - 1] += boardPoints[tPlane.x][tPlane.y].reFound;
			int randNum = generateRondom(1, typeGroup[boardPoints[tPlane.x][tPlane.y].type].resourceSum());
			typeGroup[boardPoints[tPlane.x][tPlane.y].type].resourceType[typeGroup[boardPoints[tPlane.x][tPlane.y].type].resourceNum - 1] -= boardPoints[tPlane.x][tPlane.y].reFound;
			int choiceJ = checkChoice(randNum, typeGroup[nowPos.type].resourceType);
			boardPoints[tPlane.x][tPlane.y].resource = choiceJ;
			generatePoint(choiceJ, groupCode, planeCode);
		}
	}
	planeInfo.textNum = 0;
}