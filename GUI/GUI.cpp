#include "stdafx.h"
#include <cstdio>
#include "GUI.h"
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#pragma warning(disable:4996)
void s(LPWSTR m);
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                RegisterSet(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
void Read();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
POINT Point, points;//当前鼠标坐标结构体
HWND MainWnd, Input, FileList, Guide, Set, PassWd; //主窗口的hwnd & 输入框hwnd & 文件列表框hwnd
BOOL Admin; //是否管理员
RECT rc;//zxf按钮四边形
HINSTANCE hInst;
wchar_t Readtmp[201], Readtmp2[2001][201];
int curdownload;
unsigned char u[2] = { 0xff,0xfe };  // Unicode 档头 = 0xfeff
int curcover, curcovers;//当前鼠标悬浮按钮
int cur, line;
bool NotFirst = false;
wchar_t FilePath[255];
int press, presss, pressc;//是否按下按钮
int Msv;//鼠标移出检测变量
HFONT st, stbig, stlarge;//宋体 & 更大的宋体
bool isoffline = false;
wchar_t TmpString[255], TmpString2[255];//专用缓冲区
wchar_t lasts = 'a';//Input中之前的字母
HBRUSH brush = CreateSolidBrush(RGB(210, 255, 255));//专用蓝色笔刷
HBRUSH BRush = CreateSolidBrush(RGB(225, 255, 255));//专用蓝色笔刷
HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));//专用白色笔刷
HPEN White = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));//白
HBRUSH green = CreateSolidBrush(RGB(0xBF, 0x3E, 0xFF));//专用绿色笔刷
HBRUSH Green = CreateSolidBrush(RGB(0x00, 0x8B, 0x45));//专用绿色笔刷
HPEN BLACK = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));//黑
HPEN GREEN = CreatePen(PS_SOLID, 2, RGB(0xBF, 0x3E, 0xFF));//绿
HBRUSH grey = CreateSolidBrush(RGB(210, 210, 210));
HPEN Grey = CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
HPEN CLOSE3 = CreatePen(PS_SOLID, 1, RGB(238, 99, 99));//×按钮颜色 一共三种 六个变量
HBRUSH close3 = CreateSolidBrush(RGB(238, 99, 99));
HPEN CLOSE1 = CreatePen(PS_SOLID, 1, RGB(255, 106, 106));
HBRUSH close1 = CreateSolidBrush(RGB(255, 106, 106));
HPEN CLOSE2 = CreatePen(PS_SOLID, 1, RGB(250, 102, 102));
HBRUSH close2 = CreateSolidBrush(RGB(250, 102, 102));

const wchar_t letter[27] = L"qwertyuiopasdfghjklzxcvbnm";
wchar_t PassWdBack[51];
int flashtype;
int dfmod, wordl;
HDC mdc;
HBITMAP hBmp;
int pow(int a, int b) {
	int ans = 1, base = a;
	while (b != 0) {
		if ((b & 1) != 0)
			ans *= base;
		base *= base;
		b >>= 1;
	}
	return ans;
}
struct Button//自绘按钮结构体
{
	int left, top, width, height, state = -1;
	POINT point;
	LPCWSTR name;
}zxf[40], set[10];

struct Unit
{
	int left, top;
	bool enabled = true;
	LPCWSTR name;
};
struct Choose
{
	int curchoosen, maxchoose = 1;
	Unit ch[10];
}choose[10];
int curzxf, curset;//总共按钮数
int curcoverc1 = -1, curcoverc2 = -1;
int word; bool gameover = false;
wchar_t curword[50] = L"computer", curcn[121] = L"[kəmˈpju:tə(r)] n. 计算机，电脑 ", Chinese[30001][121], Word[30001][50], unicode[2000001];
bool Right[50] = { false }, worduse[26] = { false };
wchar_t PossibleChoose[10001][30], curpossible;
int killstate = 0;

void CheateChoose(int left, int top, LPCWSTR name, int cur)
{
	choose[cur].ch[choose[cur].maxchoose].left = left;
	choose[cur].ch[choose[cur].maxchoose].top = top;
	choose[cur].ch[choose[cur].maxchoose].name = name;
	choose[cur].maxchoose++;
}
VOID CreateButton(int left, int top, int width, int height, LPCWSTR name)
{
	++curzxf;
	zxf[curzxf].left = left;
	zxf[curzxf].height = height;
	zxf[curzxf].top = top;
	zxf[curzxf].width = width;
	zxf[curzxf].name = name;
}

VOID CreateButtonS(int left, int top, int width, int height, LPCWSTR name)
{
	++curset;
	set[curset].left = left;
	set[curset].height = height;
	set[curset].top = top;
	set[curset].width = width;
	set[curset].name = name;
}


BOOL Inside(int cur)
{
	return (zxf[cur].left <= Point.x && zxf[cur].top <= Point.y && (zxf[cur].left + zxf[cur].width) >= Point.x && (zxf[cur].top + zxf[cur].height) >= Point.y);
}
BOOL Insides(int cur)
{
	return (set[cur].left <= points.x && set[cur].top <= points.y && (set[cur].left + set[cur].width) >= points.x && (set[cur].top + set[cur].height) >= points.y);
}

bool Insidec(int cur1, int cur2)
{
	return (choose[cur1].ch[cur2].left <= points.x && choose[cur1].ch[cur2].top <= points.y && (choose[cur1].ch[cur2].top + 15) >= points.y && (choose[cur1].ch[cur2].left + 200) >= points.x);
}

