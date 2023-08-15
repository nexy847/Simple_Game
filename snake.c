#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include "调试.h"


#define U 1
#define D 2
#define L 3
#define R 4//蛇的运行状态:上下左右

typedef struct snake {//定义蛇身结构体
	int x;
	int y;
	struct snake* next;
}snake;

int score = 0, add = 10;//得分记录和食物得分
int status, sleeptime = 100;//蛇的运行状态,每次运行的时间间隔
snake* head ;//蛇头指针
snake* food ;//食物指针
snake* q;//遍历蛇身用
int endgamestatus = 0;//游戏结束的情况 1:撞到墙 2:咬到自己 3:主动退出游戏

void Pos(int x, int y) {
	//GetStdHandle函数的作用是获取指定类型的标准设备的句柄
	//一个预定义的常量STD_OUTPUT_HANDLE,标准输出设备的类型,控制台屏幕或终端窗口
	//SetConsoleCursorPosition函数的作用是设置控制台输出缓冲区中光标的位置

	COORD pos;//结构体变量,用以存储光标的位置
	HANDLE hOutput;//一个句柄变量
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//将控制台输出缓冲区返回给hOutput
	SetConsoleCursorPosition(hOutput, pos);//这里将hOutput表示的控制台输出缓冲区中光标移动到pos表示的位置
}

void create_map() {//创建一个地图
	int i;
	for (i = 0; i <= 60; i += 2) {//打印上下边框
		Pos(i, 0);//将光标移到第0行第i列
		printf("■");
		Pos(i, 29);//将光标移到第26行第i列
		printf("■");
	}
	for (i = 1; i < 29; i++) {//打印左右边框
		Pos(0, i);//将光标移动到第i行第0列
		printf("■");
		Pos(60, i);//将光标移动到第i行第56列
		printf("■");
	}
}

void initsnake() {//初始化蛇身
	snake* tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));
	tail->x = 24;
	tail->y = 5;//这两句是指定蛇出现的位置
	tail->next = NULL;
	for (i = 1; i <= 4; i++) {//头插法
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + i * 2;
		head->y = 5;
		tail = head;//让tail指向head(新创建的结点),让下一次循环新的head依然指向头结点
	}
	while (tail != NULL) {//输出蛇身
		Pos(tail->x, tail->y);
		printf("■");
		tail = tail->next;
	}
}

int biteself() {//判断是否咬到自己
	snake* self = head->next;
	while (self != NULL) {
		if (self->x == head->x && self->y == head->y) {//蛇头和蛇身是否重合
			return 1;
		}
		self = self->next;
	}
	return 0;
}

void createfood() {
	snake* food_1;
	srand((unsigned)time(NULL));
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0) {//保证是偶数,因为蛇头的x坐标也是偶数,以此保证食物与蛇头对齐(蛇头走两格)
		//游戏界面是一个52*24的矩形
		food_1->x = rand() % 56 + 2;//[2,52]
	}
	food_1->y = rand() % 29 + 1;//[1,24]
	q = head;
	while (q->next == NULL) {
		if (q->x == food_1->x && q->y == food_1->y) {//如果食物与蛇身重合
			free(food_1);
			createfood();
		}
		q = q->next;
	}
	Pos(food_1->x, food_1->y);
	food = food_1;
	printf("■");
}

void cantcrosswall() {
	if (head->x == 0 || head->x == 60 || head->y == 0 || head->y == 29) {
		endgamestatus = 1;
		endgame();
	}
}

