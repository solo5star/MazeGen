/**
 * 
 * MazeGen
 * 
 * 미로를 생성한 후 방향키를 이용하여 도착지를 찾아가는 게임입니다.
 * 미로 생성은 DFS-Backtracking 알고리즘을 사용하여 생성하였습니다.
 * 
 * # DFS(Depth First Search)
 *   깊이 우선 탐색이란 뜻으로 주어진 그래프에서,
 *   루트 노드에서 시작해서 다음 분기(branch)로 넘어가기 전에
 *   해당 분기를 완벽하게 탐색하는 방법입니다.
 * 
 * # Backtracking
 *   모든 조합의 수를 살펴보되 유력하지 않은 가지는 배제(또는 가지치기)
 *   하여 경우의 수를 줄이는 방법입니다.
 * 
 * # 미로 생성 알고리즘의 구현
 *   1. 좌표 0, 0부터 깊이 우선 탐색을 진행합니다.
 *      단, 상하좌우는 항상 랜덤으로 선택합니다.
 *   2. 더 이상 진행이 불가할 때 까지 탐색한 후 다른 가지도 탐색합니다.
 *      단, 길이 막혀있으면 배제합니다.
 *   3. 이를 진행불가할 때까지 진행합니다.
 *   
 * 
 * ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 * ■♧■  ■              ■          ■  ■      ■              ■
 * ■  ■  ■  ■■■■■  ■■■  ■  ■  ■  ■  ■■■  ■■■■■
 * ■  ■  ■          ■  ■      ■          ■      ■  ■      ■
 * ■  ■  ■■■■■  ■  ■  ■  ■■■■■■■■■  ■  ■  ■  ■
 * ■                  ■  ■  ■      ■              ■  ■  ■  ■
 * ■■■■■■■■■■■  ■■■■■  ■  ■■■■■■■  ■  ■  ■
 * ■              ■  ■              ■      ■              ■  ■
 * ■  ■■■  ■  ■  ■■■■■■■■■■■  ■■■■■■■■■  ■
 * ■      ■  ■  ■          ■          ■  ■          ■      ■
 * ■■■■■  ■  ■  ■■■  ■■■■■  ■  ■  ■■■  ■  ■■■
 * ■          ■      ■  ■      ■      ■  ■      ■          ■
 * ■  ■■■■■■■■■  ■■■  ■  ■■■  ■■■  ■■■■■  ■
 * ■          ■                  ■      ■      ■      ■      ■
 * ■■■■■  ■  ■■■■■  ■■■  ■  ■■■  ■■■  ■  ■■■
 * ■          ■          ■      ■  ■      ■          ■      ■
 * ■  ■■■■■■■■■  ■  ■  ■■■  ■  ■■■■■■■■■  ■
 * ■      ■      ■      ■  ■          ■  ■  ■              ■
 * ■  ■  ■  ■  ■■■■■■■■■■■■■  ■  ■  ■  ■■■■■
 * ■  ■      ■          ■      ■      ■      ■⊙■      ■  ■
 * ■■■■■■■■■  ■  ■  ■  ■  ■  ■■■■■  ■■■  ■  ■
 * ■              ■  ■  ■  ■      ■  ■      ■  ■  ■  ■  ■
 * ■  ■■■■■  ■  ■  ■  ■  ■■■  ■  ■  ■  ■  ■  ■  ■
 * ■          ■      ■      ■      ■      ■      ■        ♪■
 * ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

// 동적 할당을 사용하지 않으므로 처음부터
// 배열의 크기를 정적으로 할당하기 위한 상수
#define MAX_MAZE_SIZE 100
#define MAX_SCREEN_SIZE 256

// 미로를 생성할 때 얼마나 느리게 생성할 지 설정
#define MAZE_GENERATION_DELAY 2

// 미로의 크기를 설정
#define MAZE_WIDTH 20
#define MAZE_HEIGHT 12

// 각 타일에 대응되는 문자
const char* wall = "■";
const char* road = "  ";
const char* start = "♧";
const char* goal = "♪";
const char* player = "⊙";

// C에는 기본적으로 부울 형이 없기 때문에
// 직접 정의
typedef enum {false, true} bool;

// conio.h의 키 입력에 대한 상수 정의
typedef enum {
	KEY_ARROW = 224,
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_RIGHT = 77,
	KEY_LEFT = 75
} key;

// direction 열거형 정의.
// 길은 사방으로 뚫릴 수도 있기 때문에
// 비트마스킹이 가능하게끔 1비트 간격으로 정의
// 예로, 위 아래로 통로가 뚫려있는 경우
// 0b0011 이 된다.
typedef enum {
	UP = 0b0001,
	DOWN = 0b0010,
	RIGHT = 0b0100,
	LEFT = 0b1000
} direction;

// 좌표에 대한 구조체 정의
typedef struct point {
	int x;
	int y;
} point;

// 미로 생성 시 필요한 멤버들.
// 노드의 방문 여부, 뚫린 방향 등이 필요
typedef struct cell {
	bool visited;
	int directions;
} cell;

// 미로 구조체
// 시작 지점, 골 지점, 플레이어 위치
// 가로길이 세로길이, cell 배열
typedef struct maze {
	point start;
	point goal;

	point player;

	int width;
	int height;

	cell map[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
} maze_t;

// maze는 프로그램에서 한 개만 사용되기 때문에
// 전역 변수로 선언해둔다
maze_t maze;

// 콘솔 드로잉에 사용할 구조체
// 2 byte character를 사용할 것이기 때문에
// \0 까지 포함한 char[3] 배열과,
// 좌표 정보 및 다음 heartbeat때 다시 드로잉
// 해야하는지 여부 shouldUpdate가 있다
typedef struct pixel {
	char ch[3];
	int x;
	int y;
	bool shouldUpdate;
} pixel;

// 화면 출력 관련 정보를 담은 구조체
// 다음 heartbeat때 드로잉이 필요한지
// 가로 길이, 세로 길이
// 픽셀 정보 등을 담고있다
typedef struct screen {
	bool shouldUpdate;
	int width;
	int height;

	pixel pixels[MAX_SCREEN_SIZE][MAX_SCREEN_SIZE];
} screen_t;

// 콘솔은 하나만 띄울 것이기 때문에 전역 변수로 선언
screen_t screen;

// 콘솔의 크기를 설정하는 메소드 정의
void setConsoleSize(int width, int height) {
	// 폰트 크기
	const int FONT_SIZE = 15;

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, (width + 12) * FONT_SIZE, (height + 5) * FONT_SIZE, TRUE);
}

// 화면의 크기 설정 및 관련 정보 initialize
void initializeScreen(int width, int height) {
	screen.width = width;
	screen.height = height;

	for (int y = 0; y < screen.height; y++) {
		for (int x = 0; x < screen.width; x++) {
			screen.pixels[y][x].x = x;
			screen.pixels[y][x].y = y;
		}
	}

	setConsoleSize(width, height);
}

// 미로의 크기를 지정
void initializeMaze(int width, int height) {
	maze.width = width;
	maze.height = height;

	// 랜덤 함수의 시드 설정
	srand(time(NULL));
}

// 미로를 생성할 때 다음에 방문할 노드를 랜덤으로
// 만들어주는 메서드
void shuffleDirections(direction* randomDirections) {
	randomDirections[0] = UP;
	randomDirections[1] = RIGHT;
	randomDirections[2] = LEFT;
	randomDirections[3] = DOWN;

	for (int i = 0; i < 4; i++) {
		int j = rand() % 4;

		direction tmp = randomDirections[i];
		randomDirections[i] = randomDirections[j];
		randomDirections[j] = tmp;
	}
}

// 콘솔에서 드로잉할 위치를 지정해주는 메서드
void gotoPixel(int x, int y) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { x * 2, y });
}

// 화면 상에 업데이트해야 할 pixel이 있는지 확인하고
// 해당되는 픽셀만 업데이트하는 메서드
void heartbeatScreen() {
	if (!screen.shouldUpdate) return;

	for (int y = 0; y < screen.height; y++) {
		for (int x = 0; x < screen.width; x++) {
			if (screen.pixels[y][x].shouldUpdate) {
				gotoPixel(x, y);
				printf("%s", screen.pixels[y][x].ch);
			}
		}
	}

	gotoPixel(screen.width, screen.height);
}

// 특정 좌표에 printf로 출력해주는 메서드
// 다음 heartbeat때 드로잉 된다
void drawAt(point p, const char* ch) {
	pixel* pixel = &screen.pixels[p.y][p.x];
	// No changes
	if (strcmp(pixel->ch, ch) == 0) return;

	screen.shouldUpdate = true;

	strcpy_s(pixel->ch, 3, ch);
	pixel->shouldUpdate = true;
}

// drawAt과 동일한 동작을 하는 메서드지만
// 다음 heartbeat를 기다리지 않고 즉시 드로잉한다
void drawAtInstantly(point p, const char* ch) {
	strcpy_s(screen.pixels[p.y][p.x].ch, 3, ch);

	gotoPixel(p.x, p.y);
	printf("%s", screen.pixels[p.y][p.x].ch);

	gotoPixel(screen.width, screen.height);
}

// direction의 반대 방향을 반환해주는 메서드
direction opposite(direction direction) {
	switch (direction) {
	case UP: return DOWN;
	case DOWN: return UP;
	case RIGHT: return LEFT;
	case LEFT: return RIGHT;
	}
	return 0;
}

// 좌표의 특정 방향에 있는 새로운 좌표를 반환
point getPointAt(point origin, direction direction) {
	int x = 0, y = 0;

	switch (direction) {
	case UP: y = 1; break;
	case DOWN: y = -1; break;
	case RIGHT: x = 1; break;
	case LEFT: x = -1; break;
	}
	return (point) {
		.x = origin.x + x,
		.y = origin.y + y
	};
}

// 미로를 생성할 때 한번 방문했는지, 미로 밖을 벗어나진
// 않는지 등을 체크하여 bool로 반환
bool canGoThrough(point origin, direction direction) {
	point next = getPointAt(origin, direction);

	if (
		next.x < 0
		|| next.y < 0
		|| next.x >= maze.width
		|| next.y >= maze.height
	) return false;

	if (maze.map[next.y][next.x].visited) return false;

	return true;
}

// 길을 뚫는 메서드
void goThrough(point origin, direction direction) {
	point next = getPointAt(origin, direction);

	maze.map[origin.y][origin.x].visited = true;
	maze.map[origin.y][origin.x].directions |= direction;

	maze.map[next.y][next.x].visited = true;
	maze.map[next.y][next.x].directions |= opposite(direction);
}

// 미로 밑에 메세지를 띄워주는 메서드
// 최대 30byte까지 출력 가능하도록 지정되어 있다
void drawMessage(const char* message) {
	const int maximumLength = 30;
	int len = strlen(message);
	char buffer[3] = { 0, };
	for (int i = 0; i < maximumLength; i++) {
		buffer[0] = i < len ? message[i] : ' ';
		i += 1;
		buffer[1] = i < len ? message[i] : ' ';

		drawAtInstantly((point) { (i - 1) / 2, maze.height * 2 + 2 }, buffer);
	}
}

// 미로의 cell 구조체를 실제 화면에 출력해주는 메서드
// 언더바를 붙인 이유는 instantly 매개 변수를 drawCell, drawCellInstantly로
// Wrapping하기 위함임
void _drawCell(point p, bool instantly) {
	cell* cell = &maze.map[p.y][p.x];

	int x = p.x * 2 + 1;
	int y = p.y * 2 + 1;

	// instantly 매개변수에 따라 즉시 그릴 것인지, 다음 heartbeat에 그릴 것인지
	void (*drawMethod)(point, const char*) = instantly ? drawAtInstantly : drawAt;

	bool isStart = maze.start.x == p.x && maze.start.y == p.y;
	bool isGoal = maze.goal.x == p.x && maze.goal.y == p.y;
	bool isPlayer = maze.player.x == p.x && maze.player.y == p.y;

	drawMethod((point) { x, y }, cell->directions ? (
		isPlayer ? player
		: isGoal ? goal
		: isStart ? start
		: road) : wall);

	drawMethod((point) { x + 1, y + 1 }, wall);
	// drawMethod((point) { x - 1, y }, cell->directions & LEFT ? road : wall);
	// drawMethod((point) { x, y - 1 }, cell->directions & DOWN ? road : wall);
	drawMethod((point) { x + 1, y }, cell->directions & RIGHT ? road : wall);
	drawMethod((point) { x, y + 1 }, cell->directions & UP ? road : wall);
}

// _drawCell(p, false) wrapping 메서드
void drawCell(point p) {
	_drawCell(p, false);
}

// _drawCell(p, true) wrapping 메서드
void drawCellInstantly(point p) {
	_drawCell(p, true);
}

// 미로 전체를 그리는 메서드
void drawMaze() {
	for (int x = 0; x < maze.width * 2 + 1; x++) {
		drawAt((point) { x, 0 }, wall);
	}

	for (int y = 0; y < maze.height * 2 + 1; y++) {
		drawAt((point) { 0, y }, wall);
	}

	for (int y = 0; y < maze.height; y++) {
		for (int x = 0; x < maze.width; x++) {
			drawCell((point) { x, y });
		}
	}
}

// 다음에 방문할 노드 방향을 랜덤하게 저장해두는 배열
direction randomDirections[4];

// backtracking을 구현한 메서드
// DFS로 구현되었기 때문에 재귀호출된다
void mazeGeneration_backtracking(point origin) {
	shuffleDirections(randomDirections);
	point next;
	direction direction;

	for (int i = 0; i < 4; i++) {
		direction = randomDirections[i];

		if (canGoThrough(origin, direction)) {
			goThrough(origin, direction);

			Sleep(MAZE_GENERATION_DELAY);

			next = getPointAt(origin, direction);

			drawCellInstantly(origin);
			drawCellInstantly(next);

			mazeGeneration_backtracking(next);
		}
	}
}

// 종합적으로 미로를 생성해주는 메서드
void generateMaze(point start, point goal) {
	maze.start = start;
	maze.goal = goal;
	maze.player = start;

	mazeGeneration_backtracking(start);

	drawMessage("미로를 탈출하세요.");
}

// 특정 방향으로 지나갈 수 있는지 여부를 알려주는 메서드
// 플레이어가 가려는 방향에 길이 막혀있는지 판단하는 데에
// 쓰인다
bool canMoveTo(point origin, direction direction) {
	if (!(maze.map[origin.y][origin.x].directions & direction)) return false;

	point next = getPointAt(origin, direction);
	if (!(maze.map[next.y][next.x].directions & opposite(direction))) return false;

	return true;
}

// 플레이어를 특정 방향으로 이동하는 메서드
void movePlayer(direction direction) {
	point origin = maze.player;
	if (!canMoveTo(origin, direction)) return;

	maze.player = getPointAt(origin, direction);

	if (maze.player.x == maze.goal.x && maze.player.y == maze.goal.y) drawMessage("클리어!!");

	drawCellInstantly(origin);
	drawCellInstantly(maze.player);
}

const char* filename = "maze.bin";

bool saveMaze() {
	FILE* fp;

	if (fopen_s(&fp, filename, "wb") != 0) {
		drawMessage("저장하는  동안  오류가  발생하였습니다.");
		return false;
	}

	// Suppress Warning C6387
	if (fp == NULL) return false;

	fwrite(&maze, sizeof(maze_t), 1, fp);
	fclose(fp);

	drawMessage("성공적으로  저장하였습니다.");

	return true;
}

bool loadMaze() {
	FILE* fp;

	if (fopen_s(&fp, filename, "rb") != 0) {
		drawMessage("저장된  파일이  없는 것 같습니다.");
		return false;
	}

	// Suppress Warning C6387
	if (fp == NULL) return false;

	fread_s(&maze, sizeof(maze_t), sizeof(maze_t), 1, fp);
	fclose(fp);

	drawMessage("성공적으로  불러왔습니다.");

	return true;
}

void createNewMaze(int width, int height) {
	// 콘솔 initialize
	initializeScreen(width * 2 + 10, height * 2 + 10);
	// 미로 initialize
	initializeMaze(width, height);

	drawMaze();
	heartbeatScreen();

	generateMaze((point) { 0, 0 }, (point) { width - 1, height - 1 });
}

// 키 입력을 핸들링하는 메서드
void handleKeyInput() {
	if (_kbhit()) {
		int key = _getch();

		// 방향키인 경우
		if (key == KEY_ARROW) {
			do {
				// 한번 더 getch를 호출해야
				// 실제 방향을 알 수 있음
				key = _getch();
			} while (key == KEY_ARROW);

			switch (key) {
			case KEY_UP: movePlayer(DOWN); break;
			case KEY_DOWN: movePlayer(UP); break;
			case KEY_LEFT: movePlayer(LEFT); break;
			case KEY_RIGHT: movePlayer(RIGHT); break;
			}

			while (_kbhit()) _getch();

			return;
		}
		else if (key == 's') {
			saveMaze();
		}
		else if (key == 'l') {
			loadMaze();
		}
		}
	}
}

int main() {
	int width = MAZE_WIDTH;
	int height = MAZE_HEIGHT;

	// 콘솔 initialize
	initializeScreen(width * 2 + 10, height * 2 + 10);
	// 미로 initialize
	initializeMaze(width, height);

	// 처음엔 아무것도 없는 상태를 그린다
	// drawMaze();
	// heartbeatScreen();

	// 미로 파일 로드에 실패 시 새로 하나 생성
	if (!loadMaze()) {
		drawMessage("");
		createNewMaze(width, height);
	}

	drawMaze();
	heartbeatScreen();

	// 미로가 생성된 후에는 키 입력만 받는다
	while (true) {
		handleKeyInput();
	}
}
