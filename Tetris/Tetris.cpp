#include "Tetris.h"
#include <time.h>
#include <conio.h>
#include <stdio.h>

//下面两个数组体现出对数据结构的运用,利用id就可以查找shape数组id行方块每一部分的坐标以及查找height数组第id个元素就可以知道方块高度
//数组的每一行对应一种形状，数组每一行最后一个元素为颜色
int shape[19][9] = { { 0,0,1,0,2,0,1,1,FOREGROUND_RED | FOREGROUND_INTENSITY },
                     { 0,1,1,0,1,1,1,2,FOREGROUND_RED | FOREGROUND_INTENSITY },
                     { 0,1,1,0,1,1,2,1,FOREGROUND_RED | FOREGROUND_INTENSITY },
                     { 0,0,0,1,0,2,1,1,FOREGROUND_RED | FOREGROUND_INTENSITY }, //T字形4种形状
					 { 0,1,1,0,1,1,2,0,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY },
                     { 0,0,0,1,1,1,1,2,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY }, //反Z形状
					 { 0,0,1,0,1,1,2,1,FOREGROUND_GREEN | FOREGROUND_RED },
                     { 0,1,0,2,1,0,1,1,FOREGROUND_GREEN | FOREGROUND_RED }, //Z形状
					 { 0,2,1,0,1,1,1,2,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY },
					 { 0,0,0,1,1,1,2,1,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY },
					 { 0,0,0,1,0,2,1,0,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY },
					 { 0,0,1,0,2,0,2,1,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY }, //反L形状
					 { 0,0,0,1,0,2,1,2,FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY },
					 { 0,0,0,1,1,0,2,0,FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY },
					 { 0,0,1,0,1,1,1,2,FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY },
					 { 0,1,1,1,2,1,2,0,FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY }, //L形状
					 { 0,0,0,1,0,2,0,3,FOREGROUND_GREEN | FOREGROUND_INTENSITY },
					 { 0,0,1,0,2,0,3,0,FOREGROUND_GREEN | FOREGROUND_INTENSITY }, //I形状
					 { 0,0,0,1,1,0,1,1,FOREGROUND_BLUE | FOREGROUND_INTENSITY } //田字形状
};

int height[19] = { 2,3,2,2,2,3,2,3,3,2,3,2,3,2,3,2,4,1,2 };//记录不同形状的高度


Tetris::Tetris()
{
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 25; j++)
			map[i][j] = 0;
	pos[0] = 12;
	pos[1] = 1;
	level = 1;
	score = 0;
	top = 28;//将最高点初始为游戏界面的底部
}

Tetris::~Tetris()
{

}

void Tetris::setcurpos(int x, int y, int color = 1|2|4)//14红绿蓝混合成白色
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {(short)x,(short)y};
	SetConsoleTextAttribute(handle, color);//设置颜色
	SetConsoleCursorPosition(handle,coord);//设置光标位置
}

void Tetris::drawmap()
{
	setcurpos(0, 0);
	for (int i = 0; i < 25; i++)
		cout << "■";
	setcurpos(0, 29);
	for (int i = 0; i < 25; i++)
		cout << "■";
	for (int i = 1; i < 29; i++) 
	{
		setcurpos(0, i);
		cout << "■";
		setcurpos(48, i);
		cout << "■";
	}
	//绘制右侧信息框
	setcurpos(54, 0);
	cout << "下一次将要显示的图形：";
	setcurpos(54, 6);
	cout << "得分：" << score;
	setcurpos(54, 7);
	cout << "等级：" << level;
	setcurpos(54, 9);
	cout << "帮助信息：a键向左移动，d键向右移动";
	setcurpos(64, 10 );
	cout << "w键旋转，s键加速，每消除一行得10分！";
}

void Tetris::drawbox(int x,int y,int id)
{
	int cx = 0, cy = 0;

	for (int i = 0; i < 4; i++)
	{
		cx = x + shape[id][2 * i];
		cy = y + shape[id][2 * i + 1];
		setcurpos(2*cx, cy, shape[id][8]);
		cout << "■";
	}
}

void Tetris::erasebox(int x, int y, int id)
{
	int cx = 0, cy = 0;

	for (int i = 0; i < 4; i++)
	{
		cx = x + shape[id][2 * i];
		cy = y + shape[id][2 * i + 1];
		setcurpos(2*cx, cy);
		cout << "  ";
	}
}

int Tetris::judge(int x,int y,int id)
{
	int cx = 0, cy = 0;

	for (int i = 0; i < 4; i++)
	{
		cx = x + shape[id][2 * i];
		cy = y + shape[id][2 * i + 1];
		if (cx < 1 || cx > 23 || cy < 1 || cy > 28 || map[cy][cx] != 0)
			return -1;
	}

	return 0;
}

