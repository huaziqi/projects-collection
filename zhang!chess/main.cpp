#include "common.h"
#include "start.h"
#include "check.h"
#include "draw.h"
#include "generate.h"


char resourceName[20][20] = {
		"photonsGun", "nuetrunGun", "repairPart", "shieldPart", "pushPart", "none"
};
char planetName[20][20] = {
		"resources", "warmhole", "none"
};
const int maxHealth = 100;


int type[10] = { 0, 1, 2, 3 };
int nowPage = 1;
int winWidth = 1480, winHeight = 680;
const int boardLeft = 20, boardTop = 20, boardWidth = 1200, boardHeight = 600, pointWidth = 100;
int xNum = boardWidth / pointWidth, yNum = boardHeight / pointWidth;
boardPoint boardPoints[30][20];
boardType typeGroup[4];

allPlane planeGroup[8];

textS movInfo;
textS planeInfo;

HWND hWnd; // ����windows���
ExMessage msg;

void initTypeGroup(int opt, int num[]) {

	for (int i = 0; i < typeGroup[opt].planetNum; i++)
		typeGroup[opt].planetType[i] = num[i];
	for (int i = typeGroup[opt].planetNum; i < typeGroup[opt].planetNum + typeGroup[opt].resourceNum; i++)
		typeGroup[opt].resourceType[i - typeGroup[opt].planetNum] = num[i];

}

void initWindow() {
	int screenWidth = GetSystemMetrics(SM_CXSCREEN); //屏幕大小
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	initgraph(winWidth, winHeight);

	hWnd = GetHWnd(); //获取句柄

	SetWindowPos(hWnd, NULL, (screenWidth - winWidth) / 2, (screenHeight - winHeight) / 2 - 50, 0, 0, SWP_NOSIZE); // ��������λ�ã����У����ı���Ļ��С��

	BeginBatchDraw();
}

void initGame() {
	int num = 0;
	bool flag[5] = { false, false, false, false };
	while (num < 4) {
		int rdn = generateRondom(0, 3);
		if (!flag[rdn])
			flag[rdn] = true, type[num++] = rdn;
	}
	typeGroup[0].color = RED, typeGroup[1].color = GREEN, typeGroup[2].color = BLUE, typeGroup[3].color = RGB(241, 196, 15);
	int a[] = { 50, 5, 45, 4, 2, 1, 1, 3, 0 }, b[] = { 60, 3, 37,  3, 1, 3, 2, 3, 0 }, c[] = { 65, 3, 32, 2, 3 ,2, 1, 2, 0 }, d[] = { 55, 6, 39, 3, 3, 2, 100, 1, 0 };
	initTypeGroup(0, a); initTypeGroup(1, b); initTypeGroup(2, c); initTypeGroup(3, d);

	planeS comPlane[1] = { 1, 1 }, playerPlane[1] = { 12, 6 };
	planeGroup[0].plane[0] = { 12, 6, 100, 1, 0, true, 0, 0 };
	planeGroup[1].plane[0] = { 1, 1, 100, 1, 0, true, 0, 0 };

	for (int i = 1; i <= boardWidth / pointWidth; i++)
		for (int j = 1; j <= boardHeight / pointWidth; j++) {
			int nType = getType(i, j, boardWidth / pointWidth, boardHeight / pointWidth);

			boardPoints[i][j] = { nType, -1, -1, -1, 0, false, false, false };
		}
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= 0; j++) {
			boardPoints[planeGroup[i].plane[j].x][planeGroup[i].plane[j].y].found = true;

		}
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	initWindow();
	wchar_t gameTitle[] = L"胀！棋";

	while (nowPage != 99) {
		IMAGE img;


		if (nowPage == 1) {

			setbkcolor(WHITE);
			cleardevice();

			startButton startButtonGroup = initStartPage(gameTitle);
			if (peekmessage(&msg)) {
				if (checkMouse(startButtonGroup.button1, msg) && msg.message == WM_LBUTTONDOWN) {
					const int startButtonWidth = 200; // ��ť���
					const int startButtonHeight = 70; //��ť�߶�
					const int startButtonBorderWidth = 3;
					wchar_t buttonText0[] = L"开始";
					settextcolor(WHITE);

					setbkmode(TRANSPARENT);
					drawButton(BLACK, WHITE, startButtonGroup.button1.leftP, startButtonGroup.button1.topP, startButtonWidth, startButtonHeight, true, startButtonBorderWidth, true, buttonText0);
					FlushBatchDraw();
					initGame();
					Sleep(100);

					nowPage = 2;

				}
				else if (checkMouse(startButtonGroup.button2, msg) && msg.message == WM_LBUTTONDOWN) {


					nowPage = 99;

				}
			}
		}
		if (nowPage == 2) {
			setbkcolor(BLACK);
			cleardevice();
			drawBoard();

			if (peekmessage(&msg)) {
				int clickPos[3] = { -1, -1 };
				if (msg.message == WM_LBUTTONDOWN && checkGameQuit(msg)) // �˳�
					nowPage = 1;
				else if (checkPos(msg, clickPos, pointWidth)) { // �ҷ��ɴ��ƶ�
					int newPos = checkPlaneMov(clickPos, msg);
					if (newPos != -1)
						movInfo.textNum = 0, checkNewPos(0, newPos), generateComMov();

				}
			}
		}

		FlushBatchDraw();

	}
	EndBatchDraw();
}