void MakeChinese()
{
	for (int i = 0; i <= line; ++i)
	{
		if (wcscmp(curword, Word[i]) == 0) { wcscpy_s(curcn, Chinese[i]); break; }
	}
}
LRESULT CALLBACK GuiderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;//初始化GDI绘图
	HDC hdc;
	RECT Rc;
	HFONT st1, st2;
	GetWindowRect(MainWnd, &Rc);
	SetWindowPos(Guide, 0, Rc.left, Rc.top, Rc.right, Rc.bottom, SWP_NOSIZE);
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		st1 = CreateFontW(22, 11, 150, 0, FW_THIN, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 2, DEFAULT_PITCH | FF_SWISS, _T("新宋体"));
		st2 = CreateFontW(22, 11, 50, 0, FW_THIN, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 2, DEFAULT_PITCH | FF_SWISS, _T("新宋体"));
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, st1);
		TextOut(hdc, 420, 210, L"可以选择右侧词库->", wcslen(L"可以选择右侧词库->"));
		TextOut(hdc, 120, 180, L"输入英文字母猜单词->", wcslen(L"输入英文字母猜单词->"));
		SelectObject(hdc, st2);
		TextOut(hdc, 470, 280, L"<-确保小人安全", wcslen(L"<-确保小人安全"));
		int kleft, kbottom; kleft = 265, kbottom = 400;
		SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 0)));
		SelectObject(hdc, grey);
		if (flashtype > 0)
		{
			MoveToEx(hdc, kleft, kbottom, NULL);
			LineTo(hdc, kleft + 150, kbottom);
		}
		if (flashtype > 1)
		{
			MoveToEx(hdc, kleft + 50, kbottom, NULL);
			LineTo(hdc, kleft + 50, kbottom - 200);
		}
		if (flashtype > 2)
		{
			MoveToEx(hdc, kleft + 30, kbottom - 200, NULL);
			LineTo(hdc, kleft + 140, kbottom - 200);
		}
		if (flashtype > 3)
		{
			MoveToEx(hdc, kleft + 124, kbottom - 200, NULL);
			LineTo(hdc, kleft + 124, kbottom - 170);
		}
		if (flashtype > 4)RoundRect(hdc, kleft + 104, kbottom - 170, kleft + 144, kbottom - 130, 50, 50);
		if (flashtype > 5)
		{
			MoveToEx(hdc, kleft + 124, kbottom - 130, NULL);
			LineTo(hdc, kleft + 124, kbottom - 105);
		}
		if (flashtype > 6)
		{
			MoveToEx(hdc, kleft + 124, kbottom - 105, NULL);
			LineTo(hdc, kleft + 90, kbottom - 114);
		}
		if (flashtype > 7)
		{
			MoveToEx(hdc, kleft + 124, kbottom - 105, NULL);
			LineTo(hdc, kleft + 158, kbottom - 114);
		}
		if (flashtype > 8)
		{
			MoveToEx(hdc, kleft + 124, kbottom - 105, NULL);
			LineTo(hdc, kleft + 103, kbottom - 64);
		}
		if (flashtype > 9)
		{
			MoveToEx(hdc, kleft + 124, kbottom - 105, NULL);
			LineTo(hdc, kleft + 145, kbottom - 64);
		}
		if (flashtype > 10)
		{
			MoveToEx(hdc, kleft + 112, kbottom - 160, NULL);
			LineTo(hdc, kleft + 119, kbottom - 153);
			MoveToEx(hdc, kleft + 119, kbottom - 160, NULL);
			LineTo(hdc, kleft + 112, kbottom - 153);
		}
		if (flashtype > 11)
		{
			MoveToEx(hdc, kleft + 128, kbottom - 160, NULL);
			LineTo(hdc, kleft + 135, kbottom - 153);
			MoveToEx(hdc, kleft + 135, kbottom - 160, NULL);
			LineTo(hdc, kleft + 128, kbottom - 153);
			TextOut(hdc, 500, 400, L"点击鼠标左键继续...", wcslen(L"点击鼠标左键继续..."));
		}
		break;
	case WM_LBUTTONDOWN:
		SendMessageW(Guide, WM_CLOSE, 0, 0);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}
ATOM RegisterGuide(HINSTANCE hInstance)
{
	WNDCLASSEXW wx = { 0 };
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.style = CS_HREDRAW | CS_VREDRAW;
	wx.lpfnWndProc = GuiderProc;
	wx.hInstance = hInstance;
	wx.hCursor = LoadCursor(0, IDC_ARROW);
	wx.hbrBackground = grey;
	wx.lpszMenuName = MAKEINTRESOURCE(IDI_GUI);
	wx.lpszClassName = L"guider";//类名
	return RegisterClassExW(&wx);//正式注册
}

VOID GetRECT(int cur)//更新rc
{
	rc.left = zxf[cur].left + 1;
	rc.right = zxf[cur].left + zxf[cur].width - 1;
	rc.top = zxf[cur].top + 1;
	rc.bottom = zxf[cur].top + zxf[cur].height - 1;
}

VOID GetRECTs(int cur)//更新rc
{
	rc.left = set[cur].left + 1;
	rc.right = set[cur].left + set[cur].width - 1;
	rc.top = set[cur].top + 1;
	rc.bottom = set[cur].top + set[cur].height - 1;
}


unsigned int x, y, z, w;

