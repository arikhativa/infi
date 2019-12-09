#ifndef __CHECK_H__
#define __CHECK_H__

#include <stdio.h> /*printf*/

enum statuscheck 
{
	START = -10,
	ONE = 1,
	END = 10,
	ALL_WORKS = 100
};

void CF(const char *func_name, int Bool);


#endif /* __CHECK_H__ */


#ifndef __COLOR_H__
#define __COLOR_H__

#define RED "\x1b[31m"

#define GREEN "\x1b[32m"

#define YELLOW "\x1b[33m"

#define BLUE "\x1b[34m"

#define MAGENTA "\x1b[35m"

#define CYAN "\x1b[36m"

#define GRAY "\x1b[2m"

#define BYELLOW "\033[01;33m"

#define BGREEN "\033[1;32m"

#define RESET "\x1b[0m"

#endif /* __COLOR_H__ */
                                    
