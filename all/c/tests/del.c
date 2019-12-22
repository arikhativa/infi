
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

class Animal
{
public:

	virtual void Foo()
	{
		printf("-- Animal \n");
	}
};

class Dog : public Animal
{
public:

	virtual void Foo()
	{
		printf("-- Dog \n");
	}
};

class Cat : public Animal
{
public:
	virtual void Foo()
	{
		printf("-- Cat \n");
	}
};

int main()
{
	Dog d;
	Dog* ptr;

	Animal* c = (&d);
	ptr = dynamic_cast<Dog*>(c);

	ptr->Foo();

	return 0;
}
