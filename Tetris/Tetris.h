#pragma once

#include <iostream>
using namespace std;

#include <Windows.h>

const int T1 = 0;
const int T2 = 1;
const int T3 = 2;
const int T4 = 3;
const int Z11 = 4;
const int Z12 = 5;
const int Z21 = 6;
const int Z22 = 7;
const int L11 = 8;
const int L12 = 9;
const int L13 = 10;
const int L14 = 11;
const int L21 = 12;
const int L22 = 13;
const int L23 = 14;
const int L24 = 15;
const int I1 = 16;
const int I2 = 17;
const int O = 18;

class Tetris 
{
public:
	Tetris();
	~Tetris();

	void drawmap();
	void setcurpos(int x, int y, int color);
	void drawbox(int x,int y,int id);//参数x,y是用来定位本次显示图形，和游戏界面外下一次要显示的图形
	void erasebox(int x,int y,int id);
	int judge(int x,int y,int id);
	int change(int id);
	void updatemap(int id);
	void run();

private:
	int map[30][25];//map数组主要作为一张表来记录游戏界面哪些坐标有方块,这里的map将游戏的边框也包含进去了
	int pos[2];//确定方块在游戏界面的坐标
	int level;
	int score;
	int top;//记录方块堆积的最高点,消除的时候需要
};
