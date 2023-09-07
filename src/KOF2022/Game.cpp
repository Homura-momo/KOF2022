// FIGHTGAME.cpp : 定义应用程序的入口点。
//

#include "Game.h"

using namespace std;


#pragma region 全局变量

#define MAX_LOADSTRING   100  

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名



HBITMAP bmp_BG;               //背景图资源
HBITMAP bmp_HELP;
HBITMAP bmp_END;
HBITMAP bmp_CHOOSE;
HBITMAP bmp_STAGE1;
HBITMAP bmp_STAGE2;
HBITMAP bmp_STAGE3;
HBITMAP bmp_STAGE4;

HBITMAP bmp_StartButton;	//开始按钮图像资源
HBITMAP bmp_HelpButton;		//帮助按钮图像资源
HBITMAP bmp_RestoreButton;  //恢复游戏按钮图像资源
HBITMAP bmp_RestartButton;  //重新开始按钮图像资源
HBITMAP bmp_ReturnButton;   //退出游戏按钮图像资源
HBITMAP bmp_ContinueButton; //继续游戏按钮图像资源
HBITMAP bmp_Stage1Button;   //选择场景按钮图像资源
HBITMAP bmp_Stage2Button;
HBITMAP bmp_Stage3Button;
HBITMAP bmp_Stage4Button;
HBITMAP bmp_KulaButton;		//选择角色按钮图像资源
HBITMAP bmp_KyokusanagiButton;

HBITMAP bmp_Unit_kula;        //库拉角色图像资源
HBITMAP bmp_Unit_kyokusanagi; //草稚京角色图像资源

HBITMAP bmp_Drug1;	//道具图像资源
HBITMAP bmp_Drug2;
HBITMAP bmp_Drug3;

HBITMAP bmp_Background;    //生成的背景图像
HBITMAP bmp_Choose;
HBITMAP bmp_Help;
HBITMAP bmp_End;
HBITMAP bmp_Stage1;
HBITMAP bmp_Stage2;
HBITMAP bmp_Stage3;
HBITMAP bmp_Stage4;



Stage* currentStage = NULL; //当前场景状态
vector<Hero*> heros;        //英雄
vector<Button*> buttons;	//按钮
vector<Prop*> props;		//道具

int mouseX = 0;             // 鼠标X3
int mouseY = 0;				// 鼠标Y
bool mouseDown = false;     // 鼠标按下


const int TOTAL_TYPE = 18;   // 英雄动作类型


// TODO: 在此添加其它全局变量

Hero* role;		//玩家
Hero* enemy;	//敌人


ULONGLONG keyLeftTime = 0;	//记录按下某键的时间
ULONGLONG keyRightTime = 0;
ULONGLONG keyDodgeTime = 0;
ULONGLONG keyDefendTime = 0;

bool success;		//玩家是否胜利
bool gameover;		//游戏是否结束

int chooseStage;	//选择的场景
int chooseRole;		//选择的角色

#pragma endregion


#pragma region Win32程序框架

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FIGHTGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIGHTGAME));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FIGHTGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// 定时器事件
		if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		// 绘图
		Paint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion


