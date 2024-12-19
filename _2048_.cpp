#include <iostream>
#include <string>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#define WIDE  100

bool runtime = 1;
enum {
	LEFT = 1, UP, DOWN, RIGHT
};
int de[5][2] = {0, 0, -1, 0, 0, -1, 0, 1, 1, 0 };
class block{
public:
	int point;
	int rx, ry, flag;
	block() {
		point = 0;
		rx = 0, ry = 0;
		flag = 0;
		std::cout << "create a block\n";
	}
	void set(int x, int y) {
		rx = x, ry = y;
	}
};

class Blocks {
public:
	block blocks[4][4];
	int num;
	std::vector<block> arr;
	int status;
	int pace;
	Blocks() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				blocks[i][j].point = 0;
			arr.clear();
		}
		num = 0;
	}

	void create_block() {
		if (num >= 16) {
			std::cout << "block fill" << std::endl;
			return;
		}
		srand((unsigned int)time(NULL));
		int p = 0, tmp = rand() % (16 - num);
		while (blocks[p / 4][p % 4].point) p++;
		while (tmp--)
		{
			p++;
			while (blocks[p / 4][p % 4].point) p++;
		}
		blocks[p / 4][p % 4].point = 1;
		blocks[p / 4][p % 4].set(p / 4 * WIDE, p % 4 *WIDE);
		num++;
		arr.push_back(blocks[p / 4][p % 4]);
	}

	void init(int N) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				blocks[i][j].point = 0;
		}
		arr.clear();
		num = 0;
		for (int i = 0; i < N; i++)
			create_block();
		pace = 0;
		status = 0;
		std::cout << "Init finished" << std::endl;
	}

	void render()
	{
		for (auto it : arr) {
			//std::cout << it.rx << ' ' << it.ry << std::endl;
			setfillcolor(RGB(it.point * 10, 50, 50));
			fillrectangle(it.rx, it.ry, it.rx + WIDE, it.ry + WIDE);
		}
	}
	bool check(int x,int  y, int dir) {
		block* b = &blocks[x][y];
		int tmpx = x + de[dir][0], tmpy = y + de[dir][1];
		if (tmpx < 0 || tmpx > 3 || tmpy < 0 || tmpy > 3) {
			return false;
		}
		block* b1 = &blocks[tmpx][tmpy];
		if (b1->point == 0) return true;
		if (b1->point == b->point) return true;
		if (b1->flag) return true;
		return false;
	}
	void moveblock(int x, int y, int dir) {
		if (check(x, y, dir)) {
			std::cout << " move " << " "<<x  << " " << y << " pace " << pace<<  std::endl;
			block* b = &blocks[x][y];
			b->flag = 1;
			b->rx = x * WIDE + de[dir][0] * pace;
			b->ry = y * WIDE + de[dir][1] *  pace;
		}
	}

	void move() {
		if (pace < 100) {
			arr.clear();
			pace += 10;
			switch (status) {
			default:
				break;
			case LEFT:
				for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++) 
					if (blocks[i][j].point) {
						moveblock(i, j, status);
						arr.push_back(blocks[i][j]);
					}
				break;
			case UP: 
				for (int j = 0; j < 4; j++)
				for(int i = 0;i < 4;i ++)
					if (blocks[i][j].point){
						moveblock(i, j, status);
						arr.push_back(blocks[i][j]);
					}
				break;
			case DOWN:
				for (int j = 3; j >= 0; j --)
					for (int i = 0; i < 4; i++)
						if (blocks[i][j].point) {
							moveblock(i, j, status);
							arr.push_back(blocks[i][j]);
						}
				break;
			case RIGHT: 
				for (int i = 3; i >=0 ; i --)
				for (int j = 0; j < 4; j++) 
						if (blocks[i][j].point) {
							moveblock(i, j, status);
							arr.push_back(blocks[i][j]);
						}
			}
		}
	}


}Blks;

ExMessage msg;
void logic() {

	ExMessage msg;
	while (!Blks.status && peekmessage(&msg, -1, 1)) {
		if (msg.message == WM_KEYDOWN) {
			int dir = -1;
			std::cout << dir << std::endl;
			switch (msg.vkcode) {
			case VK_LEFT : 
				dir = LEFT;
				std::cout << "left" << std::endl;
				break;
			case VK_UP : 
				dir = UP;
				std::cout << "up" << std::endl;
				break;
			case VK_DOWN: 
				dir = DOWN;
				std::cout << "down" << std::endl;
				break;
			case VK_RIGHT:
				dir = RIGHT;
				std::cout << "right" << std::endl;
				break;
			default :
				break;
			}
			std::cout << dir << std::endl;
			if (~dir) Blks.status = dir;
		}
	}
	if(Blks.status)
		Blks.move();
}

void render() {
	cleardevice();
	Blks.render();
}

void game_init() {
	setbkcolor(RGB(255, 255, 255));
	Blks.init(2);
}

int main()
{
	srand((unsigned int)time(NULL));
	initgraph(400, 400, EX_SHOWCONSOLE);
	BeginBatchDraw();
	game_init();
	while (runtime)
	{
		logic();
		render();
		FlushBatchDraw();
		Sleep(10);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}