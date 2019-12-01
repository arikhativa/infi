
#include <stdio.h>

void PrintTab(char **tab, int size);
static void Swap(char *a, char *b);

void FlipTable(char **tab, int size)
{
	char x = 0;
	char y = 0;
	char tmp_x = 0;
	char tmp_y = 0;
	char n = 0;
	char i = 0;
	char max = (char)size - 1;
	
	while (y < size / 2)
	{
		while (x < max - n)
		{
			tmp_x = x;
			tmp_y = y;
			for (i = 0; i < 3; ++i)
			{
				Swap(&tab[tmp_y][tmp_x], &tab[(max - tmp_x)][tmp_y]);
				tmp_x = max - tmp_x;
				Swap(&tmp_x, &tmp_y);
			}
			++x;
		}
		x = y + 1;
		++y;
		++n;
	}
}

static void Swap(char *a, char *b)
{
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

void PrintTab(char **tab, int size)
{
	int i = 0;

	for (i = 0; i < size; ++i)
		printf("%s\n",tab[i]);
	printf("\n");
}

int main()
{
	int size = 5;
	char *tab[5];
	char line1[] = "abcde";
	char line2[] = "fghij";
	char line3[] = "klmno";
	char line4[] = "pqrst";
	char line5[] = "uvwxy";

	tab[0] = line1;
	tab[1] = line2;
	tab[2] = line3;
	tab[3] = line4;
	tab[4] = line5;

	PrintTab(tab, size);

	FlipTable(tab, size);

	PrintTab(tab, size);

	return 0;

}