#pragma region 事件处理函数

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//加载图像资源
	bmp_BG = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	bmp_CHOOSE = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CHOOSE));
	bmp_HELP = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP2));
	bmp_END = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_END));
	bmp_STAGE1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE1));
	bmp_STAGE2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE2));
	bmp_STAGE3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE3));
	bmp_STAGE4 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE4));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_HelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP));
	bmp_RestoreButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RESTORE));
	bmp_RestartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RESTART));
	bmp_ReturnButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RETURN));
	bmp_ContinueButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CONTINUE));
	bmp_Stage1Button = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE1_S));
	bmp_Stage2Button = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE2_S));
	bmp_Stage3Button = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE3_S));
	bmp_Stage4Button = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE4_S));
	bmp_KulaButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_KULA_S));
	bmp_KyokusanagiButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_KYOKUSANAGI_S));

	bmp_Unit_kula = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_KULA));
	bmp_Unit_kyokusanagi = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_KYOKUSANAGI));

	bmp_Drug1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_DRUG1));
	bmp_Drug2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_DRUG2));
	bmp_Drug3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_DRUG3));

	//添加按钮
	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_NORMAL_WIDTH, BUTTON_NORMAL_HEIGHT, (WINDOW_WIDTH - BUTTON_NORMAL_WIDTH) / 2, WINDOW_HEIGHT - 150);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_NORMAL_WIDTH, BUTTON_NORMAL_HEIGHT, (WINDOW_WIDTH - BUTTON_NORMAL_WIDTH) / 2, WINDOW_HEIGHT - 300);
	Button* continueButton = CreateButton(BUTTON_CONTINUE, bmp_ContinueButton, BUTTON_NORMAL_WIDTH, BUTTON_NORMAL_HEIGHT, (WINDOW_WIDTH - BUTTON_NORMAL_WIDTH) / 2, WINDOW_HEIGHT - 450);
	Button* restoreButton = CreateButton(BUTTON_RESTORE, bmp_RestoreButton, BUTTON_NORMAL_WIDTH, BUTTON_NORMAL_HEIGHT, (WINDOW_WIDTH - BUTTON_NORMAL_WIDTH) / 2, WINDOW_HEIGHT - 450);
	Button* restartButton = CreateButton(BUTTON_RESTART, bmp_RestartButton, BUTTON_NORMAL_WIDTH, BUTTON_NORMAL_HEIGHT, (WINDOW_WIDTH - BUTTON_NORMAL_WIDTH) / 2, WINDOW_HEIGHT - 300);
	Button* returnButton = CreateButton(BUTTON_RETURN, bmp_ReturnButton, BUTTON_NORMAL_WIDTH, BUTTON_NORMAL_HEIGHT, (WINDOW_WIDTH - BUTTON_NORMAL_WIDTH) / 2, WINDOW_HEIGHT - 150);

	Button* stage1Button = CreateButton(BUTTON_STAGE1, bmp_Stage1Button, BUTTON_STAGE_WIDTH, BUTTON_STAGE_HEIGHT, 80, 100);
	Button* stage2Button = CreateButton(BUTTON_STAGE2, bmp_Stage2Button, BUTTON_STAGE_WIDTH, BUTTON_STAGE_HEIGHT, 400, 100);
	Button* stage3Button = CreateButton(BUTTON_STAGE3, bmp_Stage3Button, BUTTON_STAGE_WIDTH, BUTTON_STAGE_HEIGHT, 720, 100);
	Button* stage4Button = CreateButton(BUTTON_STAGE4, bmp_Stage4Button, BUTTON_STAGE_WIDTH, BUTTON_STAGE_HEIGHT, 1040, 100);

	Button* kulaButton = CreateButton(BUTTON_KULA, bmp_KulaButton, BUTTON_ROLE_WIDTH, BUTTON_ROLE_HEIGHT, WINDOW_WIDTH / 2 - 1.5 * BUTTON_ROLE_WIDTH, 500);
	Button* kyokusanagiButton = CreateButton(BUTTON_KYOKUSANAGI, bmp_KyokusanagiButton, BUTTON_ROLE_WIDTH, BUTTON_ROLE_HEIGHT, WINDOW_WIDTH / 2 + 0.5 * BUTTON_ROLE_WIDTH, 500);

	buttons.push_back(startButton);
	buttons.push_back(helpButton);
	buttons.push_back(continueButton);
	buttons.push_back(restoreButton);
	buttons.push_back(restartButton);
	buttons.push_back(returnButton);
	buttons.push_back(stage1Button);
	buttons.push_back(stage2Button);
	buttons.push_back(stage3Button);
	buttons.push_back(stage4Button);
	buttons.push_back(kulaButton);
	buttons.push_back(kyokusanagiButton);

	//初始化背景
	bmp_Background = InitBackGround(hWnd, bmp_BG);
	bmp_Help = InitBackGround(hWnd, bmp_HELP);
	bmp_End = InitBackGround(hWnd, bmp_END);
	bmp_Choose = InitBackGround(hWnd, bmp_CHOOSE);
	bmp_Stage1 = InitBackGround(hWnd, bmp_STAGE1);
	bmp_Stage2 = InitBackGround(hWnd, bmp_STAGE2);
	bmp_Stage3 = InitBackGround(hWnd, bmp_STAGE3);
	bmp_Stage4 = InitBackGround(hWnd, bmp_STAGE4);


	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_UP:
	case VK_W:
		role->keyUpDown = true;
		break;
	case VK_DOWN:
	case VK_S:
		role->keyDownDown = true;
		break;
	case VK_LEFT:
	case VK_A:
		if (!role->keyDoubleLeftDown && GetTickCount64() - keyLeftTime < 200) {//判断双击左键
			role->keyDoubleLeftDown = true;
		}
		else {
			role->keyLeftDown = true;
		}
		break;
	case VK_RIGHT:
	case VK_D:
		if (!role->keyDoubleRightDown && GetTickCount64() - keyRightTime < 200) {//判断双击右键
			role->keyDoubleRightDown = true;
		}
		else {
			role->keyRightDown = true;
		}
		break;
	case VK_J:
		role->keyPunchDown = true;
		break;
	case VK_K:
		role->keyKickDown = true;
		break;
	case VK_U:
		role->key_H_PunchDown = true;
		break;
	case VK_I:
		role->key_H_KickDown = true;
		break;
	case VK_L:
		role->keyDefendDown = true;
		break;
	case VK_O:
		role->keyUltimateDown = true;
		break;
	case VK_H:
		role->keyDodgeDown = true;
		break;
	case VK_ESCAPE:
		Pause(hWnd);
		break;
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_UP:
	case VK_W:
		role->keyUpDown = false;
		break;
	case VK_DOWN:
	case VK_S:
		role->keyDownDown = false;
		break;
	case VK_LEFT:
	case VK_A:
		keyLeftTime = GetTickCount64();
		role->keyDoubleLeftDown = role->keyLeftDown = false;
		break;
	case VK_RIGHT:
	case VK_D:
		keyRightTime = GetTickCount64();
		role->keyDoubleRightDown = role->keyRightDown = false;
		break;
	case VK_J:
		role->keyPunchDown = false;
		break;
	case VK_K:
		role->keyKickDown = false;
		break;
	case VK_U:
		role->key_H_PunchDown = false;
		break;
	case VK_I:
		role->key_H_KickDown = false;
		break;
	case VK_L:
		role->keyDefendDown = false;
		break;
	case VK_O:
		role->keyUltimateDown = false;
		break;
	case VK_H:
		role->keyDodgeDown = false;
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->visible) {
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY) {
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
				{
					InitStage(hWnd, STAGE_CHOOSE);
				}
				break;
				case BUTTON_HELP:
				{
					InitStage(hWnd, STAGE_HELP);
				}
				break;
				case BUTTON_RESTORE:
				{
					Restore(hWnd);
				}
				break;
				case BUTTON_RESTART:
				{
					Restart(hWnd);
				}
				break;
				case BUTTON_RETURN:
				{
					InitStage(hWnd, STAGE_STARTMENU);
				}
				break;
				case BUTTON_CONTINUE:
				{
					gameover = 0;

					switch (chooseStage) {
					case 1:
						InitStage(hWnd, STAGE_1);
						break;
					case 2:
						InitStage(hWnd, STAGE_2);
						break;
					case 3:
						InitStage(hWnd, STAGE_3);
						break;
					case 4:
						InitStage(hWnd, STAGE_4);
						break;
					default:
						break;
					}
				}
				break;
				case BUTTON_STAGE1:
				{
					chooseStage = 1;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
				case BUTTON_STAGE2:
				{
					chooseStage = 2;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
				case BUTTON_STAGE3:
				{
					chooseStage = 3;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
				case BUTTON_STAGE4:
				{
					chooseStage = 4;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
				case BUTTON_KULA:
				{
					chooseRole = HERO_KULA;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
				case BUTTON_KYOKUSANAGI:
				{
					chooseRole = HERO_KYOKUSANAGI;
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
				}
			}
		}
	}

}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	UpdateUnits(hWnd);

	//倒计时变化
	currentStage->timeCountDown--;

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion


#pragma region 其它游戏状态处理函数

//TODO: 添加游戏需要的更多函数

//游戏暂停
void Pause(HWND hWnd) {
	if (currentStage->stageID<STAGE_1 || currentStage->stageID>STAGE_4) {//不在游戏中，不暂停
		return;
	}
	currentStage->timerOn = false;

	//显示暂停界面的按钮
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->buttonID == BUTTON_RESTORE || button->buttonID == BUTTON_RESTART || button->buttonID == BUTTON_RETURN) {
			button->visible = button->visible = true;
		}
		else {
			button->visible = false;
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

//恢复游戏
void Restore(HWND hWnd) {
	currentStage->timerOn = true;

	//隐藏暂停界面的按钮
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		button->visible = button->visible = false;
	}

	InvalidateRect(hWnd, NULL, FALSE);

}

//重新开始本关卡
void Restart(HWND hWnd) {
	InitStage(hWnd, currentStage->stageID);
}

//获取英雄身体碰撞盒
Rect* GetBodyRect(Hero* hero) {

	Rect* rect = new Rect;
	int state = hero->state, x = hero->x, y = hero->y, width_0 = hero->frame_width[HERO_STATE_HOLD], width = hero->frame_width[state], height = hero->frame_height[state], biasx = hero->frame_biasx[state];

	if (hero->inverse) {
		rect->x1 = x + 0.5 * width_0 - width - biasx;
		rect->x2 = x + 2.5 * width_0;
	}
	else {
		rect->x1 = x - 0.5 * width_0;
		rect->x2 = x + 1.5 * width_0 + width + biasx;
	}

	if (state >= HERO_STATE_SQUAT_PUNCH && state <= HERO_STATE_SQUAT_H_KICK) {
		rect->y1 = y + 1 * height;
		rect->y2 = y + 2.5 * height;
	}
	else {
		rect->y1 = y - 0.5 * height;
		rect->y2 = y + 2.5 * height;
	}

	return rect;
}

//获取英雄攻击范围碰撞盒
Rect* GetAttackRect(Hero* hero) {

	Rect* rect = new Rect;
	int state = hero->state, x = hero->x, y = hero->y, width_0 = hero->frame_width[HERO_STATE_HOLD], width = hero->frame_width[state], height = hero->frame_height[state], biasx = hero->frame_biasx[state];

	if (hero->inverse) {
		rect->x1 = x + 2 * width_0 - 2.5 * width - biasx;
		rect->x2 = x - 0.5 * width_0;
	}
	else {
		rect->x1 = x + 2.5 * width_0;
		rect->x2 = x + 2.5 * width + biasx;
	}

	if (state >= HERO_STATE_SQUAT_PUNCH && state <= HERO_STATE_SQUAT_H_KICK) {
		rect->y1 = y + 1.5 * height;
		rect->y2 = y + 2.5 * height;
	}
	else {
		rect->y1 = y - 0.5 * height;
		rect->y2 = y + 1.5 * height;
	}

	return rect;
}

//获取道具碰撞盒
Rect* GetPropRect(Prop* prop) {

	Rect* rect = new Rect;
	int  x = prop->x, y = prop->y, width = prop->width, height = prop->height;

	rect->x1 = x;
	rect->x2 = x + width;
	rect->y1 = y;
	rect->y2 = y + height;

	return rect;
}

//碰撞检测
bool Detection(Rect* rect1, Rect* rect2) {

	bool detect = !(rect1->x1 > rect2->x2 || rect1->x2<rect2->x1 || rect1->y1>rect2->y2 || rect1->y2 < rect2->y1);
	delete rect1;	//释放内存
	delete rect2;
	return detect;
}

//攻击
void HeroAttack(Hero* src) {
	Hero* dest = src == role ? enemy : role;//选择被攻击者

	if (Detection(GetAttackRect(src), GetBodyRect(dest))) {//攻击者的攻击范围和被攻击者的身体相交

		int state[2] = { src->state,dest->state }, harm, distance;

		if (dest->rage < RAGE_MAX) {//被攻击者怒气值增加
			dest->rage++;
		}

		if (dest->frame_column == dest->frame_mid_length[state[1]] &&
			(state[1] == HERO_STATE_SQUAT_DEFEND
				|| (state[1] == HERO_STATE_DEFEND && !(state[0] >= HERO_STATE_SQUAT_PUNCH && state[0] <= HERO_STATE_SQUAT_H_KICK)))) {//被攻击者处于防御状态，受伤减少
			harm = src->atk - dest->def;
		}
		else {//被攻击者进入受伤状态
			harm = src->atk;

			if (state[0] == HERO_STATE_ULTIMATE) {//大招造成重伤
				dest->h_injured = true;
			}
			else {
				dest->injured = true;
			}
		}

		if (state[0] == HERO_STATE_ULTIMATE) {
			distance = 100;//击退的距离
		}
		else if (state[0] == HERO_STATE_H_PUNCH || state[0] == HERO_STATE_H_KICK || state[0] == HERO_STATE_SQUAT_H_PUNCH || state[0] == HERO_STATE_SQUAT_H_KICK) {//重击伤害加倍
			harm *= 2;
			distance = 50;
		}
		else {
			distance = 20;
		}

		//击退效果
		if (src->inverse) {
			dest->x -= distance;
			if (dest->x < 0.5 * dest->frame_width[state[0]]) {// 若超过背景左边界，则回到边界
				dest->x = 0.5 * dest->frame_width[state[0]];
			}
		}
		else {
			dest->x += distance;
			if (dest->x + dest->frame_width[state[0]] * 2.5 > WINDOW_WIDTH) {//若超过背景右边界，则回到边界
				dest->x = WINDOW_WIDTH - 2.5 * dest->frame_width[state[0]];
			}
		}

		//被攻击者血量减少
		dest->health -= harm;

		if (dest->health <= 0) {//被攻击者死亡
			if (dest->life > 0) {//被攻击者满血复活
				dest->life--;
				dest->health = HEALTH_MAX;
			}
			else {//被攻击者死亡，游戏结束
				gameover = 1;
				success = (dest == role) ? 0 : 1;//玩家死亡，游戏失败；敌人死亡，游戏胜利
			}
		}
	}
}

//判断英雄是否接触道具
bool TouchProp(Hero* hero, Prop* prop) {

	if (Detection(GetBodyRect(hero), GetPropRect(prop))) {//英雄的身体和道具相交

		switch (prop->propID) {
		case PROP_ATKDEF:
			hero->atk += 2;
			hero->def += 1;
			break;
		case PROP_HEALTH:
			hero->health += 20;
			if (hero->health > HEALTH_MAX) {//血量不能超出上限
				hero->health = HEALTH_MAX;
			}
			break;
		case PROP_LIFE:
			hero->life += 1;
		default:
			break;
		}

		return true;
	}

	return false;
}

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y) {
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

// 添加英雄函数
Hero* CreateHero(int type, int x, int y) {
	Hero* hero = new Hero();
	hero->type = type;
	hero->frame_length = new int[TOTAL_TYPE];
	hero->frame_width = new int[TOTAL_TYPE];
	hero->frame_height = new int[TOTAL_TYPE];
	hero->frame_total_height = new int[TOTAL_TYPE];
	hero->frame_mid_length = new int[TOTAL_TYPE];
	hero->frame_biasx = new int[TOTAL_TYPE];
	hero->frame_biasy = new int[TOTAL_TYPE];

	if (type == HERO_KULA) {
		/* lengths: 每种动作有多少帧【不包含反转帧】
		   widths: 每种动作中的帧宽度
		   heights: 每种动作中的帧高度
		   biasx: 各动作之间为切换协调合理做出的适当的左右偏移
		   biasy: 各动作之间为切换协调合理做出的适当的上下偏移
		*/
		int lengths[TOTAL_TYPE] = { 54, 10, 13, 11, 43, 18, 10, 10, 15, 11, 8, 7, 11, 11, 14, 14, 8, 8 },
			widths[TOTAL_TYPE] = { 50, 57, 86, 80, 318, 127, 93, 119, 154, 126, 73, 71, 136, 136, 119, 144, 88, 90 },
			heights[TOTAL_TYPE] = { 107, 107, 208, 107, 208, 170, 107, 107, 107, 115, 117, 107, 67, 67, 114, 65, 76, 66 },
			biasx[TOTAL_TYPE] = { 0, 0, -48, -75, -151, -52, 22, 35, 52, -82, -30, -53, -45, -45, -55,-43, -71, -112 },
			biasy[TOTAL_TYPE] = { 0, 0, -250, 0, -163, -101, 0, 0, 0, -20, -18, 0, 106, 106, -21, 105, 111, 103 },
			mid_lengths[TOTAL_TYPE] = { 0, 0, 0, 7, 31, 0, 6, 6, 8, 6, 3, 0, 3, 3, 5, 3, 3, 0 };

		hero->img = bmp_Unit_kula;
		hero->frame_v = KULA_FRAME_V;
		hero->atk = KULA_ATK;
		hero->def = KULA_DEF;
		hero->vx = KULA_VX;
		hero->vy = KULA_VY;

		for (int j = 0; j < TOTAL_TYPE; j++) {
			hero->frame_length[j] = lengths[j];
			hero->frame_width[j] = widths[j];
			hero->frame_height[j] = heights[j];
			if (j == 0) hero->frame_total_height[j] = 0;
			else hero->frame_total_height[j] = heights[j - 1] + hero->frame_total_height[j - 1];
			hero->frame_biasx[j] = biasx[j];
			hero->frame_biasy[j] = biasy[j];
			hero->frame_mid_length[j] = mid_lengths[j];
		}
	}
	else if (type == HERO_KYOKUSANAGI) {
		int lengths[TOTAL_TYPE] = { 10,	6,13,4,19,12,4,6,5,9,4,4,4,12,8,13,4,4 },
			widths[TOTAL_TYPE] = { 64,63,87,67,121,150,89,134,82,137,72,83,90,169,92,138,67,62 },
			heights[TOTAL_TYPE] = { 106,118,183,107,196,123,105,113,126,131,106,109,74,76,145,87,107,78 },
			biasx[TOTAL_TYPE] = { 0,3,-19,-8,-95,-71,12,-10,9,0,4,-51,3,44,-17,-12,-8,-22 },
			biasy[TOTAL_TYPE] = { 0,-33,-196,0,-222,8,0,-24,-53,-60,0,-8,80,82,-95,59,0,85 },
			mid_lengths[TOTAL_TYPE] = { 0,0,0,2,11,0,2,2,2,5,1,0,2,6,5,5,2,0 };

		hero->img = bmp_Unit_kyokusanagi;
		hero->frame_v = KYOKUSANAGI_FRAME_V;
		hero->atk = KYOKUSANAGI_ATK;
		hero->def = KYOKUSANAGI_DEF;
		hero->vx = KYOKUSANAGI_VX;
		hero->vy = KYOKUSANAGI_VY;

		for (int j = 0; j < TOTAL_TYPE; j++) {
			hero->frame_length[j] = lengths[j];
			hero->frame_width[j] = widths[j];
			hero->frame_height[j] = heights[j];
			if (j == 0) hero->frame_total_height[j] = 0;
			else hero->frame_total_height[j] = heights[j - 1] + hero->frame_total_height[j - 1];
			hero->frame_biasx[j] = biasx[j];
			hero->frame_biasy[j] = biasy[j];
			hero->frame_mid_length[j] = mid_lengths[j];
		}
	}

	hero->state = HERO_STATE_HOLD;
	hero->health = HEALTH_MAX;
	hero->frame_column = 0;
	hero->rage = 0;
	hero->life = 0;
	hero->x = x;
	hero->y = y;
	return hero;
}

// 添加道具函数
Prop* CreateProp(int propID, int x, int y) {
	Prop* prop = new Prop();
	prop->width = PROP_WIDTH;
	prop->height = PROP_HEIGHT;

	prop->propID = propID;
	prop->x = x;
	prop->y = y;

	switch (propID) {
	case PROP_ATKDEF:
		prop->img = bmp_Drug1;
		break;
	case PROP_HEALTH:
		prop->img = bmp_Drug2;
		break;
	case PROP_LIFE:
		prop->img = bmp_Drug3;
		break;
	default:
		break;
	}

	return prop;
}

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID) {
	//重置英雄、道具
	heros.clear();
	props.clear();

	// 初始化场景实例
	if (currentStage) delete currentStage;//暂停界面不需要重置场景
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (stageID == STAGE_STARTMENU) {//标题界面
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//显示按钮
		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_STARTGAME || button->buttonID == BUTTON_HELP) {
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}
	}
	else if (stageID == STAGE_CHOOSE) {//选角界面
		currentStage->bg = bmp_Choose;

		chooseStage = 1;
		chooseRole = HERO_KULA;

		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_CONTINUE || button->buttonID == BUTTON_STAGE1 || button->buttonID == BUTTON_STAGE2 || button->buttonID == BUTTON_STAGE3 || button->buttonID == BUTTON_STAGE4 || button->buttonID == BUTTON_KULA || button->buttonID == BUTTON_KYOKUSANAGI) {
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}
	}
	else if (stageID == STAGE_HELP) {//帮助界面
		currentStage->bg = bmp_Help;

		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_RETURN) {
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}
	}
	else if (stageID == STAGE_END) {//结束界面
		currentStage->bg = bmp_End;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_RETURN) {
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}
	}
	else if (stageID >= STAGE_1 && stageID <= STAGE_4) {
		currentStage->timeCountDown = 4999;
		currentStage->timerOn = true;

		//显示按钮
		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (false) {
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}

		//初始化英雄
		int enemyRole = chooseRole == HERO_KULA ? HERO_KYOKUSANAGI : HERO_KULA;

		role = CreateHero(chooseRole, 300, BASE_Y);
		enemy = CreateHero(enemyRole, 1000, BASE_Y);

		heros.push_back(enemy);
		heros.push_back(role);

		// 按场景初始化单位
		switch (stageID) {
		case STAGE_1:
			currentStage->bg = bmp_Stage1;
			break;
		case STAGE_2:
			currentStage->bg = bmp_Stage2;
			break;
		case STAGE_3:
			currentStage->bg = bmp_Stage3;
			break;
		case STAGE_4:
			currentStage->bg = bmp_Stage4;
			break;
		default:
			break;
		}
	}
	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 刷新单位状态函数
