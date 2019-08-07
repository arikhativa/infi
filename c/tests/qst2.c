
#include <stdio.h>

void	main()
{
	int x = 20, y = 35;

	x = y++ + x++;
	printf("x is %d\ny is %d\n", x, y);
	y = ++y + ++x;
	printf("x is %d\ny is %d\n", x, y);
}
