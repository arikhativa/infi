
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point
{
	int x;
	int y;
} point_t;

int IsPointInside(point_t *shape, size_t size, point_t p)
{
	int i = 0;
	int up = 0;
	int down = 0;
	int left = 0;
	int right = 0;

	for (; i <size; ++i)
	{
		if (p.x == shape[i].x)
		{
			if (p.y > shape[i].y)
				++up;
			if (p.y < shape[i].y)
				++down;
		}
		if (p.y == shape[i].y)
		{
			if (p.x > shape[i].x)
				++left;
			if (p.x < shape[i].x)
				++right;
		}
	}

	return (up && down && left && right);
}

int main()
{
	int size = 10;
	int tab[5][5] = {0};
	point_t p1 = {0, 0};
	point_t p2 = {3, 1};
	point_t p3 = {1, 3};

	point_t shape[] = {
		{2, 0},
		{3, 0},
		{4, 1},
		{4, 2},
		{3, 2},
		{2, 3},
		{1, 3},
		{1, 4},
		{0, 2},
		{1, 1}
	};

	printf("-- %d\n", IsPointInside(shape, size, p1));
	printf("-- %d\n", IsPointInside(shape, size, p2));
	printf("-- %d\n", IsPointInside(shape, size, p3));

	return 0;
}

