
#include <stdio.h>

#include "write_print.h"

#define SIZE 4

void MulityArr(int *dest, int *src, int size)
{
    int tmp = 1;
    int i = 0;

    dest[0] = 1;

    for (i = 1; i < size; ++i)
    {
        dest[i] = dest[i - 1] * src[i - 1];
    }

    for (i = size - 2; i + 1; --i)
    {
        tmp = tmp * src[i + 1];
        dest[i] *= tmp;
    }
}

int main()
{
    int arr[] = {2, 4, 1, 7};
    int dest[SIZE] = {0};

    MulityArr(dest, arr, SIZE);
    printf("-- arr ");
    PrintArray(arr, SIZE);
    printf("\n");

    printf("-- dest ");
    PrintArray(dest, SIZE);
    printf("\n");

    return 0;
}
