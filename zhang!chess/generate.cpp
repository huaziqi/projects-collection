#include "generate.h"
#include "draw.h"
#include "common.h"
#include "check.h"
#include<wchar.h>
#include<random>

extern boardPoint boardPoints[30][20];
extern int xNum, yNum;
extern textS movInfo;
extern textS planeInfo;
extern allPlane planeGroup[8];
extern char resourceName[20][20];

extern wchar_t pInfo[30], cInfo[30];

int generateRondom(int low, int high)
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(low, high);
	return dist(rd);
}

void generatePoint(int opt, int partial, int planeCode) {
	planeS& nPlane = planeGroup[partial].plane[planeCode];

	if (opt == 0) {
		nPlane.gun[0]++;

		wcscpy_s(pInfo, L"你：光子大炮"); wcscpy_s(cInfo, L"电脑：光子大炮");
		checkMovInfo(partial, pInfo, cInfo);
	}

	else if (opt == 1) {
		nPlane.gun[1]++;

		wcscpy_s(pInfo, L"你：中子大炮"); wcscpy_s(cInfo, L"电脑：中子大炮");
		checkMovInfo(partial, pInfo, cInfo);
	}

	else if (opt == 2) {

		wcscpy_s(pInfo, L"你：修复套件"); wcscpy_s(cInfo, L"电脑：修复套件");
		checkMovInfo(partial, pInfo, cInfo);
		nPlane.health = min(100, nPlane.health + 10);
	}

	else if (opt == 3) {

		wcscpy_s(pInfo, L"你：护盾"); wcscpy_s(cInfo, L"电脑：护盾");
		checkMovInfo(partial, pInfo, cInfo);
		nPlane.shiled += 10;
	}

	else if (opt == 4) {

		wcscpy_s(pInfo, L"你：推进模块"); wcscpy_s(cInfo, L"电脑：推进模块");
		checkMovInfo(partial, pInfo, cInfo);
		nPlane.foot += 1;
	}
	else {
		wcscpy_s(pInfo, L"你没找到东西"); wcscpy_s(cInfo, L"电脑没找到东西");
		checkMovInfo(partial, pInfo, cInfo);
	}
}

void generatePlaneInfo(int planeCode) {
	planeS nPlane = planeGroup[0].plane[planeCode];
	planeInfo.textNum = 5;

	std::wstring sInfo = std::to_wstring(nPlane.health);
	sInfo = L"飞船血量:" + sInfo;
	const wchar_t* wInfo = sInfo.c_str();
	wcscpy_s(planeInfo.text[0], wInfo);
	sInfo = std::to_wstring(nPlane.gun[0]);
	sInfo = L"光子大炮数:" + sInfo;
	wInfo = sInfo.c_str();
	wcscpy_s(planeInfo.text[1], wInfo);
	sInfo = std::to_wstring(nPlane.gun[1]);
	sInfo = L"中子大炮数:" + sInfo;
	wInfo = sInfo.c_str();
	wcscpy_s(planeInfo.text[2], wInfo);
	sInfo = std::to_wstring(nPlane.shiled);
	sInfo = L"护盾:" + sInfo;
	wInfo = sInfo.c_str();
	wcscpy_s(planeInfo.text[3], wInfo);
	sInfo = std::to_wstring(nPlane.foot);
	sInfo = L"飞船步长:" + sInfo;
	wInfo = sInfo.c_str();
	wcscpy_s(planeInfo.text[4], wInfo);

}

void generateComMov() {
	planeS& aPlane = planeGroup[1].plane[0];
	planeS& bAPlane = planeGroup[0].plane[0];
	if (aPlane.gun[0] || aPlane.gun[1]) {
		if (aPlane.gun[1] > 0) {
			aPlane.gun[1]--;
			if (bAPlane.shiled > 0) {
				bAPlane.health -= 20 - min(10, bAPlane.shiled);
				bAPlane.shiled = max(0, bAPlane.shiled - 10);
			}
			else {
				bAPlane.health -= 20;
			}
			movInfo.textNum += 1;
			wcscpy_s(movInfo.text[0], L"电脑使用中子大炮");
		}
		else if (aPlane.gun[0] > 0) {
			aPlane.gun[0]--;
			movInfo.textNum += 1;
			wcscpy_s(movInfo.text[0], L"电脑使用光子大炮");
			aPlane.gun[0]--;
			bAPlane.health -= 15;
		}
		checkAlive(bAPlane);
	}
	int foot = aPlane.foot, nX = aPlane.x, nY = aPlane.y;
	int tX, tY;
	int x = 0, y = 0, rFoot = 0;
	do {

		x = generateRondom(-foot, foot);
		rFoot = foot - abs(x);
		y = generateRondom(-rFoot, rFoot);
		tX = x + nX, tY = y + nY;
	} while (tX <= 0 || tY <= 0 || tX > xNum || tY > yNum || boardPoints[tX][tY].planeOn != -1 || (tX == 0 && tY == 0));
	boardPoints[nX][nY].planeOn = -1;
	boardPoints[tX][tY].planeOn = 10;
	aPlane.x = tX, aPlane.y = tY;
	checkNewPos(1, 0);

}