
#include <stdio.h>
#include "g.h"


void Foo();

int main()
{
	printf("%d\n", g_s);
	Foo();

	return 0;
}