int Tetris::change(int id)
{
	switch (id)
	{
	case T1: id = T2; break;
	case T2: id = T3; break;
	case T3: id = T4; break;
	case T4: id = T1; break;
	case Z11: id = Z12; break;
	case Z12: id = Z11; break;
	case Z21: id = Z22; break;
	case Z22: id = Z21; break;
	case L11: id = L12; break;
	case L12: id = L13; break;
	case L13: id = L14; break;
	case L14: id = L11; break;
	case L21: id = L22; break;
	case L22: id = L23; break;
	case L23: id = L24; break;
	case L24: id = L21; break;
	case I1:  id = I2; break;
	case I2:  id = I1; break;
	case O: break;
	default:
		cout << "类型错误！";
		break;
	}

	return id;
}

//每一次方块到指定位置就更新一次
void Tetris::updatemap(int id)
{
	int clearlines = 0;//清空的行数
	
	//当方块到达位置后，使用map数组进行记录哪些位置有方块
	for (int i = 0; i < 4; i++)
	{
		int cx = pos[0] + shape[id][2 * i];
		int cy = pos[1] + shape[id][2 * i + 1];
		//map只是数组组成的一张表，用来标识对应的游戏界面中的某个坐标是否有方块，所以cx,cy是map的行或列无所谓
		map[cy][cx] = shape[id][8];
	}

	//更新最高点
	if (pos[1] < top)
		top = pos[1];

	//在每个形状方块的高度范围内按行消除
	for (int i = pos[1]; i < pos[1] + height[id]; i++)
	{
		int flag = 0;
		for (int j = 1; j < 24; j++)
		{
			if (map[i][j] == 0)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
			for (int k = i; k > top; k--)
			{
				//k=1说明顶行刚好全是方块，需要消除
				if (k == 1) 
				{
					for (int n = 1; n < 24; n++) 
					{
						map[k][n] = 0;
						setcurpos(2*n, k);
						cout << "  ";
					}
				}
				else 
				{
					for (int n = 1; n < 24; n++)
					{
						map[k][n] = map[k - 1][n];

						if (map[k][n] == 0)
						{
							setcurpos(2*n, k);
							cout << "  ";
						}
						else
						{
							setcurpos(2*n, k, map[k][n]);
							cout << "■";
						}
						//清空第top行
						if (k == top + 1)
						{
							setcurpos(2*n,k-1);
							cout << "  ";
							map[k - 1][n] = 0;
						}
					}
				}
			}
			top++;
			clearlines++;
			score += 10 * clearlines;
		}
	}
	setcurpos(54, 6);
	cout << "得分：" << score;
}


void Tetris::run()
{
	char ch = '\0';
	int num = 0;//计时作用,每次方块下降之前要等待num计数时间等待键盘输入
	int autonum;//超过这个数自动下降
	int flag;

	//随机产生方块
	srand((unsigned)time(NULL));
	int curid = rand() % 19;
	int nextid = rand() % 19;

	//初始化
	drawmap();
	drawbox(pos[0],pos[1],curid);
	drawbox(30,1,nextid);

	while (1)
	{
		level = score / 50 + 1;
		autonum = 10000 - 1000 * level;
		

		//自动下降
		if (num > autonum)
		{
			num = 0;
			//Sleep(100);
			if (judge(pos[0],pos[1]+1,curid) == 0)//可以继续向下
			{
				erasebox(pos[0], pos[1], curid);
				pos[1]++;
				drawbox(pos[0], pos[1], curid);
			}
			else 
			{
				updatemap(curid);

				curid = nextid;
				nextid = rand() % 19;
				pos[0] = 12;
				pos[1] = 1;
				drawbox(pos[0], pos[1], curid);
				erasebox(30, 1, curid);
				drawbox(30, 1, nextid);
				//更新游戏等级
				setcurpos(54, 7);
				cout << "等级：" << level;
				//判断是否堆满
				if (judge(pos[0],pos[1],curid) == -1)
				{
					setcurpos(54, 12);
					cout << "游戏结束！";
					system("pause");
					exit(0);
				}
			}
		}
		//等待键盘输入，此时方块停止
		if (_kbhit()) 
		{
			ch = _getch();//_getch不会自动显示到屏幕中

			if (ch == 'a')
			{
				if (judge(pos[0] - 1, pos[1], curid) == 0)
				{
					erasebox(pos[0], pos[1], curid);
					pos[0] --;
					drawbox(pos[0], pos[1], curid);
				}
			}
			else
				if (ch == 'd')
				{
					if (judge(pos[0] + 1, pos[1], curid) == 0)
					{
						erasebox(pos[0], pos[1], curid);
						pos[0] ++;
						drawbox(pos[0], pos[1], curid);
					}
				}
				else
					if (ch == 's')
					{
						if (judge(pos[0], pos[1] + 1, curid) == 0)
						{
							erasebox(pos[0], pos[1], curid);
							pos[1] ++;
							drawbox(pos[0], pos[1], curid);
						}
					}
					else
						if (ch == 'w')
						{
							int temp = curid;
							curid = change(curid);

							if (judge(pos[0], pos[1], curid) == 0)
							{
								erasebox(pos[0], pos[1], temp);
								drawbox(pos[0], pos[1], curid);
							}
							else
								curid = temp;
						}
						else
							if (ch = '\0')
								_getch();

			while (_kbhit())//读取剩余的键盘信息
				_getch();
		}
		num++;
	}	
}