void UpdateUnits(HWND hWnd) {

	if (currentStage->timeCountDown == 0) {//时间截止，游戏结束
		success = (role->life > enemy->life || (role->life == enemy->life && role->health >= enemy->health)) ? 1 : 0;
		gameover = 1;
	}

	if (gameover) {//游戏结束
		InitStage(hWnd, STAGE_END);
	}

	if (currentStage->timeCountDown % 500 == 0) {//每隔一定时间生成一个道具

		int propID;

		switch (rand() % 5) {
		case 0:case 1:
			propID = PROP_ATKDEF;
			break;
		case 2: case 3:
			propID = PROP_HEALTH;
			break;
		case 4:
			propID = PROP_LIFE;
			break;
		default:
			break;
		}

		props.push_back(CreateProp(propID, rand() % (WINDOW_WIDTH - PROP_WIDTH), 650 - PROP_HEIGHT));

	}

	for (int i = 0; i < heros.size(); i++) {//英雄行动
		Hero* hero = heros[i];
		HeroBehaviour(hero);
		if (hero == enemy) {//AI自动行动
			EnemyOperate(hero);
		}
	}

	for (vector<Prop*>::iterator itor = props.begin(); itor != props.end();) {//检测道具是否被拾取
		Prop* prop = *itor;
		int i = 0;
		for (; i < heros.size(); i++) {
			Hero* hero = heros[i];
			if (TouchProp(hero, prop)) {//有英雄接触到道具
				break;
			}
		}
		if (i == heros.size()) {//没有英雄接触到道具
			itor++;
		}
		else {
			itor = props.erase(itor);
		}
	}
}

