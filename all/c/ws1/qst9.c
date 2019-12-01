
#include <stdio.h>
#include <stdlib.h>

void	expre_1()
{

	int	a = 77;
	int	*b = &a;
	int	*c = b;

	*c = 5;

	printf("-- expression 1 --\n\n");
	printf("a is %d\n", a);
}

void	expre_2()
{
	int 	a = 0;

	a = (a = 0) ? 0 : 1;

	printf("-- expression 2 --\n\n");
	printf("a is == %d\n", a);
}

void	expre_3()
{
	int	a = 1;
	int	b = 0;

	while (a-- >= 0)
		b++;

	printf("-- expression 3 --\n\n");
	printf("b == %d\n", b);

}

void	expre_4()
{
	char	str[7] = {'h', 'e', 'y', '\0', 't', 'i', 'm'};

	printf("-- expression 4 --\n\n");
	printf("%s\n", str);

}

void	main(int ac, char **av)
{
	int	n;

	if (ac != 2)
	{
		printf("gimmi one arg\n\n");
	}
	else
	{
		n = atoi(av[1]);
		if (n == 1)
			expre_1();
		if (n == 2)
			expre_2();
		if (n == 3)
			expre_3();
		if (n == 4)
			expre_4();
	}
}
