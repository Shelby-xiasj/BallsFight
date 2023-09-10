#include <stdio.h>
#include <graphics.h>//����ͼ�ο�
#include <stdlib.h>
#include<time.h>
#include <math.h>

#include <mmsystem.h> // �������ֵ�
#pragma comment(lib,"winmm.lib")
// #include <D:/code learning/c++_language/sfSnake-master/ballsFight/SFML/Audio.hpp>


#define FOOD_NUM 200 // ʳ������
#define WIDTH 1500
#define HEIGHT 800
#define AI_NUM 10
#define BIGGEST_SIZE 200
/*
����Ϸ��1.���ڣ���ͼ����ͼ...easyx
ʳ�Բ��Բ�����꣬�뾶����ɫ���Ƿ���ڵı�־��
*/

struct Ball // С��ṹ����ң�ʳ��Զ�����
{
	int x;
	int y;
	int r;//�뾶
	DWORD color;
	bool flag; // �Ƿ����
};

struct Ball food[FOOD_NUM];
struct Ball player;
struct Ball ai[AI_NUM];
int score = 0;
void GameInit()
{
	//����bgm,windowsϵͳ��api���������ڲ�������,�ļ�����Ҫ�пո�
	/*mciSendString("open ./1.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);*/



	srand((unsigned)time(NULL));
	// �ҵ�ÿһ��Ԫ��
	for (int i = 0; i < FOOD_NUM; i++)
	{
		food[i].x = rand() % WIDTH;
		food[i].y = rand() % HEIGHT;
		food[i].flag = true; // �ոտ�ʼʳ����ڣ�δ���Ե�
		food[i].r = rand() % 6 + 1;
		food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	//��ʼ���������
	player.x = rand() % WIDTH;
	player.y = rand() % HEIGHT;
	player.r = 18; // ֻҪ������ʳ������
	player.flag = true;
	player.color = BLUE;

	//��ʼ��ai
	for (int i = 0; i < AI_NUM; i++)
	{
		ai[i].x = rand() % WIDTH;
		ai[i].y = rand() % HEIGHT;
		ai[i].flag = true;
		ai[i].r = rand() % 10 + 10;
		ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
}

//����
void GameDraw()
{
	//��ֹ����
	BeginBatchDraw();// ˫����
	setbkcolor(WHITE);
	cleardevice(); //����
	for (int i = 0; i < FOOD_NUM; i++) {
		if (food[i].flag)
		{
			setfillcolor(food[i].color);// ���������ɫ
			solidcircle(food[i].x, food[i].y, food[i].r); // ��һ�����Բ

		}
		else {
			food[i].x = rand() % WIDTH;
			food[i].y = rand() % HEIGHT;
			food[i].flag = true; // �ոտ�ʼʳ����ڣ�δ���Ե�
			food[i].r = rand() % 6 + 1;
			food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
	}
	//�������
	if (player.flag)
	{
		setfillcolor(player.color);
		solidcircle(player.x, player.y, player.r);
		settextcolor(GREEN); // ����������ɫ
		settextstyle(30, 0, "����");
		setbkmode(0); // �������ֱ���͸��
		outtextxy(player.x - 18, player.y - 3, "you");
	}
	else
	{
		outtextxy(100, 100, "game over");
		exit(-1);
	}

	// �������
	char str[20];
	sprintf_s(str, "score:%d", score);
	outtextxy(20, 20, str);

	//����ai
	for (int i = 0; i < AI_NUM; i++)
	{
		if (ai[i].flag)
		{
			setfillcolor(ai[i].color);// ���������ɫ
			solidcircle(ai[i].x, ai[i].y, ai[i].r); // ��һ�����Բ

		}
		else {
			ai[i].x = rand() % WIDTH;
			ai[i].y = rand() % HEIGHT;
			ai[i].flag = true; // �ոտ�ʼai���ڣ�δ���Ե�
			ai[i].r = rand() % 6 + 1;
			ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
	}

	EndBatchDraw();
}
//����ƶ�����
void keyControl(int speed)
{
	//��ȡ������Ϣ���û�������ʲô,����һ��windowsϵͳ���������첽�������Ҹ���˿������_getch()���������������������
	// ����wsad���д����Сд���Լ��ݣ���Сд�Ͳ�����
	if ((GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)) && player.y >= 0) // �����ϼ�
	{
		player.y -= speed;
	}
	if ((GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)) && player.y <= HEIGHT)
	{
		player.y += speed;
	}
	if ((GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT)) && player.x >= 0)
	{
		player.x -= speed;
	}
	if ((GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT)) && player.x <= WIDTH)
	{
		player.x += speed;
	}
}
//������֮�����
double distance(struct Ball a, struct Ball b)
{
	return sqrt((double)(a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
//��ҳ�ʳ��,ai��food
void eatFood() {
	for (int i = 0; i < FOOD_NUM; i++)
	{
		if (food[i].flag && distance(food[i], player) <= player.r)
		{
			food[i].flag = false;
			if (player.r < BIGGEST_SIZE)
			{
				player.r += food[i].r / 4;
			}
			score++;
		}
		for (int j = 0; j < AI_NUM; j++)
		{
			if (food[i].flag && distance(food[i], ai[j]) <= ai[j].r)
			{
				food[i].flag = false;
				if (ai[j].r < BIGGEST_SIZE)
				{
					ai[j].r += food[i].r / 4;
				}
			}
		}
	}

}
//�����
void eatEachOther(Ball& A, Ball& B)
{
	
	
	
}
//ai�ƶ�
void aiMove()
{
	for (int i = 0; i < AI_NUM; i++)
	{
		if (ai[i].flag)
		{
			// Ѱ�������ai�����
			int closest_index = -1;
			double closest_distance = INFINITY;

			for (int j = (i + 1) % AI_NUM; j != i; j = (j + 1) % AI_NUM)
			{
				if (ai[j].flag)
				{
					double distance_to_ai = distance(ai[i], ai[j]);
					if (distance_to_ai < closest_distance)
					{
						closest_distance = distance_to_ai;
						closest_index = j;
					}
					double distance_to_player = distance(ai[i], player);
					if (distance_to_player < closest_distance)
					{
						closest_distance = distance_to_player;
						closest_index = -2;
					}
				}
			}

			Ball& target = player;
			if (closest_index != -2 && closest_index != -1)
			{
				target = ai[closest_index];
			}

			// �����������
			int dx = ai[i].x - target.x;
			int dy = ai[i].y - target.y;

			// �������������ٶ�
			double magnitude = sqrt(dx * dx + dy * dy);
			double speed = 1.1; // �ɸ�����Ҫ�����ٶ�

			//׷
			if (target.r < ai[i].r)
			{
				if (magnitude > 0 && ai[i].x >= 0 && ai[i].x <= WIDTH && ai[i].y >= 0 && ai[i].y <= HEIGHT)
				{
					ai[i].x -= static_cast<int>(dx / magnitude * speed);
					ai[i].y -= static_cast<int>(dy / magnitude * speed);
				}
				if (distance(ai[i], target) < ai[i].r) {
					target.flag = false;

					if (ai[i].r < BIGGEST_SIZE)
					{
						ai[i].r += target.r / 4;
					}
				}
			}
			else //��
			{
				if (magnitude > 0 && ai[i].x >= 0 && ai[i].x <= WIDTH && ai[i].y >= 0 && ai[i].y <= HEIGHT)
				{
					ai[i].x += static_cast<int>(dx / magnitude * speed);
					ai[i].y += static_cast<int>(dy / magnitude * speed);
				}
				if (distance(ai[i], target) < target.r) {
					ai[i].flag = false;
					if (target.r < BIGGEST_SIZE)
					{
						target.r += ai[i].r / 4;
					}
				}
			}
		}
	}
}
int main(int argc, char* agv[]) {
	initgraph(WIDTH, HEIGHT);
	GameInit();

	while (1)
	{
		GameDraw();
		eatFood();
		keyControl(1);
		
		
		aiMove();
	}

	getchar();
	return 0;
}