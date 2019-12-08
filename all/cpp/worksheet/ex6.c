
#include <stdio.h>
#include <stdlib.h>


int main()
{
    int *arr = new int(5);

    printf("-- *arr - %d\n", *arr);

    free(arr);

    arr = (int*)malloc(sizeof(int) * 2);

    arr[0] = 4;
    arr[1] = 6;
    printf("-- *arr - %d\n", *arr);
    printf("-- *arr+1 - %d\n", *(arr+1));

    delete[] arr;
    arr = NULL;
    delete arr;


    return 0;
}
