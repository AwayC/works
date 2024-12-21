#include <iostream>
#include <string>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#define WIDE  100
#define PACE 10
#define MG_TIME 5
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
	std::vector<block> mg;
	int status, firstmove;
	int pace, mg_cnt, move_stop = 0;
	Blocks() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				blocks[i][j].point = 0;
			arr.clear();
		}
		score = 0;
		num = 0;
		firstmove = 1;
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
		mg.push_back(blocks[p / 4][p % 4]);
	}

	void init(int N) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				blocks[i][j].point = 0;
		}
		arr.clear();
		buff.clear();
		num = 0;
		mg_cnt = 6;
		firstmove = 1;
#if 1
		for (int i = 0; i < N; i++)
			create_block();
#endif
		mg.clear();
		pace = 0;
		status = 0;
		std::cout << "Init finished" << std::endl;
	}
	wchar_t* trans(int n) {
		int len = 0;
		wchar_t s[9];
		if (n == 0) {
			s[0] = '0';
			return s;
		}
		while (n) {
			s[len++] = n % 10 + '0';
			n /= 10;
		}
		wchar_t tmp;
		for (int i = 0; i <= (len - 1) / 2; i++) { tmp = s[i], s[i] = s[len - 1 - i], s[len - 1 - i] = tmp; };
		s[len] = '\0';
		return s;
	}
	void render()
	{
		for (auto it : arr) {
			setfillcolor(RGB(     block_color[it.point][0],     block_color[it.point][1],    block_color[it.point][2]  ));
			RECT r = { it.rx, it.ry, it.rx + WIDE, it.ry + WIDE};
			setbkmode(TRANSPARENT);
			settextcolor(RGB(255, 255, 255));
			fillrectangle(it.rx, it.ry, it.rx + WIDE, it.ry + WIDE);
			drawtext(strnum[it.point], &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		for (auto it : mg) {
			setfillcolor(RGB(block_color[it.point][0], block_color[it.point][1], block_color[it.point][2]));
			RECT r = { it.rx, it.ry, it.rx + WIDE, it.ry + WIDE };
			setbkmode(TRANSPARENT);
			settextcolor(RGB(255, 255, 255));
			fillrectangle(it.rx + (5 - mg_cnt) * WIDE / 10 , it.ry + (5 - mg_cnt) * WIDE / 10, it.rx - (5 - mg_cnt) * WIDE / 10 + WIDE, it.ry -  (5 - mg_cnt) * WIDE / 10 + WIDE);
			drawtext(strnum[it.point], &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		settextcolor(RGB(0, 0, 0));
		outtextxy(0, 405, L"score : ");
		outtextxy(55, 405, trans(score));
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
		int cnt = 0;
		if (mg_cnt < MG_TIME) {
			mg_cnt++;
		}
		else  mg.clear(), mg_cnt = MG_TIME + 1;
		if (move_stop) return mg_cnt <= MG_TIME;
		if (pace == 0) {
			arr.clear();
			buff.clear();
			pace += PACE;
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
			if (cnt == 0) {
				pace = 0;
				move_stop = 1;
				if (mg_cnt == 1) {
					int ret = rand() % 2 + 1;
					while (ret--)
						create_block();
				}
			}
		}
		else if(pace < 100){
			pace += PACE;
			for (int it : buff){
				block* b = &arr[it];
				b->rx = b->rx + de[status][0] * PACE;
				b->ry = b->ry + de[status][1] * PACE;
			}
			return buff.size();
		}
		else if(pace == 100){
			merge();
			mg_cnt = 0;
		}

		return cnt || (mg_cnt <= MG_TIME);
	}
	void merge() {
		mg.clear();
		for (int it : buff) {
			mergeblock(&arr[it], it);
		}
		buff.clear();
		
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				blocks[i][j].set(i * 100, j * 100);
				blocks[i][j].flag = 0;
			}
		}
		pace = 0;
	}

	void mergeblock(block* b, int id) {
		int x1 = b->rx / WIDE, y1 = b->ry / WIDE;
		int x = x1 - de[status][0], y = y1 - de[status][1];
		if (check(x, y, status) == SAME) {
			blocks[x1][y1].point++, blocks[x][y].point = 0, blocks[x1][y1].st = 1, num--;
			score += (1 << (b->point + 1));
			block b1;
			b1.set(b->rx, b->ry);
			b1.point = b->point + 1;
			mg.push_back(b1);
		}
		else {
			blocks[x1][y1].point = b->point, blocks[x][y].point = 0;
		}
	}

	bool checkout() {
		static int out_cnt = 2;
		int ret = 0;
		for (int k = 1; k <= 4; k ++) {
			for (int j = 0; j < 4; j++ ){
				for (int i = 0; i < 4; i++) {
					int res = check(i, j, k);
						ret += res;
				}
			}
		}
		if (ret == 0) out_cnt--;
		if(out_cnt == 0 && ret == 0) return 1;
		return 0;
	}

}Blks;

ExMessage msg;
int logic() {
	ExMessage msg;
	if (Blks.status) flushmessage();
	while (peekmessage(&msg, -1, 1)) {
		flushmessage();
		if (msg.message == WM_KEYDOWN) {
			int dir = -1;
			switch (msg.vkcode) {
			case VK_LEFT : 
				dir = LEFT;
				break;
			case VK_UP : 
				dir = UP;
				break;
			case VK_DOWN: 
				dir = DOWN;
				break;
			case VK_RIGHT:
				dir = RIGHT;
				break;
			default :
				break;
			}
			if (~dir) Blks.status = dir;
		}
	}
	int move_stop = 0;
	if (Blks.status) {
		if (!Blks.move()) {
			move_stop = 1;
			Blks.move_stop = 0;
			Blks.firstmove = 1;
			Blks.mg.clear();
			Blks.arr.clear();
			Blks.status = 0;
			Blks.mg_cnt = 6;
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					Blks.blocks[i][j].st = 0;
					Blks.blocks[i][j].flag = 0;
					Blks.blocks[i][j].set(i * 100, j * 100);
					if (Blks.blocks[i][j].point)
						Blks.arr.push_back(Blks.blocks[i][j]);
					std::cout << Blks.blocks[i][j].point << ' ';
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			if (Blks.num == 16 && Blks.checkout()) return 2;
		}
		
	}
	
	if (move_stop) return 0;
	return 1;
}

void render() {
	cleardevice();
	Blks.render();
}

void game_init() {
	Blks.init(2);
	setbkcolor(RGB(192, 192, 192));
}

int test()
{
	int ret = 0, res = 0;
	for (int k = 1; k <= 4; k++) {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ret = Blks.check(i, j, k);
				if (ret) {
					res += ret;
					std::cout << i << ' ' << j << ' ' << k << std::endl;
				}
			}
		}
	}
	return res;
}

int main()
{
#if 0
	test();
#endif
#if 1
	srand((unsigned int)time(NULL));
	initgraph(400, 430);
	BeginBatchDraw();
	game_init();
	int timer = 2;
	//std::cin >> timer;
	while (runtime)
	{
		int ret = logic();
		render();
		FlushBatchDraw();
		Sleep(timer );
		if (ret == 2) break;
	}
	while (Blks.mg_cnt < MG_TIME) {
		Blks.mg_cnt++; render(); FlushBatchDraw(); Sleep(timer);
	}	
	FlushBatchDraw();
	settextcolor(RGB(0, 0, 0));
	outtextxy(200, 405, L"Game Over");
	FlushBatchDraw();
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			std::cout << Blks.blocks[i][j].point << ',';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << test();
	Sleep(5000);
	EndBatchDraw();
	closegraph();
#endif
	return 0;
}