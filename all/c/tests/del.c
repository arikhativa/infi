
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

class X
{
private:
	int m_a;
public:
	static void Foo(X* x);
};

void X::Foo(X* x)
{
	x->m_a = 0;
}

int main()
{
	X x;

	X::Foo(&x);

	return 0;
}
