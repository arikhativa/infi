
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
		if (pressed == 'A' || pressed == 'T')
		{
			printf("%c pressed\n", pressed);
		}
	}
	if (system("stty icanon echo") == -1)
	{
		return -1;
	}

	return 0;
}
