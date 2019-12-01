
#include <stdio.h> /* printf() */

unsigned int NumSetBitsNoLoop(unsigned int num)
{
	num = num - ((num >> 1) & 0x55555555);
	num = (num & 0x33333333) + ((num >> 2) & 0x33333333);

	return (((num + ((num >> 4) & 0x0F0F0F0F)) * 0x01010101) >> 24);
}

int main()
{
	unsigned int num = 15;

	printf("number is: %d\n", num);
	printf("number of set bit: %d\n",  NumSetBitsNoLoop(num));

	return 0;
}
