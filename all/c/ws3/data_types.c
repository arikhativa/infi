
#include <stdio.h> /* printf() */

#define PRINT(SIZE) printf("The size of %s is %lu\n", #SIZE, sizeof(SIZE));

int main()
{
	PRINT(char);
	PRINT(unsigned char);
	printf("\n");
	PRINT(short int);
	PRINT(unsigned short int);
	printf("\n");
	PRINT(int);
	PRINT(unsigned int);
	printf("\n");
	PRINT(long int);
	PRINT(unsigned long int);
	printf("\n");
	PRINT(long long int);
	PRINT(unsigned long long int);
	printf("\n");
	PRINT(float);
	PRINT(double);
	PRINT(long double);

	return 0;
}
