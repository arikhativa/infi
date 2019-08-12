
#include <stdio.h> /* printf() */
#include <stdlib.h> /* system() */
#define ESCAPE 27

int main()
{
	char pressed = 0;

	if (system("stty -icanon -echo") == -1)
	{
		return -1;
	}
	while (ESCAPE != pressed)
	{
		pressed = getchar();
		switch (pressed)
		{
			case 'A':
				printf("A pressed\n");
				break ;
			case 'T':
				printf("T pressed\n");
				break ;
		}
	}
	if (system("stty icanon echo") == -1)
	{
		return -1;
	}

	return 0;
}
