
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
	int number = 666;

	printf(" %+d \n", number);
	printf(" %+d \n\n", -number);
	printf(" %-10d \n", number);
    printf(" %010d \n", number);
    printf(" %-#10x \n", number);
    printf(" %#x \n", number);
	return 0;
}
