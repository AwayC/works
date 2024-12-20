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
enum {
	SAME = 1, BLANK
};
int block_color[16][3] = {
	0,0,0,
	50, 0, 0,
	100, 0, 0,
	150, 0, 0,
	200, 0, 0,
	200, 50, 0,
	200, 100, 0,
	200, 150, 0,
	200, 200, 0,
	100, 200, 50,
	100, 200, 100,
	100, 200, 150,
	100, 200, 200,
	50, 200, 200,
	50, 50 , 150,
	50, 50, 100,
};
wchar_t strnum[16][5] = {L"0", L"2", L"4", L"8", L"16" , L"32",L"64" , L"128" , L"256", L"512", L"1024", L"2048", L"4096", L"8192", L"14", L"15"};
int de[5][2] = {0, 0, -1, 0, 0, -1, 0, 1, 1, 0 };
class block{
public:
	int point;
	int rx, ry, flag, st;
	block() {
		point = 0;
		rx = 0, ry = 0;
		flag = 0, st = 0;
		std::cout << "create a block\n";
	}
	void set(int x, int y) {
		rx = x, ry = y;
	}
};

class Blocks {
public:
	block blocks[4][4];
	int num, score;
	std::vector<block> arr;
	std::vector<int> buff;
	int status;
	int pace;
	Blocks() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				blocks[i][j].point = 0;
			arr.clear();
		}
		score = 0;
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
		buff.clear();
		num = 0;
#if 1
		for (int i = 0; i < N; i++)
			create_block();
