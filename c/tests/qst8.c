
#include <stdio.h>

long l = 8;
long *foo(){ return &l; }

void	main()
{

	*(foo()) = 5;
	printf("%ld\n", l);
}
