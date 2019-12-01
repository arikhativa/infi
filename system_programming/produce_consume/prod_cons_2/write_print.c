
#include <unistd.h>

void PutChar(char ch)
{
    write(1, &ch, 1);
}

void PutStr(char *str)
{
    while (*str)
    {
        PutChar(*str);
        ++str;
    }
}

void PutNum(ssize_t num)
{
    if (0 > num)
    {
        PutChar('-');
        num = -num;
    }
    if (num / 10)
        PutNum(num / 10);
    PutChar((num % 10) + '0');
}

void PrintArray(int *arr, size_t size)
{
    size_t i = 0;

    PutNum(arr[i]);

    for (i = 1; i < size; ++i)
    {
        PutStr(", ");
        PutNum(arr[i]);
    }

    PutChar('\n');
}

/*int main()
{
    ssize_t num = -45567;

    PutNum(num);

    return 0;
}*/