#endif
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++)
				std::cout << blocks[i][j].point << ' ';
			std::cout << std::endl;
		}
		pace = 0;
		status = 0;
		std::cout << "Init finished" << std::endl;
	}
	wchar_t* trans(int n) {
		int len = 0;
		wchar_t s[8];
		if (n == 0) {
			s[0] = '0';
			return s;
		}
		while (n) {
			s[len++] = n % 10 + '0';
			n /= 10;
		}
		for (int i = 0; i < len / 2; i++) s[i] = s[len - 1 - i];
		return s;
	}
	void render()
	{
		for (auto it : arr) {
			//std::cout << it.rx << ' ' << it.ry << std::endl;
			setfillcolor(RGB(     block_color[it.point][0],     block_color[it.point][1],    block_color[it.point][2]  ));
			RECT r = { it.rx, it.ry, it.rx + WIDE, it.ry + WIDE};
			//RECT r = { 0, 0, 400, 400};
			settextcolor(RGB(255, 255, 255));
			fillrectangle(it.rx, it.ry, it.rx + WIDE, it.ry + WIDE);
			drawtext(strnum[it.point], &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			outtextxy(0, 405, L"score : ");
			outtextxy(55, 405, trans(score));
		}
	}
	int check(int x,int  y, int dir) {
		block* b = &blocks[x][y];
		int tmpx = x + de[dir][0], tmpy = y + de[dir][1];
		if (tmpx < 0 || tmpx > 3 || tmpy < 0 || tmpy > 3) {
			return 0;
		}
		block* b1 = &blocks[tmpx][tmpy];
		if (b1->point == 0) return BLANK;
		if (b1->flag) return BLANK;		
		if (b1->point == b->point && b->st == 0 && b1->st == 0) return SAME;
		return 0;
	}
	int moveblock(int x, int y, int dir) {
		if (check(x, y, dir)) {
			std::cout << " move " << " "<<x  << " " << y << " pace " << pace<<  std::endl;
			block* b = &blocks[x][y];
			b->flag = 1;
			b->rx = x * WIDE + de[dir][0] * pace;
			b->ry = y * WIDE + de[dir][1] *  pace;
			buff.push_back(arr.size());
			return 1;
		}
		return 0;
	}

	int move() {
		if (pace == 0) {
			int cnt = 0;
			arr.clear();
			buff.clear();
			pace += 10;
			switch (status) {
				default:
					break;
				case LEFT:
					for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++) 
						if (blocks[i][j].point) {
							cnt +=moveblock(i, j, status);
							arr.push_back(blocks[i][j]);
						}
					break;
				case UP: 
					for (int j = 0; j < 4; j++)
					for(int i = 0;i < 4;i ++)
						if (blocks[i][j].point){
							cnt +=moveblock(i, j, status);
							arr.push_back(blocks[i][j]);
						}
					break;
				case DOWN:
					for (int j = 3; j >= 0; j --)
						for (int i = 0; i < 4; i++)
							if (blocks[i][j].point) {
								cnt += moveblock(i, j, status);
								arr.push_back(blocks[i][j]);
							}
					break;
				case RIGHT: 
					for (int i = 3; i >=0 ; i --)
					for (int j = 0; j < 4; j++) 
							if (blocks[i][j].point) {
								cnt +=moveblock(i, j, status);
								arr.push_back(blocks[i][j]);
							}
					break;
			}
			if (cnt == 0) pace = 0;
			return cnt;
		}
		else if(pace < 100){
			pace += 10;
			for (int it : buff){
				block* b = &arr[it];
				b->rx = b->rx + de[status][0] * 10;
				b->ry = b->ry + de[status][1] * 10;
			}
			return buff.size();
		}
		else if(pace == 100){
			pace = 0;
			merge();
		}
	}
	void merge() {
		for (int it : buff) {
			mergeblock(&arr[it]);
		}
		arr.clear();
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				blocks[i][j].set(i * 100, j * 100);
				blocks[i][j].flag = 0;
				if (blocks[i][j].point)
					arr.push_back(blocks[i][j]);
				std::cout << blocks[i][j].point << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void mergeblock(block* b) {
		int x1 = b->rx / WIDE, y1 = b->ry / WIDE;
		int x = x1 - de[status][0], y = y1 - de[status][1];
		if (check(x, y, status) == SAME) {
			blocks[x1][y1].point++, blocks[x][y].point = 0, blocks[x1][y1].st = 1, num--;
			score += (1 << (b->point + 1));
		}
		else 
			blocks[x1][y1].point = b->point,  blocks[x][y].point = 0;
		std::cout << num << std::endl;
	}

	bool checkout() {
		int ret = 0;
		for (int k = 1; k <= 4; k ++) {
			for (int j = 0; j < 4; j++ ){
				for (int i = 0; i < 4; i++)
					ret += check(i, j, k);
			}
		}
		if (ret == 0) return 1;
		return 0;
	}

}Blks;

ExMessage msg;
bool logic() {

	ExMessage msg;
	if (Blks.status) flushmessage();
	while (peekmessage(&msg, -1, 1)) {
		flushmessage();
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
	if (Blks.status) {
		if (!Blks.move()) {
			Blks.status = 0;
			int ret = rand() % 2 + 1;
			while(ret --)
				Blks.create_block();
			Blks.arr.clear();
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					Blks.blocks[i][j].st = 0;
					Blks.blocks[i][j].set(i * 100, j * 100);
					if (Blks.blocks[i][j].point)
						Blks.arr.push_back(Blks.blocks[i][j]);
					std::cout << Blks.blocks[i][j].point << ' ';
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
	if (Blks.num == 16 && Blks.checkout()) return 1;
	return 0;
}

void render() {
	cleardevice();
	Blks.render();
}

void game_init() {
	//setbkcolor(RGB(255, 255, 255));
	Blks.init(2);
}

int main()
{
	srand((unsigned int)time(NULL));
	initgraph(400, 430, EX_SHOWCONSOLE);
	BeginBatchDraw();
	game_init();
	while (runtime)
	{
		int ret = logic();
		render();
		FlushBatchDraw();
		Sleep(10);
		if (ret) break;
	}
	Sleep(5000);
	EndBatchDraw();
	closegraph();
	return 0;
}