//AI自动行动
void EnemyOperate(Hero* hero) {
	int state = hero->state, inverse = hero->inverse;

	//先重置
	hero->keyUpDown = false;
	hero->keyDownDown = false;
	hero->keyLeftDown = false;
	hero->keyRightDown = false;
	hero->keyDoubleLeftDown = false;
	hero->keyDoubleRightDown = false;
	hero->keyPunchDown = false;
	hero->keyKickDown = false;
	hero->key_H_PunchDown = false;
	hero->key_H_KickDown = false;
	hero->keyDefendDown = false;
	hero->keyUltimateDown = false;
	hero->keyDodgeDown = false;

	int distance = hero->x - role->x; // 英雄之间的距离

	if (distance > 600) {
		hero->keyDoubleLeftDown = true;
		hero->keyDodgeDown = true;
	}
	else if (distance < -600) {
		hero->keyDoubleRightDown = true;
		hero->keyDodgeDown = true;
	}
	else if (distance > 200 || (!inverse && distance > 0)) {
		hero->keyLeftDown = true;
	}
	else if (distance < -200 || (inverse && distance < 0)) {
		hero->keyRightDown = true;
	}
	else if (GetTickCount64() < keyDefendTime + 500) {//防御持续1s
		hero->keyDefendDown = true;
	}
	else {
		hero->keyUltimateDown = true;

		switch (rand() % 9) {
		case 0:case 1:
			hero->keyPunchDown = true;
			break;
		case 2:case 3:
			hero->keyKickDown = true;
			break;
		case 4:case 5:
			hero->key_H_PunchDown = true;
			break;
		case 6:case 7:
			hero->key_H_PunchDown = true;
			break;
		case 8:
			hero->keyDefendDown = true;
			keyDefendTime = GetTickCount64();
			break;
		default:
			break;
		}
	}
}


