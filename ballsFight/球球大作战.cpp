#include <stdio.h>
#include <graphics.h>//包含图形库
#include <stdlib.h>
#include<time.h>
#include <math.h>

#include <mmsystem.h> // 播放音乐的
#pragma comment(lib,"winmm.lib")
// #include <D:/code learning/c++_language/sfSnake-master/ballsFight/SFML/Audio.hpp>


#define FOOD_NUM 200 // 食物数量
#define WIDTH 1500
#define HEIGHT 800
#define AI_NUM 10
#define BIGGEST_SIZE 200
/*
做游戏：1.窗口，画图，贴图...easyx
食物：圆（圆心坐标，半径，颜色，是否存在的标志）
*/

struct Ball // 小球结构，玩家，食物，自动动球
{
	int x;
	int y;
	int r;//半径
	DWORD color;
	bool flag; // 是否存在
};

struct Ball food[FOOD_NUM];
struct Ball player;
struct Ball ai[AI_NUM];
int score = 0;
void GameInit()
{
	//播放bgm,windows系统的api函数，用于播放音乐,文件名不要有空格
	/*mciSendString("open ./1.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);*/



	srand((unsigned)time(NULL));
	// 找到每一个元素
	for (int i = 0; i < FOOD_NUM; i++)
	{
		food[i].x = rand() % WIDTH;
		food[i].y = rand() % HEIGHT;
		food[i].flag = true; // 刚刚开始食物存在，未被吃掉
		food[i].r = rand() % 6 + 1;
		food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	//初始化玩家数据
	player.x = rand() % WIDTH;
	player.y = rand() % HEIGHT;
	player.r = 18; // 只要比最大的食物大就行
	player.flag = true;
	player.color = BLUE;

	//初始化ai
	for (int i = 0; i < AI_NUM; i++)
	{
		ai[i].x = rand() % WIDTH;
		ai[i].y = rand() % HEIGHT;
		ai[i].flag = true;
		ai[i].r = rand() % 10 + 10;
		ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
}

//绘制
void GameDraw()
{
	//防止闪屏
	BeginBatchDraw();// 双缓冲
	setbkcolor(WHITE);
	cleardevice(); //清屏
	for (int i = 0; i < FOOD_NUM; i++) {
		if (food[i].flag)
		{
			setfillcolor(food[i].color);// 设置填充颜色
			solidcircle(food[i].x, food[i].y, food[i].r); // 画一个填充圆

		}
		else {
			food[i].x = rand() % WIDTH;
			food[i].y = rand() % HEIGHT;
			food[i].flag = true; // 刚刚开始食物存在，未被吃掉
			food[i].r = rand() % 6 + 1;
			food[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
	}
	//绘制玩家
	if (player.flag)
	{
		setfillcolor(player.color);
		solidcircle(player.x, player.y, player.r);
		settextcolor(GREEN); // 设置文字颜色
		settextstyle(30, 0, "黑体");
		setbkmode(0); // 设置文字背景透明
		outtextxy(player.x - 18, player.y - 3, "you");
	}
	else
	{
		outtextxy(100, 100, "game over");
		exit(-1);
	}

	// 输出分数
	char str[20];
	sprintf_s(str, "score:%d", score);
	outtextxy(20, 20, str);

	//绘制ai
	for (int i = 0; i < AI_NUM; i++)
	{
		if (ai[i].flag)
		{
			setfillcolor(ai[i].color);// 设置填充颜色
			solidcircle(ai[i].x, ai[i].y, ai[i].r); // 画一个填充圆

		}
		else {
			ai[i].x = rand() % WIDTH;
			ai[i].y = rand() % HEIGHT;
			ai[i].flag = true; // 刚刚开始ai存在，未被吃掉
			ai[i].r = rand() % 6 + 1;
			ai[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
	}

	EndBatchDraw();
}
//玩家移动起来
void keyControl(int speed)
{
	//获取键盘消息，用户按的是什么,这是一个windows系统函数，是异步函数，且更加丝滑，而_getch()是阻塞函数，会等着输入
	// 这里wsad填大写，大小写可以兼容，填小写就不行了
	if ((GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)) && player.y >= 0) // 按了上键
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
//求两点之间距离
double distance(struct Ball a, struct Ball b)
{
	return sqrt((double)(a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
//玩家吃食物,ai吃food
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
//互相吃
void eatEachOther(Ball& A, Ball& B)
{
	
	
	
}
//ai移动
void aiMove()
{
	for (int i = 0; i < AI_NUM; i++)
	{
		if (ai[i].flag)
		{
			// 寻找最近的ai或玩家
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

			// 计算距离向量
			int dx = ai[i].x - target.x;
			int dy = ai[i].y - target.y;

			// 根据向量调整速度
			double magnitude = sqrt(dx * dx + dy * dy);
			double speed = 1.1; // 可根据需要调整速度

			//追
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
			else //躲
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