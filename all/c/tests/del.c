
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


class
int main()
{
	StackMin stack;

	stack.Push(1);
	stack.Push(2);
	stack.Push(3);
	stack.Push(-5);

	stack.Pop();
	stack.Pop();

	printf("-- min %d\n", stack.MinPeek());

	return 0;
}
