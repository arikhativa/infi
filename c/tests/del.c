
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



int main()
{
	size_t i = 0;

	#pragma omp parallel for

	for (i = 0; i < 100000000; ++i)
	{

	}

	PutStr("-- heyy\n");

	return 0;
}
