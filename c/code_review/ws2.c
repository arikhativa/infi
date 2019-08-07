#include <stdio.h> /*for printf*/
#include <stdlib.h> /*for malloc*/
#include <string.h> /*for strken*/
#include <ctype.h> /*for tolower and isspace*/
#include <assert.h>/*for assert*/


int IsPalindrome(const char *str)
{
    char *end_ptr;
    
    assert(str);
    
    end_ptr = (char *)str + strlen(str) - 1;
    
    while (str < end_ptr)
    {
        if (*str != *end_ptr)
        {
            return 1;
        } 
        ++str;
        --end_ptr;      
        }
    return 0;
}

void SevenBoom(int start, int end)
{
    while (start < end)
    {
        if ((start % 7 == 0) || (7 == start % 10))
        {
            printf("%s\n", "BOOM");
        }
        else
        {
            printf("%d\n", start);
        }
        ++start;
    }
}

void SwapPointers(int **ptr1, int **ptr2)
{
    int *tmp;
    
    assert(ptr1);
    assert(ptr2);
    
    tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

void RmSpaces(char *str)
{
    char *runner = str;
    char *follower;
    
    assert(str);
    
    if(isspace(*runner) != 0)
    {
        while(*runner == ' ')
        {
            ++runner;
        }
    
        while(*str && *runner != ' ')
        {
            *str = *runner;
            ++str;
            ++runner;
        }    
    }
    
    follower = runner + 1;
    while (*str)
    {
        if(*runner == ' ' && *follower == ' ')
        {
            ++runner;
            ++follower;
        }
        else
        {
            *str = *(runner);
            ++str;
            ++runner;
            ++follower;
        }
    }
    
    *str = '\0';
}


void RevStr(char *str)
{
    char *end = str + (strlen(str) - 1);
    char tmp;
    
    assert(str);
    
    while(str < end)
    {
        tmp = *end;
        *end = *str;
        *str = tmp;
        ++str;
        --end;
    }
}


 
char *StringPlusString(char *s1, char *s2)
{
    char carry = 0;
    char *small = s1;
    char *big = s2;
    char *result = NULL;
    char *tmp;
    
    assert(s1);
    assert(s2);
    
    RevStr(s1);
    RevStr(s2);
    
    if(strlen(s1) > strlen(s2))
    {
        small = s2;
        big = s1;
    }
    
    result = (char *)malloc(sizeof(char) * (strlen(big) + 2));
    tmp = result;
    
    while(*small)
    {
        *result = (*small + *big + carry - 96) % 10 + 48;
         carry = (*small + *big + carry - 96) / 10;
         ++big;
         ++small;
         ++result;  
    }
    
    while(*big)
    {
        *result = (*big + carry - 48) % 10 + 48;
        carry = (*big + carry - 48) / 10;
        ++result;
        ++big;
    }
    
    if(carry == 1)
    {
        *result = '1';
    }
    
    RevStr(tmp);
    
    return tmp;
}








