unsigned int rnd() {
	unsigned int t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

wchar_t pron[1001], tmp[10000], sen[10001];
wchar_t* TMP;
DWORD WINAPI ShowMeanings(void* a)
{
	(a);
	wchar_t* Des = new wchar_t[1000001];
	ZeroMemory(pron, sizeof(pron));
	ZeroMemory(tmp, sizeof(tmp));
	ZeroMemory(sen, sizeof(sen));

	wcscpy_s(tmp, L"https://dict.youdao.com/w/eng/");
	wcscat_s(tmp, curword);
	wcscat_s(tmp, L"/#keyfrom=dict2.index");

	wcscpy_s(sen, FilePath);
	wcscat_s(sen, L"index.html");
	URLDownloadToFileW(NULL, tmp, sen, 0, 0);

	FILE* p_source = NULL;
	wchar_t buffer[10001];

	if (p_source = _wfopen(sen, L"r,ccs=utf-8"))
	{
		while (!feof(p_source))
		{
			fgetws(buffer, 10000, p_source);
			wcscat(Des, buffer); //MessageBox(0, Des, 0, 0);
			//MessageBox(0, buffer, 0, 0);
		}
	}
	fcloseall();

	ZeroMemory(tmp, sizeof(tmp));

	wcscpy_s(tmp, curword);
	wcscat_s(tmp, L"\n\n音标：\n");
	while (1)
	{
		ZeroMemory(pron, sizeof(pron));
		//MessageBox(0, Des, 0, 0);
		TMP = wcsstr(Des, L"<span class=\"pronounce\">");
		if (TMP == 0)break;
		int curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 24;
		int curp = 0, curP = 0;
		// _itow_s(curpos, tmp, 10);
		//MessageBox(0, tmp, 0, 0);
		Des[curpos - 2] = ' ';
		while (Des[curpos + curp] != ' ' && Des[curpos + curp] != '\n' && Des[curpos + curp] != '	')curp++;
		memcpy(pron, &Des[curpos], sizeof(wchar_t) * curp);


		pron[curp] = ' ';
		curp++;

		TMP = wcsstr(Des, L"<span class=\"phonetic\">");
		if (TMP == 0)break;
		curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 23;
		Des[curpos - 2] = ' ';
		while (Des[curpos + curP] != '<' && Des[curpos + curP] != '\n' && Des[curpos + curP] != '	')curP++;
		memcpy(&pron[curp], &Des[curpos], sizeof(wchar_t) * curP);
		wcscat_s(tmp, pron);
		wcscat_s(tmp, L" ");
	}

	wcscat_s(tmp, L"\n\n解释：\n");

	TMP = wcsstr(Des, L"<ul>");
	int pos1 = (TMP - Des) * 2 / sizeof(wchar_t);

	TMP = wcsstr(TMP, L"</ul>");
	int pos2 = (TMP - Des) * 2 / sizeof(wchar_t);

	while (1)
	{
		ZeroMemory(pron, sizeof(pron));
		TMP = wcsstr(&Des[pos1], L"<li>");
		if (TMP == 0)break;
		int curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 4;
		if (curpos > pos2)break;
		int curp = 0, curP = 0;
		Des[curpos - 2] = ' ';
		while (Des[curpos + curp] != '<' && Des[curpos + curp] != '\n' && Des[curpos + curp] != '	')curp++;
		memcpy(pron, &Des[curpos], sizeof(wchar_t) * curp);


		pron[curp] = '\n';
		wcscat_s(tmp, pron);
	}
	wcscat_s(tmp, L"\n例句：\n");
	while (1)
	{
		ZeroMemory(sen, sizeof(sen));
		TMP = wcsstr(Des, L"<div class=\"examples\">");

		if (TMP == 0)break;
		int curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 4;
		memset(TMP, ' ', sizeof(wchar_t));
		//   memcpy(pron,D)
		//Des[curpos] = ' ';

		TMP = wcsstr(TMP, L"<p>");

		curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 3;
		Des[curpos - 2] = ' ';
		int curp = 0, curP = 0;

		while (Des[curpos + curp] != '<' && Des[curpos + curp] != '\n' && Des[curpos + curp] != '	')curp++;
		memcpy(sen, &Des[curpos], sizeof(wchar_t) * curp);

		sen[curp++] = '\n';

		TMP = wcsstr(TMP, L"<p>");
		curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 3;

		Des[curpos - 2] = ' ';
		while (Des[curpos + curP] != '<' && Des[curpos + curP] != '\n' && Des[curpos + curp] != '	')curP++;
		memcpy(&sen[curp], &Des[curpos], sizeof(wchar_t) * curP);

		wcscat_s(tmp, sen);
		wcscat_s(tmp, L"\n");
	}

	wcscat_s(tmp, L"\n相关短语(来自于网络，不一定可靠)：\n");
	int ul[1001] = { 0 }, ui[1001] = { 0 }, maxul = 0, curul = 0;
	TMP = Des;
	while (1)
	{
		TMP = wcsstr(TMP, L"<ul>");
		ul[maxul] = (TMP - Des) * 2 / sizeof(wchar_t);
		if (TMP == 0)break;
		TMP = wcsstr(TMP, L"</ul>");
		ui[maxul] = (TMP - Des) * 2 / sizeof(wchar_t);
		if (TMP == 0)break;
		maxul++;
	}

	TMP = wcsstr(Des, L"<div id=\"synonyms\" class=\"trans-container tab-content hide\">");
	int maxpos = (TMP - Des) * 2 / sizeof(wchar_t);
	while (1)
	{
		ZeroMemory(sen, sizeof(sen));
		TMP = wcsstr(Des, L"<a class=\"search-js\"");

		if (TMP == 0)break;
		int curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 4;

		if (curpos > maxpos)break;

		while (ui[curul] < curpos && curul < maxul)curul++;

		memset(TMP, ' ', sizeof(wchar_t));
		if (ul[curul] < curpos)continue;
		TMP = wcsstr(TMP, L">");

		curpos = (TMP - Des) * 2 / sizeof(wchar_t) + 1;
		int curp = 0, curP = 0;


		while (Des[curpos + curp] != '<' && Des[curpos + curp] != '\n' && Des[curpos + curp] != '	')curp++;
		memcpy(sen, &Des[curpos], sizeof(wchar_t) * curp);

		sen[curp++] = '\n';

		curpos = curpos + 11 + curp;

		int Curp = curP;
		while (Des[curpos + Curp] != '<')
		{
			Curp++;
			if (Des[curpos + Curp] != '\n' && Des[curpos + Curp] != '	' && Des[curpos + Curp] != ' ' && Des[curpos + Curp] != '<')sen[curp + curP] = Des[curpos + Curp], curP++;
		}
		wcscpy_s(TmpString, sen);
		_wcslwr_s(TmpString);
		wcscpy_s(TmpString2, curword);
		_wcslwr_s(TmpString2);
		if (wcsstr(TmpString, TmpString2) == 0)continue;
		wcscat_s(tmp, sen);
		if (curP != 0)wcscat_s(tmp, L"\n");
	}
	wcscpy_s(TmpString, FilePath);
	wcscat_s(TmpString, L"tmp.txt");

	HANDLE handle;
	handle = CreateFile(TmpString, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
	CloseHandle(handle);

	//FILE *p_source = NULL;
	p_source = _wfopen(TmpString, L"w,ccs=utf-8");
	fwprintf_s(p_source, L"%ws", tmp);
	fcloseall();
	wcscpy_s(TmpString2, L"notepad.exe ");
	wcscat_s(TmpString2, TmpString);
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	CreateProcessW(0, TmpString2, 0, 0, FALSE, 0, 0, 0, &si, &pi);
	wcscpy_s(sen, FilePath);
	wcscat_s(sen, L"index.html");
	DeleteFile(sen);
	delete[]Des;
	return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,//入口点
	LPWSTR lpCmdLine, int nCmdShow) {
	//注册窗口类
	MyRegisterClass(hInstance);
	RegisterGuide(hInstance);
	RegisterSet(hInstance);
	//初始化窗口
	GetModuleFileName(NULL, FilePath, MAX_PATH);
	(_tcsrchr(FilePath, _T('\\')))[1] = 0;

	wcscpy_s(TmpString, FilePath);
	wcscat_s(TmpString, L"错词本.ini");

	HANDLE handle;
	handle = CreateFile(TmpString, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (handle != INVALID_HANDLE_VALUE)NotFirst = true;
	CloseHandle(handle);
	if (!InitInstance(hInstance, nCmdShow))return FALSE;

	//得到程序路径

	x = GetTickCount() * 11 + 34;//设置随机数种子
	y = GetTickCount() * 46 + 51;
	z = GetTickCount() * 67 + 28;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUI));
	MSG msg;
	// 主消息循环: 
	while (GetMessageW(&msg, 0, 0, 0))//等待消息
	{
		GetCursorPos(&Point);//消息循环中获取鼠标位置
		ScreenToClient(MainWnd, &Point);//全局坐标->当前窗口坐标
		GetCursorPos(&points);
		ScreenToClient(Set, &points);
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);//翻译字符消息
			DispatchMessage(&msg);//分发消息
		}
	}

	return (int)msg.wParam;
}
void RefreshAll()
{
	rc.left = 0;
	rc.top = 445;
	rc.right = 800;
	rc.bottom = 500;
	InvalidateRect(MainWnd, &rc, FALSE);
	rc.left = 180;
	rc.top = 140;
	rc.right = 600;
	rc.bottom = 445;
	InvalidateRect(MainWnd, &rc, FALSE);
	rc.left = 0;
	rc.top = 50;
	rc.right = 800;
	rc.bottom = 140;
	InvalidateRect(MainWnd, &rc, FALSE);
	rc.left = 0;
	rc.top = 190;
	rc.right = 200;
	rc.bottom = 445;
	InvalidateRect(MainWnd, &rc, FALSE);
}

