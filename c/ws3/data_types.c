
#include <stdio.h> /* printf() */
#include <stdlib.h> /* malloc() */
#include <ctype.h> /* tolower */
#include <assert.h> /* assert */


int main()
{
	char a = 1;
	unsigned char b = 1;

	short int c = 1;
	unsigned short int d = 1;
	int e = 1;
	unsigned int f = 1;
	long int g = 1;
	unsigned long int h = 1;
	long long int i = 1;
	unsigned long long int j = 1;

	float k = 1;
	double l = 1;
	long double m = 1;



	printf("\n-- The size of char is %ld byte\n", sizeof(a));
	printf("-- The size of unsigned char is %ld byte\n\n", sizeof(b));

	printf("-- The size of short int is %ld byte\n", sizeof(c));
	printf("-- The size of unsigned short int is %ld byte\n\n", sizeof(d));
	printf("-- The size of int is %ld byte\n", sizeof(e));
	printf("-- The size of unsigned int is %ld byte\n\n", sizeof(f));
	printf("-- The size of long int is %ld byte\n", sizeof(g));
	printf("-- The size of unsigned long int is %ld byte\n\n", sizeof(h));
	printf("-- The size of long long int is %ld byte\n", sizeof(i));
	printf("-- The size of unsigned long long int is %ld byte\n\n", sizeof(j));

	printf("-- The size of float is %ld byte\n", sizeof(k));
	printf("-- The size of double char is %ld byte\n", sizeof(l));
	printf("-- The size of long double char is %ld byte\n", sizeof(m));
	return 0;
}