//英雄行为函数
void HeroBehaviour(Hero* hero) {

	int state = hero->state, inverse = hero->inverse, vx = hero->vx;//记录旧的状态

	//判断当前状态, 以及判断是否需要状态变化

	if (hero->frame_column >= hero->frame_length[state] - 1) {//帧序列结束

		if (state >= HERO_STATE_SQUAT_PUNCH && state <= HERO_STATE_SQUAT_HURT) {//下蹲类的动作结束，恢复到下蹲状态
			hero->state = HERO_STATE_SQUAT;
			hero->frame_column = hero->frame_mid_length[HERO_STATE_SQUAT];
		}
		else {//恢复到站立状态
			hero->state = HERO_STATE_HOLD;
			hero->frame_column = 0;
		}
	}
	else {//帧序列未结束
		if (!(((state == HERO_STATE_SQUAT && hero->keyDownDown)
			|| (state == HERO_STATE_DEFEND && hero->keyDefendDown)
			|| (state == HERO_STATE_SQUAT_DEFEND && hero->keyDefendDown))
			&& (hero->frame_column == hero->frame_mid_length[state]))) {//下蹲或防御到一半时停止
			hero->frame_column += hero->frame_v;
		}
	}

	//当前在攻击
	if ((state >= HERO_STATE_PUNCH && state <= HERO_STATE_H_KICK) || (state >= HERO_STATE_SQUAT_PUNCH && state <= HERO_STATE_SQUAT_H_KICK)) {
		if (abs(hero->frame_column - hero->frame_mid_length[state]) < 0.000001) {//运动到中间帧时攻击判定
			HeroAttack(hero);
		}
	}

	//当前在大招
	if (state == HERO_STATE_ULTIMATE) {
		if ((int)(hero->frame_column / hero->frame_v) % 5 == 0) {//每隔5帧攻击判定一次
			HeroAttack(hero);
		}
	}

	//当前静止或者走路
	if (state == HERO_STATE_HOLD || state == HERO_STATE_WALK) {

		hero->state = HERO_STATE_HOLD;//没有其他操作，则进入静止状态

		if (hero->keyRightDown || hero->keyLeftDown || hero->keyDoubleLeftDown || hero->keyDoubleRightDown) {//按下左、右键，进入走路状态
			hero->state = HERO_STATE_WALK;

			if (hero->keyDoubleLeftDown || hero->keyDoubleRightDown) {//双击左、右键，速度加快
				vx *= 3;
			}

			if (hero->keyRightDown || hero->keyDoubleRightDown) {
				hero->inverse = 0;
				hero->x += vx;
				if (hero->x + hero->frame_width[state] * 2.5 > WINDOW_WIDTH) {//若超过背景右边界，则回到边界
					hero->x = WINDOW_WIDTH - 2.5 * hero->frame_width[state];
				}
			}
			else {
				hero->inverse = 1;
				hero->x -= vx;
				if (hero->x < 0.5 * hero->frame_width[state]) {// 若超过背景左边界，则回到边界
					hero->x = 0.5 * hero->frame_width[state];
				}
			}
		}

		if (hero->keyUpDown) {//跳跃
			hero->state = HERO_STATE_JUMP;
		}

		if (hero->keyDownDown) {//下蹲
			hero->state = HERO_STATE_SQUAT;
		}

		if (hero->keyPunchDown) {//轻拳
			hero->state = HERO_STATE_PUNCH;
		}

		if (hero->keyKickDown) {//轻踢
			hero->state = HERO_STATE_KICK;
		}

		if (hero->key_H_PunchDown) {//重拳
			hero->state = HERO_STATE_H_PUNCH;
		}

		if (hero->key_H_KickDown) {//重踢
			hero->state = HERO_STATE_H_KICK;
		}

		if (hero->keyDefendDown) {//防御
			hero->state = HERO_STATE_DEFEND;
		}

		if (hero->keyDodgeDown && GetTickCount64() - keyDodgeTime >= 2000) {//闪避,每次间隔2s
			keyDodgeTime = GetTickCount64();
			if (inverse) {
				hero->x -= 400;
				if (hero->x < 0.5 * hero->frame_width[state]) {// 若超过背景左边界，则回到边界
					hero->x = 0.5 * hero->frame_width[state];
				}
			}
			else {
				hero->x += 400;
				if (hero->x + hero->frame_width[state] * 2.5 > WINDOW_WIDTH) {//若超过背景右边界，则回到边界
					hero->x = WINDOW_WIDTH - 2.5 * hero->frame_width[state];
				}
			}
		}

		if (hero->keyUltimateDown) {//大招
			if (hero->rage == RAGE_MAX) {
				hero->rage = 0;
				hero->state = HERO_STATE_ULTIMATE;
			}
		}

	}

	//当前在下蹲
	if (state == HERO_STATE_SQUAT) {

		if (hero->keyPunchDown) {//轻拳
			hero->state = HERO_STATE_SQUAT_PUNCH;
		}

		if (hero->keyKickDown) {//轻踢
			hero->state = HERO_STATE_SQUAT_KICK;
		}

		if (hero->key_H_PunchDown) {//重拳
			hero->state = HERO_STATE_SQUAT_H_PUNCH;
		}

		if (hero->key_H_KickDown) {//重踢
			hero->state = HERO_STATE_SQUAT_H_KICK;
		}

		if (hero->keyDefendDown) {//防御
			hero->state = HERO_STATE_SQUAT_DEFEND;
		}

	}

	//当前在跳跃
	if (state == HERO_STATE_JUMP) {
		if (hero->frame_column == 0) {//跳跃结束或者被打断，恢复初始纵坐标
			hero->y = BASE_Y;
		}
		else if (hero->frame_column > hero->frame_length[state] / 2.0) {
			hero->y += hero->vy;
		}
		else if (hero->frame_column <= hero->frame_length[state] / 2.0) {
			hero->y -= hero->vy;
		}
	}

	//受伤
	if (hero->injured) {

		if (hero->state == HERO_STATE_SQUAT || state >= HERO_STATE_SQUAT_PUNCH && state <= HERO_STATE_SQUAT_HURT) {
			hero->state = HERO_STATE_SQUAT_HURT;
		}
		else {
			hero->state = HERO_STATE_HURT;
		}
		hero->y = BASE_Y;	//英雄纵坐标重置
		hero->injured = 0;
	}

	//受重伤
	if (hero->h_injured) {
		hero->state = HERO_STATE_H_HURT;
		hero->h_injured = 0;
	}

	if (hero->state != state) {//动作发生改变
		if (!((state >= HERO_STATE_SQUAT_PUNCH && state <= HERO_STATE_SQUAT_HURT) && hero->state == HERO_STATE_SQUAT)) {//下蹲类动作恢复到下蹲，另外考虑
			hero->frame_column = 0;
		}
	}

}
#pragma endregion


