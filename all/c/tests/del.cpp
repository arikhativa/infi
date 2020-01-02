
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#include "write_print.h"

class Base
{
public:
	int m_a;
};

class Derive : public Base
{
public:
	double m_b;
};

int main()
{
	Base b1;
	Derive d2;

	b1 = d2;

	try
	{
		throw Derive();
	}
	// catch (Derive)
	// {
	// 	printf("-- catch Derive\n");
	// }
	catch (Base)
	{
		printf("-- catch Base\n");
	}

	return 0;
}