void GetRECTc(int curcoverc1, int curcoverc2)
{
	rc.top = choose[curcoverc1].ch[curcoverc2].top;
	rc.bottom = choose[curcoverc1].ch[curcoverc2].top + 15;
	rc.left = choose[curcoverc1].ch[curcoverc2].left;
	rc.right = choose[curcoverc1].ch[curcoverc2].left + 150;
}
LRESULT CALLBACK SetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;//初始化GDI绘图
	HDC Hdc;
	switch (message)
	{
	case WM_PAINT:
		Hdc = BeginPaint(hWnd, &ps);
		SelectObject(Hdc, st);
		for (int i = 1; i <= curset; i++)
		{

			SelectObject(Hdc, BLACK);
			if (curcovers == i)
				if (presss == 1)
					SelectObject(Hdc, BRush);
				else
					SelectObject(Hdc, brush);
			else
				SelectObject(Hdc, white);
			Rectangle(Hdc, set[i].left, set[i].top, set[i].left + set[i].width, set[i].top + set[i].height);

			GetRECTs(i);
			SetBkMode(Hdc, TRANSPARENT);//去掉文字背景

			DrawTextW(Hdc, set[i].name, (int)wcslen(set[i].name), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}
		for (int i = 1; i <= 10; i++)
		{
			for (int j = 1; j < choose[i].maxchoose; j++)
			{
				SelectObject(Hdc, Grey);
				SelectObject(Hdc, white);
				RoundRect(Hdc, choose[i].ch[j].left, choose[i].ch[j].top, choose[i].ch[j].left + 15, choose[i].ch[j].top + 15, 20, 20);
				if (choose[i].curchoosen == j)
				{
					SelectObject(Hdc, White);
					SelectObject(Hdc, Green);
					RoundRect(Hdc, choose[i].ch[j].left + 3, choose[i].ch[j].top + 3, choose[i].ch[j].left + 12, choose[i].ch[j].top + 12, 20, 20);
				}
				SetBkMode(Hdc, TRANSPARENT);//去掉文字背景
				rc.top = choose[i].ch[j].top - 5;
				rc.bottom = choose[i].ch[j].top + 20;
				rc.left = choose[i].ch[j].left + 23;
				rc.right = choose[i].ch[j].left + 200;
				DrawTextW(Hdc, choose[i].ch[j].name, (int)wcslen(choose[i].ch[j].name), &rc, DT_VCENTER | DT_SINGLELINE);

			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (curcovers != -1)
		{
			presss = 1;
			GetRECTs(curcovers);
			InvalidateRect(Set, &rc, FALSE);
		}
		if (curcoverc1 != -1)
		{
			pressc = 1;
			GetRECTc(curcoverc1, curcoverc2);
			InvalidateRect(Set, &rc, FALSE);
		}
		break;
	case WM_MOUSEMOVE:

		if (curcovers == -1)//在外面
		{
			for (int i = 0; i <= curset; i++)
			{
				if (Insides(i))
				{
					curcovers = i;
					GetRECTs(i);
					InvalidateRect(Set, &rc, FALSE);
					break;
				}
			}
		}
		if (curcovers >= 0)
		{
			if (!Insides(curcovers))
			{
				GetRECTs(curcovers);
				curcovers = -1;
				InvalidateRect(Set, &rc, FALSE);
			}
		}

		if (curcoverc1 == -1)//在外面
		{
			for (int i = 1; i < 10; i++)
			{
				for (int j = 1; j < choose[i].maxchoose; j++)
					if (Insidec(i, j))
					{

						curcoverc1 = i;
						curcoverc2 = j;
						GetRECTc(i, j);
						InvalidateRect(Set, &rc, FALSE);
						//s(0);
						break;
					}
			}
		}
		if (curcoverc1 >= 0)
		{
			if (!Insidec(curcoverc1, curcoverc2))
			{
				GetRECTc(curcoverc1, curcoverc2);
				curcoverc1 = curcoverc1 = -1;
				InvalidateRect(Set, &rc, FALSE);
			}
		}
		break;
	case WM_LBUTTONUP:

		if (curcovers != -1)
		{
			presss = 0;
			GetRECTs(curcovers);
			InvalidateRect(Set, &rc, FALSE);
		}
		if (curcoverc1 != -1)
		{
			pressc = 0;
			//ZeroMemory()
			GetRECTc(curcoverc1, choose[curcoverc1].curchoosen);
			choose[curcoverc1].curchoosen = curcoverc2;
			InvalidateRect(Set, &rc, FALSE);
			GetRECTc(curcoverc1, choose[curcoverc1].curchoosen);
			InvalidateRect(Set, &rc, FALSE);
			//choose[curcoverc1].curchoosen = 1- choose[curcoverc1].curchoosen ;
			//GetRECTc(curcoverc1,curcoverc2);
			//InvalidateRect(Set, &rc, FALSE);
		}

		for (int i = 0; i <= curset; i++)
		{
			if (Insides(i))
			{
				//自绘按钮响应，用i标识
				switch (i)
				{
				case 1:
					if (choose[2].curchoosen != 3)break;
					GetWindowTextW(PassWd, TmpString, 50);
					wcscpy_s(curword, TmpString);
					ZeroMemory(curcn, sizeof(curcn));
					gameover = false;//重置状态
					memset(Right, false, 50);
					memset(worduse, false, 26);
					killstate = 0;
					if (dfmod == 1)killstate = 4;
					SetWindowText(Input, L"a");
					lasts = 'a';
					bool xf = false;
					for (int i = 0; i < 50; i++)if (curword[i] < 'a' || curword[i]>'z')Right[i] = true; else xf = true;
					if (xf == false)
					{
						RefreshAll();
						MessageBox(MainWnd, L"双人单词不符合要求!", L"提示", MB_ICONINFORMATION);
						memset(Right, false, 50);
						memset(worduse, false, 26);
						gameover = true;

						Read();
						RefreshAll();
					}
					RefreshAll();
					break;
				}
			}
		}

		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < choose[i].maxchoose; j++)
				if (Insidec(i, j))
				{
					if (i == 1)dfmod = j - 1;
				}
		}
		break;
	case WM_CLOSE:
		GetWindowTextW(PassWd, PassWdBack, 50);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
HICON gui; WNDCLASSEXW wcex;
//注册窗口类。
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	gui = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUI));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = gui;//大图标
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDI_GUI);
	wcex.lpszClassName = L"xiaofei";//类名
	wcex.hIconSm = gui;//小图标
	return RegisterClassExW(&wcex);//正式注册
}

ATOM RegisterSet(HINSTANCE hInstance)
{
	gui = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUI));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SetProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = gui;//大图标
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDI_GUI);
	wcex.lpszClassName = L"xiaofeisett";//类名
	wcex.hIconSm = gui;//小图标
	return RegisterClassExW(&wcex);//正式注册
}


void SearchDictionaryFile()
{
	wchar_t lpPath[255] = { 0 }, szFind[255] = { 0 };
	WIN32_FIND_DATA FindFileData;
	GetModuleFileName(NULL, lpPath, MAX_PATH);
	(_tcsrchr(lpPath, _T('\\')))[1] = 0;
	wcscpy_s(szFind, lpPath);
	wcscat_s(szFind, L"*.ini");
	HANDLE hFind = ::FindFirstFile((LPCWSTR)szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)return;
	while (TRUE)
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			SendMessage(FileList, LB_ADDSTRING, 0, (LPARAM)FindFileData.cFileName);
		}
		if (!FindNextFile(hFind, &FindFileData))break;
	}
	FindClose(hFind);
}



DWORD WINAPI Thread(LPVOID pM)
{

	if (isoffline)
	{
		MCI_OPEN_PARMS mciOpen;
		mciOpen.lpstrDeviceType = L"mpegvideo";
		wcscpy_s(TmpString, FilePath);
		wcscat_s(TmpString, L"music\\");
		wcscat_s(TmpString, curword);
		wcscat_s(TmpString, L".mp3");

		mciOpen.lpstrElementName = TmpString;
		if (mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen) != 0)return 0;
		UINT DeviceID = mciOpen.wDeviceID;
		MCI_PLAY_PARMS mciPlay;
		mciSendCommand(DeviceID, MCI_PLAY, 0, (DWORD)&mciPlay);
	}
	else
	{
		int n = (int)pM;
		URLDownloadToFileW(NULL, Readtmp, Readtmp2[n], 0, 0);
		MCI_OPEN_PARMS mciOpen;
		mciOpen.lpstrDeviceType = L"mpegvideo";
		mciOpen.lpstrElementName = Readtmp2[n];
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
		UINT DeviceID = mciOpen.wDeviceID;
		MCI_PLAY_PARMS mciPlay;
		mciSendCommand(DeviceID, MCI_PLAY, 0, (DWORD)&mciPlay);
		DeleteFile(Readtmp2[n]);
	}
	return 0;
}

