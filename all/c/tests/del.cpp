
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
#include <iostream>

#include "write_print.h"

class Base0
{
public:
	virtual ~Base0()
	{
		std::cout << "-- Base0 Dtor\n";
	}
private:
};

class Base : public Base0
{
public:
	int m_a;
private:
	~Base()
	{
		std::cout << "-- Base Dtor\n";
	}
};

class Derive : public Base
// class Derive
{
public:
	double m_b;
};

int main()
{
	Base0* p = new Base();

	delete p;

	return 0;
}
