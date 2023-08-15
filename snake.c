#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include "����.h"


#define U 1
#define D 2
#define L 3
#define R 4//�ߵ�����״̬:��������

typedef struct snake {//��������ṹ��
	int x;
	int y;
	struct snake* next;
}snake;

int score = 0, add = 10;//�÷ּ�¼��ʳ��÷�
int status, sleeptime = 100;//�ߵ�����״̬,ÿ�����е�ʱ����
snake* head ;//��ͷָ��
snake* food ;//ʳ��ָ��
snake* q;//����������
int endgamestatus = 0;//��Ϸ��������� 1:ײ��ǽ 2:ҧ���Լ� 3:�����˳���Ϸ

void Pos(int x, int y) {
	//GetStdHandle�����������ǻ�ȡָ�����͵ı�׼�豸�ľ��
	//һ��Ԥ����ĳ���STD_OUTPUT_HANDLE,��׼����豸������,����̨��Ļ���ն˴���
	//SetConsoleCursorPosition���������������ÿ���̨����������й���λ��

	COORD pos;//�ṹ�����,���Դ洢����λ��
	HANDLE hOutput;//һ���������
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//������̨������������ظ�hOutput
	SetConsoleCursorPosition(hOutput, pos);//���ｫhOutput��ʾ�Ŀ���̨����������й���ƶ���pos��ʾ��λ��
}

void create_map() {//����һ����ͼ
	int i;
	for (i = 0; i <= 60; i += 2) {//��ӡ���±߿�
		Pos(i, 0);//������Ƶ���0�е�i��
		printf("��");
		Pos(i, 29);//������Ƶ���26�е�i��
		printf("��");
	}
	for (i = 1; i < 29; i++) {//��ӡ���ұ߿�
		Pos(0, i);//������ƶ�����i�е�0��
		printf("��");
		Pos(60, i);//������ƶ�����i�е�56��
		printf("��");
	}
}

void initsnake() {//��ʼ������
	snake* tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));
	tail->x = 24;
	tail->y = 5;//��������ָ���߳��ֵ�λ��
	tail->next = NULL;
	for (i = 1; i <= 4; i++) {//ͷ�巨
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + i * 2;
		head->y = 5;
		tail = head;//��tailָ��head(�´����Ľ��),����һ��ѭ���µ�head��Ȼָ��ͷ���
	}
	while (tail != NULL) {//�������
		Pos(tail->x, tail->y);
		printf("��");
		tail = tail->next;
	}
}

int biteself() {//�ж��Ƿ�ҧ���Լ�
	snake* self = head->next;
	while (self != NULL) {
		if (self->x == head->x && self->y == head->y) {//��ͷ�������Ƿ��غ�
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
	while ((food_1->x % 2) != 0) {//��֤��ż��,��Ϊ��ͷ��x����Ҳ��ż��,�Դ˱�֤ʳ������ͷ����(��ͷ������)
		//��Ϸ������һ��52*24�ľ���
		food_1->x = rand() % 56 + 2;//[2,52]
	}
	food_1->y = rand() % 29 + 1;//[1,24]
	q = head;
	while (q->next == NULL) {
		if (q->x == food_1->x && q->y == food_1->y) {//���ʳ���������غ�
			free(food_1);
			createfood();
		}
		q = q->next;
	}
	Pos(food_1->x, food_1->y);
	food = food_1;
	printf("��");
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
		if (headnext->x == food->x && headnext -> y == food->y) {//�����һ����ʳ��
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {//�����һ��û��ʳ��
			headnext->next = head;
			head = headnext;//��ǰ����һ�����
			q = head;
			while (q->next->next != NULL) {//��qָ�����ڶ������,��ΪҪɾ�����һ�����
				Pos(q->x, q->y);
				printf("��");
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
		if (headnext->x == food->x && headnext->y == food->y) {//��ʳ��
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {//���û��ʳ��
			headnext->next = head;
			head = headnext;
			q = head;
			while (q->next->next != NULL) {
				Pos(q->x, q->y);
				printf("��");
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
		if (headnext->x == food->x && headnext->y == food->y) {//��ʳ��
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("��");
				q = q->next;
			}
			score = score + add;
			createfood();
		}
		else {//ûʳ��
			headnext->next = head;
			head = headnext;
			q = head;
			while (q->next->next != NULL) {
				Pos(q->x, q->y);
				printf("��");
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
		if (headnext->x == food->x && headnext->y == food->y) {//��ʳ��
			headnext->next = head;
			head = headnext;
			q = head;
			while (q != NULL) {
				Pos(q->x, q->y);
				printf("��");
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
				printf("��");
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

void pause() {//��ͣ��Ϸ
	while (1) {
		Sleep(300);//sleep��һ���������������ó�����ͣһ��ʱ�䣬��λ�Ǻ���
		if (GetAsyncKeyState(VK_SPACE)) {//�������ⰴ��(���̼�)
			break;
		}
	}
}

void gamecircle() {//������Ϸ
	Pos(64, 15);
	printf("���ܴ�ǽ������ҧ���Լ�\n");
	Pos(64, 16);
	printf("�á�.��.��.���ֱ�����ߵ��ƶ�.");
	Pos(64, 17);
	printf("F1 Ϊ���٣�F2 Ϊ����\n");
	Pos(64, 18);
	printf("ESC ���˳���Ϸ.space����ͣ��Ϸ.");
	status = R;//�߳�ʼ���ƶ�����
	while (1) {
		Pos(64, 10);
		printf("�÷�%d", score);
		Pos(64, 11);
		printf("ÿ��ʳ��÷�:%d", add);
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
					add = 2;//��ֹ����һ֮���ټӻ����д�
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
		//�����˼Ӽ���ʱ�ĵ÷ּӼ�
		Sleep(sleeptime);//��ͨ��Sleep���������ƼӼ���
		snakemove();
	}
}

void welcometogame() {
	//system��һ��������������ִ��һ��ϵͳ��������һ�����������Ļ����ͣ��
	Pos(40, 12);
	printf("̰��������!");
	Pos(40, 25);
	system("pause"); //�ó�����ͣ���ȴ��û������������
	system("cls");//�����Ļ����֮ǰ��������ݲ���
	Pos(25, 12);
	printf("�淨:�ⶼ������?\n f1����,f2����\n");
	Pos(25, 13);
	printf("���ٷֵ�,���ٷָ�\n");
	system("pause");
	system("cls");
}

void endgame() {
	system("cls");//�����Ļ
	Pos(24, 12);
	if (endgamestatus == 1) {
		printf("��������ײǽ��");
	}
	else if (endgamestatus == 2) {
		printf("��ô�����˴���ҧ�Լ�");
	}
	else if (endgamestatus == 3) {
		printf("��ѽ,�͹�");
	}
	Pos(24, 13);
	printf("�÷�:%d\0", score);
	exit(0);//�ó��������˳�
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