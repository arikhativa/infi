

/*
	The Game works but...
	there sould be a better system for the thread that get the input.
	there is a leek becuse of him.
	and generally. realy messy code. it a nice try and there is a game but
	it can be mach mach bettr.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>

#define _GNU_SOURCE
#define TAB_SIZE_X 8
#define TAB_SIZE_Y 8
#define START_POS 3

#define EXIT 999

#define FOOD_MAX_X (TAB_SIZE_X - 3)
#define FOOD_MAX_Y (TAB_SIZE_Y - 2)

#define CAN_GET	1
#define BLOCK	10

#define UP		-1
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
	point_t *pos;
	char snake_len;
	char dir;
	char get_input;
	int points;
	pthread_t th_io;
}	snake_t;


void PrintStartMassege(void)
{
	printf("\n-- Hello\n");
	printf("-- Welcome to game\n");
	printf("--\n");
}

void PrintBoard(char (*tab)[TAB_SIZE_X], int points)
{
	int i = 0;

	printf("%s	%d\n",tab[i], points);
	for (i = 1; i < TAB_SIZE_Y; ++i)
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
//	sleep(1);

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
	tab[4][2] = 'O';

	tab[START_POS][START_POS] = 'X';
}


int CheckMove(snake_t *board, int y, int x)
{
	int ret = 0;
	void *tmp;

	if (board->tab[board->pos[0].y + y][board->pos[0].x + x] == '*'
		|| board->tab[board->pos[0].y + y][board->pos[0].x + x] == 'X'
		|| board->tab[board->pos[0].y + y][board->pos[0].x + x] == 'x')
	{
		board->dir = 27;

		return EXIT;
	}
	if (board->tab[board->pos[0].y + y][board->pos[0].x + x] == 'O')
		SponFood(board);

	return (ret == 0);
}

static int MoveSnake(snake_t *board)
{
	char i = 0;
	int ret = 0;
	point_t tmp;

	tmp.y = board->pos[0].y;
	tmp.x = board->pos[0].x;


	switch (board->dir)
	{
		case 'w':
			if (ret = CheckMove(board, UP, 0))
				--tmp.y;
			break ;
		case 's':
			if (ret = CheckMove(board, DOWN, 0))
				++tmp.y;
			break ;
		case 'a':
			if (ret = CheckMove(board, 0, LEFT))
				--tmp.x;
			break ;
		case 'd':
			if (ret = CheckMove(board, 0, RIGHT))
				++tmp.x;
			break ;
	}
	if (ret == EXIT)
	{
		return ret;
	}
	for (i = 0; i < board->snake_len; ++i)
	{
		board->tab[board->pos[i].y][board->pos[i].x] = ' ';
	}

	for (i = board->snake_len - 1; i ; --i)
	{
		board->pos[i].y = board->pos[i - 1].y;
		board->pos[i].x = board->pos[i - 1].x;
	}
	board->pos[0].y = tmp.y;
	board->pos[0].x = tmp.x;

	board->tab[board->pos[0].y][board->pos[0].x] = 'X';
	for (i = 1; i < board->snake_len; ++i)
	{
		board->tab[board->pos[i].y][board->pos[i].x] = 'x';
	}

	return 0;
}


void SponFood(snake_t *board)
{
	point_t food = {0};

	do
	{
		food.x = rand() % FOOD_MAX_X + 1;
		food.y = rand() % FOOD_MAX_Y + 1;

		/*if (food.y == 0)
			++food.y;*/

		/*if (board->tab[food.y][food.x] == '*')
		{
			continue ;
		}*/
	} while (board->tab[food.y][food.x] == 'O'
	 || board->tab[food.y][food.x] == 'x'
	 || board->tab[food.y][food.x] == 'X');

	board->tab[food.y][food.x] = 'O';
	board->pos[board->snake_len].x = board->pos[board->snake_len - 1].x;
	board->pos[board->snake_len].y = board->pos[board->snake_len - 1].y;
	++board->snake_len;
}

int RunGame(snake_t *board)
{
	int stt = 1;

	while (27 != board->dir && EXIT != stt)
	{
		system("clear");
		PrintBoard(board->tab, board->snake_len);
		board->get_input = CAN_GET;
		system("sleep 0.2");
		stt = MoveSnake(board);

	}

	return 0;
}

void EndGame(pthread_t th_io)
{
	void *tmp;

	printf("-- fail..\n");

	system("stty icanon echo");

	pthread_join(th_io, (void**)&tmp);
}

int CheakInput(snake_t *board, char ch)
{
	if (board->get_input == BLOCK)
		return 0;

	if (board->dir == 'w' && ch != 's')
		return 1;
	if (board->dir == 's' && ch != 'w')
		return 1;
	if (board->dir == 'd' && ch != 'a')
		return 1;
	if (board->dir == 'a' && ch != 'd')
		return 1;

	return 0;
}

void *GetInput(void *data)
{
	snake_t *board = data;
	char ch = 0;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	sleep(1);
	while (27 != board->dir)
	{
		scanf("%c", &ch);
		if (CheakInput(board, ch))
		{
			board->dir = ch;
			board->get_input = BLOCK;
		}
	}

	return NULL;
}

int main()
{
	char tab[TAB_SIZE_Y][TAB_SIZE_X] = {0};
	snake_t board = {0};
	point_t pos[60] = {0};
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	board.tab = tab;
	board.pos = pos;
	board.pos[0].x = START_POS;
	board.pos[0].y = START_POS;
	board.snake_len = 1;
	board.dir = 'd';
	board.get_input = CAN_GET;

	if (pthread_create(&(board.th_io), &attr, &GetInput, &board))
    {
        return -1;
    }


	InitGame(tab);

	RunGame(&board);

	EndGame(board.th_io);

	return 0;
}
