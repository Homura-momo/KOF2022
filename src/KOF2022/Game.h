#pragma once

#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")			//图象处理的函数接口，例如：透明色的位图的绘制TransparentBlt函数


// TODO:  在此处引用程序需要的其他头文件
#include <vector>
#include <math.h>

#include <atlstr.h>

#pragma endregion


#pragma region 宏定义

#define WINDOW_TITLEBARHEIGHT	32			//标题栏高度
#define WINDOW_WIDTH			1360		//游戏窗口宽度
#define WINDOW_HEIGHT			768		//游戏窗口高度


#define STAGE_STARTMENU		0		//开始画面的ID
#define STAGE_HELP			1		//帮助画面的ID
#define STAGE_END			2		//结束画面的ID
#define STAGE_CHOOSE		3		//选角画面的ID

#define STAGE_1					10		//第一个游戏场景的ID
#define STAGE_2					11	
#define STAGE_3					12	
#define STAGE_4					13	


#define UNIT_SIZE_X				64		//单位的宽度
#define UNIT_SIZE_Y				64		//单位的高度
#define UNIT_LAST_FRAME			11		//单位的动画帧最大值

//单位阵营定义
#define UNIT_SIDE_RED			10000	//红方
#define UNIT_SIDE_BLUE			10001	//蓝方


//单位状态定义
#define UNIT_STATE_HOLD			0		//静止
#define UNIT_STATE_WALK			1		//行走
#define UNIT_STATE_ATTACK		2		//攻击

//单位方向定义
#define UNIT_DIRECT_RIGHT		0		//向右
#define UNIT_DIRECT_LEFT		1		//向左

//英雄类型定义
#define HERO_KULA               100   //库拉
#define HERO_KYOKUSANAGI        101   //草稚京

//英雄状态定义
#define HERO_STATE_HOLD				0		//静止
#define HERO_STATE_WALK				1		//行走
#define HERO_STATE_JUMP				2		//跳跃
#define HERO_STATE_SQUAT			3		//下蹲
#define HERO_STATE_ULTIMATE			4		//大招
#define HERO_STATE_H_HURT			5		//重伤
#define HERO_STATE_PUNCH			6		//站轻拳
#define HERO_STATE_KICK				7		//站轻踢
#define HERO_STATE_H_PUNCH			8		//站重拳
#define HERO_STATE_H_KICK			9		//站重踢
#define HERO_STATE_DEFEND			10		//站防御
#define HERO_STATE_HURT				11		//站受伤
#define HERO_STATE_SQUAT_PUNCH		12		//蹲轻拳
#define HERO_STATE_SQUAT_KICK		13		//蹲轻踢
#define HERO_STATE_SQUAT_H_PUNCH	14		//蹲重拳
#define HERO_STATE_SQUAT_H_KICK		15		//蹲重踢
#define HERO_STATE_SQUAT_DEFEND		16		//蹲防御
#define HERO_STATE_SQUAT_HURT		17		//蹲受伤


//键盘字母值定义
#define VK_A                    65
#define VK_W                    87
#define VK_S                    83
#define VK_D                    68
#define VK_E                    69
#define VK_U                    85
#define VK_I                    73
#define VK_J                    74
#define VK_K                    75
#define VK_L                    76
#define VK_O                    79
#define VK_H                    72


//其它定义

//按钮ID
#define BUTTON_STARTGAME		1001	//开始游戏按钮ID
#define BUTTON_HELP				1002	//帮助按钮ID
#define BUTTON_RESTORE			1003	//恢复游戏按钮ID
#define BUTTON_RESTART			1004	//重新开始按钮ID
#define BUTTON_RETURN			1005	//返回标题界面按钮ID
#define BUTTON_CONTINUE			1006	//继续游戏按钮ID
#define BUTTON_STAGE1			1007	//选择场景按钮ID
#define BUTTON_STAGE2			1008	
#define BUTTON_STAGE3			1009	
#define BUTTON_STAGE4			1010	
#define BUTTON_KULA				1011	//选择角色按钮ID	
#define BUTTON_KYOKUSANAGI		1012	

//按钮尺寸
#define BUTTON_NORMAL_WIDTH		212		//普通按钮宽度
#define BUTTON_NORMAL_HEIGHT	76		//普通按钮高度
#define BUTTON_STAGE_WIDTH		240		//选择场景按钮宽度
#define BUTTON_STAGE_HEIGHT		136		//选择场景按钮高度
#define BUTTON_ROLE_WIDTH		200		//选择角色按钮宽度
#define BUTTON_ROLE_HEIGHT		200		//选择角色按钮高度