void Read()
{
	curdownload++;
	wcscpy_s(Readtmp, L"http://dict.youdao.com/dictvoice?audio=");
	wcscat_s(Readtmp, curword);
	wcscpy_s(Readtmp2[curdownload], FilePath);
	wcscat_s(Readtmp2[curdownload], L"tmp");
	_itow_s(curdownload, TmpString, 10);
	wcscat_s(Readtmp2[curdownload], TmpString);
	wcscat_s(Readtmp2[curdownload], L".mp3");

	CreateThread(NULL, 0, Thread, (LPVOID)curdownload, 0, NULL);
}
void s(LPWSTR m)
{
	MessageBox(0, m, L"调试", 0);
}
int Testin()
{
	if (gameover)return 0;//游戏结束直接无视

	GetWindowTextW(Input, TmpString, 10);

	bool right, pass; //是否猜对一个字母 & 是否至今猜对了所有字母
	right = pass = false;

	if (dfmod == 3)
	{
		wchar_t let = TmpString[0];
		int count[513] = { 0 }, maxpos, cpb = curpossible, ff = 0;
		for (int i = 0; i < curpossible; i++)
		{
			maxpos = 0;
			for (int j = 0; j < word; j++)
				if (PossibleChoose[i][j] == let)maxpos += (int)pow(2, j);
			count[maxpos]++;
		}
		maxpos = 0;
		for (int i = 0; i < 513; i++)if (count[maxpos] < count[i])maxpos = i;
		bool isright[10] = { false };

		//_itow_s(maxpos, TmpString2, 10);
		//s(TmpString2);
		//s(PossibleChoose[0]);
		for (int i = 8; i >= 0; i--)
		{
			if (maxpos - pow(2, i) >= 0)isright[i] = true, maxpos -= pow(2, i), Right[i] = true, right = true;
		}

		curpossible = 0;
		for (int i = 0; i < cpb; i++)
		{
			ff = 0;
			for (int j = 0; j < word; j++)
			{
				if ((PossibleChoose[i][j] == let && isright[j] == true) || (PossibleChoose[i][j] != let && isright[j] == false)); else { ff = 1; break; }
			}

			if (ff == 0)wcscpy_s(PossibleChoose[curpossible++], PossibleChoose[i]);
			else ZeroMemory(PossibleChoose[i], sizeof(PossibleChoose[i]));
		}
		wcscpy_s(curword, PossibleChoose[rnd() % curpossible]);
		goto df;
	}

	for (int i = 0; i < word; ++i)
	{
		if (Right[i] == false && TmpString[0] == curword[i])
		{
			Right[i] = true;
			right = true;
		}
	}
df:
	GetWindowTextW(Input, TmpString, 10);
	if (right == false && killstate < 12 && worduse[TmpString[0] - 97] == false)if (dfmod == 2)killstate += 2; else killstate++;
	worduse[TmpString[0] - 97] = true;

	RefreshAll();

	if (killstate == 12)
	{
		for (int i = 0; i < word; ++i)Right[i] = true;
		if (dfmod == 3)MakeChinese();
		RefreshAll();
		MessageBox(MainWnd, L"您失败了!", L"提示", MB_ICONINFORMATION);
		gameover = true;
		RefreshAll();
		Read();
		return 0;
	}

	for (int i = 0; i < word; ++i)if (Right[i] == false) pass = true;

	if (pass == false)
	{
		if (dfmod == 3)MakeChinese();
		RefreshAll();
		MessageBox(MainWnd, L"您成功了!", L"提示", MB_ICONINFORMATION);
		gameover = true;

		Read();
		RefreshAll();
	}
	return 0;
}