void snakemove() {
	snake* headnext;
	cantcrosswall();

	headnext = (snake*)malloc(sizeof(snake));
	if (status == U) {
		headnext->x = head->x;
		headnext->y = head->y - 1;
		if (headnext->x == food->x && headnext -> y == food->y) {//如果下一格有食物
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {//如果下一格没有食物
			headnext->next = head;
			head = headnext;//往前走了一个结点
			q = head;
			while (q->next->next != NULL) {//让q指向倒数第二个结点,因为要删除最后一个结点
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status ==  D) {
		headnext->x = head->x;
		headnext->y = head->y + 1;
		if (headnext->x == food->x && headnext->y == food->y) {//有食物
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {//如果没有食物
			headnext->next = head;
			head = headnext;
			q = head;
			while (q->next->next != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == L) {
		headnext->x = head->x - 2;
		headnext->y = head->y;
		if (headnext->x == food->x && headnext->y == food->y) {//有食物
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {//没食物
			headnext->next = head;
			head = headnext;
			q = head;
			while (q->next->next != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == R) {
		headnext -> x = head->x + 2;
		headnext -> y = head->y;
		if (headnext->x == food->x && headnext->y == food->y) {//有食物
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {
			headnext->next = head;
			head = headnext;
			q = head;
			while (q->next->next != NULL) {
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf(" ");
			free(q->next);
			q->next = NULL;
		}
	}
	if (biteself() == 1) {
		endgamestatus = 2;
		endgame();
	}
}

void pause() {//暂停游戏
	while (1) {
		Sleep(300);//sleep是一个函数，它可以让程序暂停一段时间，单位是毫秒
		if (GetAsyncKeyState(VK_SPACE)) {//接收虚拟按键(键盘键)
			break;
		}
	}
}

void gamecircle() {//控制游戏
	Pos(64, 15);
	printf("不能穿墙，不能咬到自己\n");
	Pos(64, 16);
	printf("用↑.↓.←.→分别控制蛇的移动.");
	Pos(64, 17);
	printf("F1 为加速，F2 为减速\n");
	Pos(64, 18);
	printf("ESC ：退出游戏.space：暂停游戏.");
	status = R;//蛇初始的移动方向
	while (1) {
		Pos(64, 10);
		printf("得分%d", score);
		Pos(64, 11);
		printf("每个食物得分:%d", add);
		if (GetAsyncKeyState(VK_UP) && status != D) {
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN)&&status!=U) {
			status = D;
		}
		else if (GetAsyncKeyState(VK_RIGHT)&&status!=L) {
			status = R;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R) {
			status = L;
		}
		else if (GetAsyncKeyState(VK_SPACE)) {
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			endgamestatus = 3;
			break;
		}
		else if (GetAsyncKeyState(VK_F1)) {
			if (sleeptime >= 50) {
				sleeptime = sleeptime - 30;
				add = add + 2;
				if (sleeptime == 320) {
					add = 2;//防止减到一之后再加回来有错
				}
			}
		}
		else if (GetAsyncKeyState(VK_F2)) {
			if (sleeptime < 350) {
				sleeptime = sleeptime + 30;
				add = add - 2;
				if (sleeptime == 350) {
					add = 1;
				}
			}
		}
		//先做了加减速时的得分加减
		Sleep(sleeptime);//后通过Sleep函数来控制加减速
		snakemove();
	}
}

void welcometogame() {
	//system是一个函数，它可以执行一条系统命令，比如打开一个程序，清空屏幕，暂停等
	Pos(40, 12);
	printf("贪吃蛇莱拉!");
	Pos(40, 25);
	system("pause"); //让程序暂停，等待用户按任意键继续
	system("cls");//清空屏幕，把之前的输出内容擦掉
	Pos(25, 12);
	printf("玩法:这都不会玩?\n f1加速,f2减速\n");
	Pos(25, 13);
	printf("减速分低,加速分高\n");
	system("pause");
	system("cls");
}

void endgame() {
	system("cls");//清空屏幕
	Pos(24, 12);
	if (endgamestatus == 1) {
		printf("不会有人撞墙吧");
	}
	else if (endgamestatus == 2) {
		printf("怎么会有人蠢到咬自己");
	}
	else if (endgamestatus == 3) {
		printf("别呀,客官");
	}
	Pos(24, 13);
	printf("得分:%d\0", score);
	exit(0);//让程序正常退出
}

void gamestart() {
	system("mode con cols=100 lines=30");
	welcometogame();
	create_map();
	initsnake();
	createfood();
}

int main() {
	gamestart();
	gamecircle();
	endgame();
	return 0;
}