#define TIMER_GAMETIMER				1		//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		30		//默认计时器刷新间隔的毫秒数

#define  HEALTH_MAX	100 //血量上限
#define  RAGE_MAX	25 //怒气上限

//英雄属性
#define KULA_FRAME_V	1 / 2.0
#define KULA_ATK	3
#define KULA_DEF	2
#define KULA_VX		3
#define KULA_VY		10
#define KYOKUSANAGI_FRAME_V	1 / 3.0
#define KYOKUSANAGI_ATK	3
#define KYOKUSANAGI_DEF	2
#define KYOKUSANAGI_VX	3
#define KYOKUSANAGI_VY	10

#define BASE_Y	400	//英雄的初始纵坐标

//道具ID
#define PROP_ATKDEF	1	//体能药
#define	PROP_HEALTH	2	//回血药
#define PROP_LIFE	3	//复活药

//道具尺寸
#define PROP_WIDTH	50	//道具宽度
#define PROP_HEIGHT	78	//道具长度


#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage {
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）

};


// 按钮结构体
struct Button {
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// 英雄结构体
struct Hero {
	HBITMAP img;
	int type;				// 英雄类型
	int state;				// 英雄状态（站立、行走、攻击）
	float frame_column;		// 当前使用帧所处的列
	int* frame_length;		// 英雄帧序列在每种状态下的长度
	int* frame_width;		// 英雄帧序列在每种状态下的帧宽度
	int* frame_height;		// 英雄帧序列在每种状态下的帧高度
	int* frame_total_height;// 当前使用帧所处的高度（从上到下）
	int* frame_biasx;		// 英雄帧序列在每种状态下的偏置

	int x;					// 坐标x
	int y;					// 坐标y
	int vx;					// 速度x
	int vy;					// 速度y
	int health;				// 生命值

	/*待添加其他属性*/

	int inverse;			//是否反转
	int* frame_biasy;		//英雄帧序列在每种状态下的竖直偏置
	double frame_v;			//帧的变化速率
	int* frame_mid_length;	//英雄帧序列在每种状态下的中间长度
	int rage;				//怒气值
	int atk;				//攻击力
	int def;				//防御力
	int life;				//复活次数

	//记录英雄的操作
	bool keyUpDown = false;//方向
	bool keyDownDown = false;
	bool keyLeftDown = false;
	bool keyRightDown = false;
	bool keyDoubleLeftDown = false;//双击方向
	bool keyDoubleRightDown = false;
	bool keyPunchDown = false;//轻拳
	bool keyKickDown = false;//轻踢
	bool key_H_PunchDown = false;//重拳
	bool key_H_KickDown = false;//重踢
	bool keyDefendDown = false;//防御
	bool keyUltimateDown = false;//大招
	bool keyDodgeDown = false;//闪避

	//英雄是否受伤
	bool injured = false;//受轻伤
	bool h_injured = false;//受重伤
};

//TODO: 添加游戏需要的更多种数据（地物、砖块等）

// 道具结构体
struct Prop {
	HBITMAP img;	//图片
	int propID;		//道具编号
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};


//碰撞盒结构体
struct Rect {
	int x1;	//左
	int x2;	//右
	int y1;	//上
	int y2;	//下
};

#pragma endregion


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region 其它游戏状态处理函数声明

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// 添加英雄函数
Hero* CreateHero(int type, int x, int y);

// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

// 刷新单位状态函数
void UpdateUnits(HWND hWnd);

//英雄行为函数
void HeroBehaviour(Hero* hero);

//TODO: 添加游戏需要的更多函数

//游戏暂停
void Pause(HWND hWnd);

//恢复游戏
void Restore(HWND hWnd);

//重新开始本关卡
void Restart(HWND hWnd);

//获取英雄身体碰撞盒
Rect* GetBodyRect(Hero* hero);

//获取英雄攻击范围碰撞盒
Rect* getAttackRect(Hero* hero);

//获取道具碰撞盒
Rect* GetPropRect(Prop* prop);

//碰撞检测
bool detection(Rect* rect1, Rect* rect2);

//攻击
void Attack(Hero* src);

//AI自动行动
void EnemyOperate(Hero* hero);

//判断英雄是否接触道具
bool TouchProp(Hero* hero, Prop* prop);

#pragma endregion 


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src);

#pragma endregion