
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#define _GNU_SOURCE
#define TAB_SIZE_Y 9
#define TAB_SIZE_X 10
#define START_POS 3

#define FOOD_MAX_X (TAB_SIZE_X - 2)
#define FOOD_MAX_Y (TAB_SIZE_Y - 1)

#define UP	-1
#define DOWN	1
#define LEFT	-1
#define RIGHT	1

#define P(num) printf("here %d\n", num);


typedef	struct	point
{
	int x;
	int y;
}		point_t;

typedef struct snake
{
	char (*tab)[TAB_SIZE_X];
	point_t pos;
}	snake_t;


void PrintStartMassege(void)
{
	printf("\n-- Hello\n");
	printf("-- Welcome to game\n");
	printf("--\n");
}

void PrintBoard(char (*tab)[TAB_SIZE_X])
{
	int i = 0;

	for (i = 0; i < TAB_SIZE_Y; ++i)
		printf("%s\n",tab[i]);
}


void SponFood(snake_t *board);

void InitGame(char (*tab)[TAB_SIZE_X])
{
	char enter = 0;
	int i = 0;
	int x = 0;

	PrintStartMassege();

	if (system("stty -icanon -echo") == -1)
	{
		return ;
	}

	while (enter != '\n')
	{
		printf("-- Press Enter to start\n");
		scanf("%c", &enter);
	}

	printf("-- Prossesing...\n");
	sleep(1);

	for (i = 0; i + 1 < TAB_SIZE_X; ++i)
	{
		tab[0][i] = '*';
		tab[TAB_SIZE_Y - 1][i] = '*';
	}

	for (i = 1; i < TAB_SIZE_Y - 1; ++i)
	{
		tab[i][0] = '*';
		tab[i][TAB_SIZE_X - 2] = '*';

		for (x = 1; x < (TAB_SIZE_X - 2); ++x)
		{
			tab[i][x] = ' ';
		}
	}

	tab[START_POS][START_POS] = 'X';
}


int CheckMove(snake_t *board, int y, int x)
{
	int ret = 0;

	if (board->tab[board->pos.y + y][board->pos.x + x] == '*')
		++ret;
	if (board->tab[board->pos.y + y][board->pos.x + x] == 'O')
		SponFood(board);

	return (ret == 0);
}

static void MoveSnake(snake_t *board, char ch)
{
	board->tab[board->pos.y][board->pos.x] = ' ';

	switch (ch)
	{
		case 'w':
			if (CheckMove(board, UP, 0))
				--board->pos.y;
			break ;
		case 's':
			if (CheckMove(board, DOWN, 0))
				++board->pos.y;
			break ;
		case 'a':
			if (CheckMove(board, 0, LEFT))
				--board->pos.x;
			break ;
		case 'd':
			if (CheckMove(board, 0, RIGHT))
				++board->pos.x;
			break ;
	}

	board->tab[board->pos.y][board->pos.x] = 'X';
}


void SponFood(snake_t *board)
{
	point_t food = {rand() % (FOOD_MAX_X), rand() % FOOD_MAX_Y};

//	while (food.x == board->pos.x && food.y == board->pos.y)
	while (board->tab[food.y][food.x] == 'O')
	{
		food.x = rand() % FOOD_MAX_X;
		food.y = rand() % FOOD_MAX_Y;
	printf("-- f.x %d\n", food.x);
	printf("-- f.y %d\n", food.y);

	}


	board->tab[food.y][food.x] = 'O';
}

int RunGame(snake_t *board)
{
	char input = 0;
	char c;
	int stt = 1;
	int secs = 0;
	long keypress = 0;
	int i =0;

	while (stt)
	{
		system("clear");
		PrintBoard(board->tab);

		printf("111111111111 %d", i++);
	        read(0, &input, 1);

//		scanf("%c", &input);
		MoveSnake(board, input);
		fflush(stdin);
	}

	return 0;
}

void EndGame(void)
{
	system("stty icanon echo");
}

int main()
{
	char tab[TAB_SIZE_Y][TAB_SIZE_X] = {0};
	snake_t board = {0};

	board.tab = tab;
	board.pos.x = START_POS;
	board.pos.y = START_POS;


	InitGame(tab);
	SponFood(&board);
	RunGame(&board);

	EndGame();

	return 0;
}
