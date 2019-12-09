#include <iostream>

#include "slist.hpp"
#include "check.h"

using namespace hrd11;


int main()
{
    SList ls;

    CF("SLIST", START);

    ls.PushFront(2);
    CF("", 2 == ls.Head()); 
    CF("", 1 == ls.Size());
    ls.PopFront(); 
    CF("", 0 == ls.Size());
    CF("", -1 == ls.Head());    
    ls.PopFront(); 
    ls.PushFront(5);
    ls.PushFront(4);
    CF("", 2 == ls.Size());
    CF("", 4 == ls.Head());
    CF("", END);

    return 0;
}



void CF(const char *func_name, int arg)
{	
	static int flag = 1;
	static int test_count = 0;
	
	if(START == arg)
	{
		printf(BYELLOW "%s:   " RESET, func_name);
		arg = ONE;
		test_count = 0;
		flag = 1;
		return;
	}	
	flag *= arg;
	++test_count; 
	if(!arg)
	{
		printf("\ntest%d:  " RED "FAIL" RESET, test_count);
		return;
	}
	if(ALL_WORKS == flag * arg)
	{
		printf(BGREEN "PASS" RESET);
	}
	if(arg == END)
	{
		printf("\n");
	}
}