void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	bool smaller = false;
	if (nTimerid == 1)
	{
		if (flashtype < 12)flashtype++, InvalidateRect(Guide, 0, FALSE); else KillTimer(Guide, 1);
	}
	if (nTimerid == 2)
	{
		for (int i = 0; i <= curzxf; i++)
		{
			if (zxf[i].state != -1)
			{
				if (zxf[i].state < 0)smaller = true;
				if (smaller == true)
					if (zxf[i].state <= -zxf[i].width / 4)
						zxf[i].state += (int)((double)((int)zxf[i].width / 40.0) + 0.99);
					else
						zxf[i].state += (int)((double)((int)zxf[i].width / 8.0) + 0.99);
				else
					if (zxf[i].state <= zxf[i].width / 4)
						zxf[i].state += (int)((double)((int)zxf[i].width / 6.0) + 0.99);
					else
						zxf[i].state += (int)((double)((int)zxf[i].width / 18.0) + 0.99);

				GetRECT(i);

				InvalidateRect(MainWnd, &rc, FALSE);
				if ((zxf[i].state > zxf[i].width / 2) || (zxf[i].state >= 0 && smaller == true))
				{
					zxf[i].state = -1;
					GetRECT(i);
					rc.left -= 1;
					rc.bottom += 1;
					rc.right += 1;
					rc.top -= 1;

					InvalidateRect(MainWnd, &rc, FALSE);
				}
			}
		}
	}
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	//创建三个窗口
	MainWnd = CreateWindowW(L"xiaofei", L"小飞猜词", WS_OVERLAPPEDWINDOW, 200, 200, 800, 500, 0, 0, hInstance, 0);//200 不知能不能居中
	Input = CreateWindowW(L"EDIT", L"a", ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | DT_VCENTER | ES_CENTER, 10, 160, 139, 60, MainWnd, (HMENU)1, hInstance, 0);
	FileList = CreateWindowW(L"ListBox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 600, 140, 200, 320, MainWnd, (HMENU)2, hInstance, 0);
	if (NotFirst == false)Guide = CreateWindowW(L"guider", NULL, 0, 0, 0, 800, 500, 0, 0, hInstance, 0);
	//窗口透明度,
	SetWindowLong(MainWnd, GWL_EXSTYLE, GetWindowLong(MainWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(MainWnd, NULL, 234, LWA_ALPHA);
	if (NotFirst == false)SetWindowLong(Guide, GWL_EXSTYLE, GetWindowLong(Guide, GWL_EXSTYLE) | WS_EX_LAYERED);
	if (NotFirst == false)SetLayeredWindowAttributes(Guide, NULL, 180, LWA_ALPHA);
	//三个不同大小字体
	st = CreateFontW(16, 8, 0, 0, FW_THIN, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 2, DEFAULT_PITCH | FF_SWISS, _T("新宋体"));
	stlarge = CreateFontW(32, 16, 0, 0, 700, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 2, DEFAULT_PITCH | FF_SWISS, _T("新宋体"));
	stbig = CreateFontW(28, 13, 0, 0, FW_THIN, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 2, DEFAULT_PITCH | FF_SWISS, _T("Calibri"));

	//设置控件字体 & 字符位置
	::SendMessage(FileList, WM_SETFONT, (WPARAM)st, 1);
	::SendMessage(Input, WM_SETFONT, (WPARAM)stlarge, 1);
	GetClientRect(Input, &rc);
	rc.top += 12;
	rc.bottom += 12;
	::SendMessage(Input, EM_SETRECT, 0, (LPARAM)&rc);

	//设置窗口无边框
	::SetWindowLong(MainWnd, GWL_STYLE, GetWindowLong(MainWnd, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME);
	::SetWindowPos(MainWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	if (NotFirst == false)::SetWindowLong(Guide, GWL_STYLE, GetWindowLong(Guide, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME);
	if (NotFirst == false)::SetWindowPos(Guide, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	//创建按钮
	RegisterHotKey(MainWnd, 1, MOD_ALT, 'K');
	CreateButton(10, 230, 139, 60, L"随机单词");
	CreateButton(10, 300, 139, 60, L"放弃猜词");
	CreateButton(10, 370, 139, 60, L"加至错词本");
	CreateButton(180, 320, 30, 30, L"Q");
	CreateButton(220, 320, 30, 30, L"W");
	CreateButton(260, 320, 30, 30, L"E");
	CreateButton(300, 320, 30, 30, L"R");
	CreateButton(340, 320, 30, 30, L"T");
	CreateButton(380, 320, 30, 30, L"Y");
	CreateButton(420, 320, 30, 30, L"U");
	CreateButton(460, 320, 30, 30, L"I");
	CreateButton(500, 320, 30, 30, L"O");
	CreateButton(540, 320, 30, 30, L"P");
	CreateButton(200, 360, 30, 30, L"A");
	CreateButton(240, 360, 30, 30, L"S");
	CreateButton(280, 360, 30, 30, L"D");
	CreateButton(320, 360, 30, 30, L"F");
	CreateButton(360, 360, 30, 30, L"G");
	CreateButton(400, 360, 30, 30, L"H");
	CreateButton(440, 360, 30, 30, L"J");
	CreateButton(480, 360, 30, 30, L"K");
	CreateButton(520, 360, 30, 30, L"L");
	CreateButton(220, 400, 30, 30, L"Z");
	CreateButton(260, 400, 30, 30, L"X");
	CreateButton(300, 400, 30, 30, L"C");
	CreateButton(340, 400, 30, 30, L"V");
	CreateButton(380, 400, 30, 30, L"B");
	CreateButton(420, 400, 30, 30, L"N");
	CreateButton(460, 400, 30, 30, L"M");

	CreateButtonS(140, 105, 70, 50, L"确定");
	CheateChoose(10, 10, L"单人模式", 2);
	CheateChoose(10, 40, L"单人模式(允许标点)", 2);
	CheateChoose(10, 70, L"双人模式", 2);


	CheateChoose(10, 180, L"简单模式(12次机会)", 1);
	CheateChoose(10, 210, L"普通模式(8次机会)", 1);
	CheateChoose(10, 240, L"困难模式(6次机会)", 1);
	CheateChoose(10, 270, L"极限模式(12次机会)", 1);

	SetTimer(MainWnd, 2, 10, (TIMERPROC)TimerProc);
	if (NotFirst == false)SetTimer(Guide, 1, 100, (TIMERPROC)TimerProc);
	//添加*.ini
	SearchDictionaryFile();

	//关闭按钮
	zxf[0].top = 10;
	zxf[0].left = 715;
	zxf[0].height = 30;
	zxf[0].width = 60;
	zxf[0].name = L"×";

	//创建失败则退出
	if (!MainWnd)return FALSE;

	//显示 & 绘制窗口
	ShowWindow(MainWnd, nCmdShow);
	if (NotFirst == false)ShowWindow(Guide, SW_SHOW);
	UpdateWindow(MainWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;//初始化GDI绘图
	HDC hdc;
	switch (message)
	{
	case	WM_CREATE:
		hdc = GetDC(MainWnd);
		mdc = CreateCompatibleDC(hdc);
		hBmp = CreateCompatibleBitmap(hdc, 1000, 600);
		SelectObject(mdc, hBmp);
		ReleaseDC(MainWnd, hdc);

		break;
	case WM_HOTKEY:
		if (isoffline == false)isoffline = true; else isoffline = false;
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		SetBkMode(mdc, TRANSPARENT);//背景
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(mdc, white);
		SelectObject(mdc, White);
		Rectangle(mdc, 0, 0, 800, 500);
		SetTextColor(mdc, RGB(255, 255, 255));//字符颜色
		SelectObject(mdc, st);//选择"宋体"

		SelectObject(mdc, GREEN);
		SelectObject(mdc, green);
		Rectangle(mdc, 0, 0, 800, 50);


		TextOut(mdc, 57, 17, L"小飞猜词", wcslen(L"小飞猜词"));
		if (curcover == 0)
			if (press == 0)
				SelectObject(mdc, close2), SelectObject(mdc, CLOSE2);
			else
				SelectObject(mdc, close3), SelectObject(mdc, CLOSE3);
		else
			SelectObject(mdc, close1), SelectObject(mdc, CLOSE1);

		Rectangle(mdc, zxf[0].left, zxf[0].top, zxf[0].left + zxf[0].width, zxf[0].top + zxf[0].height);
		SetTextColor(mdc, RGB(240, 240, 240));
		GetRECT(0);

		DrawTextW(mdc, L"×", 1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		SetTextColor(mdc, RGB(0, 0, 0));

		SelectObject(mdc, BLACK);
		SelectObject(mdc, white);

		int p1 = 0, p2 = 50, p3 = 799, p4 = 499;//大frame
		MoveToEx(mdc, p1, p2, NULL);
		LineTo(mdc, p1, p4);
		LineTo(mdc, p3, p4);
		LineTo(mdc, p3, p2);
		LineTo(mdc, p1, p2);

		MoveToEx(mdc, 0, 140, 0);
		LineTo(mdc, 800, 140);

		MoveToEx(mdc, 600, 150, 0);
		LineTo(mdc, 600, 445);

		for (int i = 1; i <= curzxf; i++)
		{

			SelectObject(mdc, BLACK);
			if (curcover == i)
				if (press == 1)
					SelectObject(mdc, BRush);
				else
					SelectObject(mdc, brush);
			else
				SelectObject(mdc, white);
			if (zxf[i].state != -1)SelectObject(mdc, white);
			RoundRect(mdc, zxf[i].left, zxf[i].top, zxf[i].left + zxf[i].width, zxf[i].top + zxf[i].height, 10, 10);
			if (zxf[i].state != -1)
			{
				if (press == 1)
					SelectObject(mdc, BRush);
				else
					SelectObject(mdc, brush);
				SelectObject(mdc, White);
				RoundRect(mdc,
					zxf[i].left + zxf[i].width / 2 - labs(zxf[i].state) + 1,
					1 + zxf[i].top + zxf[i].height / 2 - (labs(zxf[i].state) * zxf[i].height / zxf[i].width),
					zxf[i].left + zxf[i].width / 2 + labs(zxf[i].state) - 1,
					zxf[i].top + zxf[i].height / 2 + (labs(zxf[i].state) * zxf[i].height / zxf[i].width - 1),
					20, 20);

			}
			GetRECT(i);
			if (i < 4)rc.left += 50;
			SetBkMode(mdc, TRANSPARENT);//去掉文字背景
			if (i == 2 && gameover == true)DrawTextW(mdc, L"朗读单词", 5, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			else
				DrawTextW(mdc, zxf[i].name, (int)wcslen(zxf[i].name), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		}

		int less = 0, rless = 0;
		word = wcslen(curword);
		if (word != 0)
		{
			if (word > 13)less = 61 - (800 / word), rless = (int)(less / 2.4);
			SelectObject(mdc, ::CreateFontW(32 - less / 2, 16 - less / 4, 0, 0, 700, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 2, DEFAULT_PITCH | FF_SWISS, _T("新宋体")));
			for (int i = 1; i <= word; ++i)
			{
				SelectObject(mdc, BLACK);
				SelectObject(mdc, white);

				if (word % 2 != 0)
				{
					if (curword[i - 1] != ' ')Rectangle(mdc, 314 + rless * 2 + (i - word / 2) * (60 - less), 70 + rless, 364 + (i - word / 2) * (60 - less), 120 - rless);
					if (Right[i - 1] || (curword[i - 1] < 'a' || curword[i - 1]>'z'))TextOut(mdc, 330 + (int)((i - word / 2) * (60 - less) + rless * 1.3), 75 + (int)(rless / 1.4), &curword[i - 1], 1);

				}
				else
				{
					if (curword[i - 1] != ' ')Rectangle(mdc, 345 + rless * 2 + (i - word / 2) * (60 - less), 70 + rless, 395 + (i - word / 2) * (60 - less), 120 - rless);
					if (Right[i - 1] || (curword[i - 1] < 'a' || curword[i - 1]>'z'))TextOut(mdc, 361 + (int)((i - word / 2) * (60 - less) + rless * 1.3), (int)(75 + rless / 1.4), &curword[i - 1], 1);
				}

			}
		}

		wchar_t wordtmp;
		SelectObject(mdc, stbig);
		SelectObject(mdc, BLACK);
		MoveToEx(mdc, 0, 445, NULL);
		LineTo(mdc, 800, 445);
		if (gameover)
		{
			wcscpy_s(TmpString, curword);
			wcscat_s(TmpString, L" ");
			wcscat_s(TmpString, curcn);
			TextOut(mdc, 20, 458, TmpString, wcslen(TmpString));
		}
		else
		{
			SelectObject(mdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));
			for (int i = 0; i < 26; ++i)
			{
				wordtmp = i + 97;
				TextOut(mdc, i * 29 + 30, 458, &wordtmp, 1);
				if (worduse[i])
				{
					MoveToEx(mdc, i * 29 + 25, 462, NULL);
					LineTo(mdc, i * 29 + 45, 482);
				}
			}
		}

		HICON hicon; //MessageBox(0, MAKEINTRESOURCE(IDI_GUI), 0, 0);
		if (gameover)
			hicon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ICON3));
		else
			hicon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_SMALL));
		DrawIconEx(mdc, 20, 313, hicon, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		hicon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ADD));
		DrawIconEx(mdc, 20, 383, hicon, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		hicon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_RANDOM));
		DrawIconEx(mdc, 20, 243, hicon, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		hicon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_GUI));
		DrawIconEx(mdc, 15, 7, hicon, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		if (gameover) {
			hicon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ICON4));
			DrawIconEx(mdc, 750, 455, hicon, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		}

		rc.left = 460; rc.right = 800; rc.top = 150; rc.bottom = 500;
		SelectObject(mdc, st);

		int kleft = 465, kbottom = 280;
		SelectObject(mdc, CreatePen(PS_SOLID, 2, RGB(20 * killstate, 255 - 20 * killstate, 20)));
		if (killstate > 0)
		{
			MoveToEx(mdc, kleft, kbottom, NULL);
			LineTo(mdc, kleft + 75, kbottom);
		}
		if (killstate > 1)
		{
			MoveToEx(mdc, kleft + 25, kbottom, NULL);
			LineTo(mdc, kleft + 25, kbottom - 100);
		}
		if (killstate > 2)
		{
			MoveToEx(mdc, kleft + 15, kbottom - 100, NULL);
			LineTo(mdc, kleft + 70, kbottom - 100);
		}
		if (killstate > 3)
		{
			MoveToEx(mdc, kleft + 62, kbottom - 100, NULL);
			LineTo(mdc, kleft + 62, kbottom - 85);
		}
		if (killstate > 4)
		{
			RoundRect(mdc, kleft + 50, kbottom - 85, kleft + 74, kbottom - 61, 30, 30);
		}
		if (killstate > 5)
		{
			MoveToEx(mdc, kleft + 62, kbottom - 61, NULL);
			LineTo(mdc, kleft + 62, kbottom - 53);
		}
		if (killstate > 6)
		{
			MoveToEx(mdc, kleft + 62, kbottom - 53, NULL);
			LineTo(mdc, kleft + 45, kbottom - 57);
		}
		if (killstate > 7)
		{
			MoveToEx(mdc, kleft + 62, kbottom - 53, NULL);
			LineTo(mdc, kleft + 79, kbottom - 57);
		}
		if (killstate > 8)
		{
			MoveToEx(mdc, kleft + 62, kbottom - 53, NULL);
			LineTo(mdc, kleft + 52, kbottom - 32);
		}
		if (killstate > 9)
		{
			MoveToEx(mdc, kleft + 62, kbottom - 53, NULL);
			LineTo(mdc, kleft + 73, kbottom - 32);
		}
		if (killstate > 10)
		{
			MoveToEx(mdc, kleft + 56, kbottom - 80, NULL);
			LineTo(mdc, kleft + 60, kbottom - 76);
			MoveToEx(mdc, kleft + 60, kbottom - 80, NULL);
			LineTo(mdc, kleft + 56, kbottom - 76);
		}
		if (killstate > 11)
		{
			MoveToEx(mdc, kleft + 64, kbottom - 80, NULL);
			LineTo(mdc, kleft + 68, kbottom - 76);
			MoveToEx(mdc, kleft + 68, kbottom - 80, NULL);
			LineTo(mdc, kleft + 64, kbottom - 76);
		}
		if (dfmod == 2)
			_itow_s((12 - killstate) / 2, TmpString, 10);
		else
			_itow_s(12 - killstate, TmpString, 10);
		wcscpy_s(TmpString2, L"剩余次数: ");
		wcscat_s(TmpString2, TmpString);
		TextOut(mdc, 300, 200, TmpString2, wcslen(TmpString2));
		BitBlt(hdc, 0, 0, 800, 600, mdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_COMMAND:
		int wmId; wmId = LOWORD(wParam);
		switch (wmId)
		{
		case 1:
			if (HIWORD(wParam) != EN_CHANGE)break;
			GetWindowTextW(Input, TmpString, 20);
			if (TmpString[0] < 97 || TmpString[0]>122) { SetWindowTextW(Input, &lasts); break; }
			if (wcslen(TmpString) > 1)
			{
				if (TmpString[1] < 97 || TmpString[1]>122) { SetWindowTextW(Input, &lasts); break; }
				if (lasts != TmpString[1])
				{
					lasts = TmpString[1];
					SetWindowTextW(Input, &lasts);
				}
				else
				{
					lasts = TmpString[0];
					SetWindowTextW(Input, &lasts);

				}
			}
			if (wcslen(TmpString) == 0) TmpString[0] = lasts;
			Testin();
			break;
		case 2:

			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				ZeroMemory(TmpString2, sizeof(TmpString2));
				SendMessage(FileList, LB_GETTEXT, ::SendMessage(FileList, LB_GETCURSEL, 0, 0), (LPARAM)TmpString2);

				wcscpy_s(TmpString, FilePath);
				wcscat_s(TmpString, TmpString2);
				HANDLE hFile;
				hFile = CreateFile(TmpString, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				DWORD dwReads;
				ReadFile(hFile, unicode, 1000000, &dwReads, NULL);
				unicode[dwReads / 2] = NULL;

				ZeroMemory(Chinese, sizeof(Chinese));
				ZeroMemory(Word, sizeof(Word));
				int dev, wcs_len, begin; bool type, FE; FE = type = false;
				wcs_len = wcslen(unicode); dev = line = 0;
				if (unicode[0] == NULL)begin = 1; else begin = 0;
				for (int i = 1; i < wcs_len; ++i)
				{
					if (unicode[i] == ' ' && FE == false) { FE = true; type = true; dev = 0; line++; continue; }
					if (unicode[i] == '\n')
					{
						FE = false;
						type = false;
						dev = 0;
						continue;
					}
					if (type == false)
					{
						if ((unicode[i] < 'a' || unicode[i]>'z') && choose[2].curchoosen != 2) {
							wcscpy_s(TmpString, L"词库错误:\n\n位置:\nLine: ");
							_itow_s(line + 1, TmpString2, 10);
							wcscat_s(TmpString, TmpString2);
							wcscat_s(TmpString, L"\nColumn: ");
							_itow_s(dev + 1, TmpString2, 10);
							wcscat_s(TmpString, TmpString2);
							wcscat_s(TmpString, L"\nLetter: \"");
							ZeroMemory(TmpString2, sizeof(TmpString2));
							TmpString2[0] = unicode[i];
							wcscat_s(TmpString, TmpString2);
							wcscat_s(TmpString, L"\"");
							MessageBox(MainWnd, TmpString, L"提示", MB_ICONINFORMATION);
						}
						Word[line][dev] = unicode[i];
					}
					else Chinese[line - 1][dev] = unicode[i];
					dev++;
				}
				CloseHandle(hFile); SetFocus(MainWnd);
				break;
			}
			break;
		}

		break;

	case WM_LBUTTONDOWN:
		BOOL f; f = FALSE;

		for (int i = 0; i <= curzxf; i++)
			if (Inside(i)) { f = TRUE; break; }
		if (gameover == true && Point.x >= 748 && Point.x <= 784 && Point.y >= 453 && Point.y <= 489) { f = TRUE; break; }
		if (Point.x >= 13 && Point.x <= 49 && Point.y >= 5 && Point.y <= 41) { f = TRUE; break; }
		if (f == FALSE)PostMessage(MainWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);

		if (curcover != -1)
		{
			press = 1;
			GetRECT(curcover);
			InvalidateRect(MainWnd, &rc, FALSE);
		}
		break;

	case WM_LBUTTONUP:

		if (curcover != -1)
		{
			press = 0;
			GetRECT(curcover);
			InvalidateRect(MainWnd, &rc, FALSE);
		}

		if (gameover == true && Point.x >= 748 && Point.x <= 784 && Point.y >= 453 && Point.y <= 489) {
			CreateThread(0, 0, ShowMeanings, 0, 0, 0);
		}
		if (Point.x >= 13 && Point.x <= 49 && Point.y >= 5 && Point.y <= 41)
		{
			SendMessage(Set, WM_CLOSE, 0, 0);
			Set = CreateWindowW(L"xiaofeisett", L"设置", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 50, 50, 250, 350, 0, 0, hInst, 0);
			PassWd = CreateWindowW(L"EDIT", 0, ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | DT_VCENTER | ES_CENTER | ES_PASSWORD, 12, 105, 120, 50, Set, (HMENU)1, hInst, 0);
			SendMessage(PassWd, WM_SETFONT, (WPARAM)st, 1);
			GetClientRect(PassWd, &rc);
			rc.top += 10;
			rc.bottom += 10;
			::SendMessage(PassWd, EM_SETRECT, 0, (LPARAM)&rc);

			if (PassWdBack[0] != 0)SetWindowText(PassWd, PassWdBack);
			SendMessage(PassWd, EM_SETPASSWORDCHAR, '*', 0);
			ShowWindow(Set, SW_SHOW);
		}
		for (int i = 0; i <= curzxf; i++)
		{
			if (Inside(i))
			{
				//自绘按钮响应，用i标识
				switch (i)
				{
				case 0://关闭按钮
					DestroyWindow(MainWnd);
					break;

				case 1://"随机"按钮
					int curchoose;//当前选择行
					if (line != 0)curchoose = rnd() % (line); else break;//随机;若未选择词典则退出
					gameover = false;//重置状态
					memset(Right, false, 50);
					memset(worduse, false, 26);
					//memset(Right, false, 20);
					killstate = 0;
					if (dfmod == 1)killstate = 4;
					SetWindowText(Input, L"a");
					lasts = 'a';
					if (dfmod == 3)
					{
						word = rnd() % 7 + 3;
						//word = 3;
						ZeroMemory(PossibleChoose, sizeof(PossibleChoose));
						curpossible = 0;
						for (i = 0; i <= line; ++i)if (wcslen(Word[i]) == word)
						{
							wcscpy_s(PossibleChoose[curpossible], Word[i]);
							curpossible++;
						}
						wcscpy_s(curword, PossibleChoose[rnd() % curpossible]);
						ZeroMemory(curcn, sizeof(curcn));
						RefreshAll();
						break;
					}
					wcscpy_s(curword, Word[curchoose]);
					wcscpy_s(curcn, Chinese[curchoose]);
					RefreshAll();
					break;
				case 2:
					for (int i = 0; i < word; ++i)Right[i] = true;
					gameover = true;
					MakeChinese();
					RefreshAll();
					Read();
					break;
				case 3:
					wchar_t str1[101], str2[101]; int null; null = 0;
					ZeroMemory(str1, sizeof(str1));
					ZeroMemory(str2, sizeof(str2));
					wcscpy_s(str1, curword);
					wcscpy_s(str2, curcn);
					wcscpy_s(TmpString, FilePath);
					wcscat_s(TmpString, L"错词本.ini");
					HANDLE handle; handle = CreateFile(TmpString, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
					//MessageBox(0, 0, 0, 0);
					if (handle == INVALID_HANDLE_VALUE)null = 1;
					CloseHandle(handle);

					FILE* fp; fp = _tfopen(TmpString, _T("ab+"));
					if (fp != NULL)
					{
						if (null == 1)fwrite(u, 2, 1, fp);
						if (null == 1)_ftprintf(fp, _T("%s %s"), str1, str2); else _ftprintf(fp, _T("\r\n%s %s"), str1, str2);
						fclose(fp);
					}
					for (int j = 0; j <= 100; j++)SendMessage(FileList, LB_DELETESTRING, 0, j);
					SearchDictionaryFile();
					RefreshAll();
					break;
				case 4:case 5:case 6:case 7:case 8:case 9:case 10:
				case 11:case 12:case 13:case 14:case 15:case 16:
				case 17:case 18:case 19:case 20:case 21:case 22:
				case 23:case 24:case 25:case 26:case 27:case 28:case 29:
					wchar_t tletter; tletter = letter[i - 4];
					SetWindowText(Input, &tletter);
					Testin();
					break;
				}

			}
		}
		break;

	case WM_KEYUP:
		if (wParam < 65 || wParam>90) break;
		ZeroMemory(TmpString, sizeof(TmpString));
		TmpString[0] = wParam + 32;
		lasts = wParam + 32;
		SetWindowText(Input, TmpString);
		Testin();
		break;
	case WM_MOUSEMOVE:

		if (curcover == -1)//在外面
		{
			for (int i = 0; i <= curzxf; i++)
			{
				if (Inside(i))
				{
					zxf[i].state = 0;
					curcover = i;
					GetRECT(i);
					InvalidateRect(MainWnd, &rc, FALSE);
					break;
				}
			}
		}
		if (curcover >= 0)
		{
			if (!Inside(curcover))
			{
				zxf[curcover].state = -(zxf[curcover].width / 10) * 5 - 1;
				GetRECT(curcover);
				curcover = -1;
				InvalidateRect(MainWnd, &rc, FALSE);
			}
		}

		if (Msv == 0)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = MainWnd;
			tme.dwFlags = TME_LEAVE;
			TrackMouseEvent(&tme);
			Msv = 1;
		}
		else
			Msv = 0;
		break;

	case WM_MOUSELEAVE:
		PostMessage(MainWnd, WM_MOUSEMOVE, NULL, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}