#pragma region 绘图函数
// 绘图函数
void Paint(HWND hWnd) {

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	HBRUSH redBrush = CreateSolidBrush(RGB(255, 51, 0));
	HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH yellowBrush = CreateSolidBrush(RGB(247, 186, 11));

	HFONT smallFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("consolas"));
	HFONT midFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("consolas"));
	HFONT largeFont = CreateFont(200, 0, 0, 0, 1800, 1, 1, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("consolas"));

	CString str;

	//初始化缓存
	HBITMAP blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, currentStage->bg);

	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	SetBkMode(hdc_memBuffer, TRANSPARENT);

	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_CHOOSE) {//在选角界面中

		SelectObject(hdc_memBuffer, midFont);
		SetTextColor(hdc_memBuffer, RGB(255, 255, 255));
		SetTextAlign(hdc_memBuffer, TA_LEFT);


		TextOut(hdc_memBuffer, WINDOW_WIDTH / 2 - 100, 30, TEXT("STAGE:"), lstrlen(TEXT("STAGE:")));
		TextOut(hdc_memBuffer, WINDOW_WIDTH / 2 - 100, 450, TEXT("ROLE:"), lstrlen(TEXT("ROLE:")));

		str.Format(_T("%d"), chooseStage);
		TextOut(hdc_memBuffer, WINDOW_WIDTH / 2 + 50, 30, str, str.GetLength());

		switch (chooseRole) {
		case HERO_KULA:
			str = "Kula";
			break;
		case HERO_KYOKUSANAGI:
			str = "Kyo Kusanagi";
			break;
		default:
			break;
		}
		TextOut(hdc_memBuffer, WINDOW_WIDTH / 2 + 50, 450, str, str.GetLength());

	}
	else if (currentStage->stageID == STAGE_END) {//在结束界面中

		SelectObject(hdc_memBuffer, largeFont);
		SetTextColor(hdc_memBuffer, RGB(255, 76, 0));
		SetTextAlign(hdc_memBuffer, TA_CENTER);

		if (success) {
			TextOut(hdc_memBuffer, (WINDOW_WIDTH - lstrlen(TEXT("SUCCESS"))) / 2, 55, TEXT("SUCCESS"), lstrlen(TEXT("SUCCESS")));
		}
		else {
			TextOut(hdc_memBuffer, (WINDOW_WIDTH - lstrlen(TEXT("FALSE"))) / 2, 55, TEXT("FALSE"), lstrlen(TEXT("FALSE")));
		}
	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_4) {//在游戏主界面中
		// 绘制英雄到缓存
		int column, state;

		for (int i = 0; i < heros.size(); i++) {
			Hero* hero = heros[i];
			state = hero->state;
			int width = hero->frame_width[state], height = hero->frame_height[state], total_height = hero->frame_total_height[state], biasx = hero->frame_biasx[state], biasy = hero->frame_biasy[state];

			if (hero->inverse) {
				column = width * int(hero->frame_length[state] + hero->frame_column);
				biasx = 2 * hero->frame_width[0] - 2 * width - biasx;
			}
			else column = width * int(hero->frame_column);

			SelectObject(hdc_loadBmp, hero->img);
			TransparentBlt(
				hdc_memBuffer, hero->x - 0.5 * width + biasx, hero->y - 0.5 * height + biasy, // HERO POSITION IN THE MAP
				width * 3, height * 3,   // HERO SIZE
				hdc_loadBmp, column, total_height, width, height, // HERO POSITION IN THE BITMAP
				RGB(255, 255, 255)
			);

			//显示倒计时
			SelectObject(hdc_memBuffer, midFont);
			SetTextColor(hdc_memBuffer, RGB(255, 170, 0));
			SetTextAlign(hdc_memBuffer, TA_CENTER);

			CString str;
			str.Format(_T("%d"), currentStage->timeCountDown);
			TextOut(hdc_memBuffer, (WINDOW_WIDTH - str.GetLength()) / 2, 55, str, str.GetLength());


			//绘制血条，怒气条，角色属性
			SelectObject(hdc_memBuffer, smallFont);
			SetTextColor(hdc_memBuffer, RGB(255, 102, 255));

			if (hero == role) {

				SelectObject(hdc_memBuffer, nullBrush);
				Rectangle(hdc_memBuffer, 100, 30, 100 + HEALTH_MAX * 5, 50);
				Rectangle(hdc_memBuffer, 100, 55, 100 + RAGE_MAX * 20, 75);

				SelectObject(hdc_memBuffer, redBrush);
				Rectangle(hdc_memBuffer, 100, 31, 100 + hero->health * 5, 49);

				SelectObject(hdc_memBuffer, yellowBrush);
				Rectangle(hdc_memBuffer, 100, 56, 100 + hero->rage * 20, 74);

				SetTextAlign(hdc_memBuffer, TA_LEFT);

				str.Format(_T("%d"), hero->atk);
				TextOut(hdc_memBuffer, 0, 100, TEXT("ATK:"), lstrlen(TEXT("ATK:")));
				TextOut(hdc_memBuffer, 75, 100, str, str.GetLength());

				str.Format(_T("%d"), hero->def);
				TextOut(hdc_memBuffer, 0, 150, TEXT("DEF:"), lstrlen(TEXT("DEF:")));
				TextOut(hdc_memBuffer, 75, 150, str, str.GetLength());

				str.Format(_T("%d"), hero->vx);
				TextOut(hdc_memBuffer, 0, 200, TEXT("SPD:"), lstrlen(TEXT("SPD:")));
				TextOut(hdc_memBuffer, 75, 200, str, str.GetLength());

				str.Format(_T("%d"), hero->life);
				TextOut(hdc_memBuffer, 0, 250, TEXT("LIFE:"), lstrlen(TEXT("LIFE:")));
				TextOut(hdc_memBuffer, 75, 250, str, str.GetLength());

			}
			else {

				SelectObject(hdc_memBuffer, nullBrush);
				Rectangle(hdc_memBuffer, WINDOW_WIDTH - 100 - HEALTH_MAX * 5, 30, WINDOW_WIDTH - 100, 50);
				Rectangle(hdc_memBuffer, WINDOW_WIDTH - 100 - RAGE_MAX * 20, 55, WINDOW_WIDTH - 100, 75);

				SelectObject(hdc_memBuffer, redBrush);
				Rectangle(hdc_memBuffer, WINDOW_WIDTH - 100 - HEALTH_MAX * 5, 31, WINDOW_WIDTH - 100 - HEALTH_MAX * 5 + hero->health * 5, 49);

				SelectObject(hdc_memBuffer, yellowBrush);
				Rectangle(hdc_memBuffer, WINDOW_WIDTH - 100 - RAGE_MAX * 20, 56, WINDOW_WIDTH - 100 - RAGE_MAX * 20 + hero->rage * 20, 74);

				SetTextAlign(hdc_memBuffer, TA_RIGHT);

				str.Format(_T("%d"), hero->atk);
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 75, 100, TEXT("ATK:"), lstrlen(TEXT("ATK:")));
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 30, 100, str, str.GetLength());

				str.Format(_T("%d"), hero->def);
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 75, 150, TEXT("DEF:"), lstrlen(TEXT("DEF:")));
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 30, 150, str, str.GetLength());

				str.Format(_T("%d"), hero->vx);
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 75, 200, TEXT("SPD:"), lstrlen(TEXT("SPD:")));
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 30, 200, str, str.GetLength());

				str.Format(_T("%d"), hero->life);
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 75, 250, TEXT("LIFE:"), lstrlen(TEXT("LIFE:")));
				TextOut(hdc_memBuffer, WINDOW_WIDTH - 30, 250, str, str.GetLength());
			}

		}

		for (int i = 0; i < props.size(); i++) {//绘制道具
			Prop* prop = props[i];
			SelectObject(hdc_loadBmp, prop->img);
			TransparentBlt(
				hdc_memBuffer, prop->x, prop->y,
				prop->width, prop->height,
				hdc_loadBmp, 0, 0, prop->width, prop->height,
				RGB(255, 255, 255)
			);
		}
	}

	// 绘制按钮到缓存
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->visible) {
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteObject(nullBrush);
	DeleteObject(redBrush);
	DeleteObject(yellowBrush);
	DeleteObject(midFont);

	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}


// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src) {

	srand(time(NULL));
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);


	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);



	//初始化缓存
	HBITMAP bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	//加载资源
	SelectObject(hdc_loadBmp, bmp_src);

	TransparentBlt(
		hdc_memBuffer, 0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		hdc_loadBmp, 0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		RGB(255, 255, 255)
	);

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}

#pragma endregion