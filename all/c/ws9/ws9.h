#ifndef __WS9__
#define __WS9__
#include <stddef.h> /* size_t */


void *Memset(void *s, int c, size_t n);

void *Memcpy(void *dest, const void *src, size_t n);

void *Memmove(void *dest, const void *src, size_t n);

int Atoi(const char *str);

char *ItoaBaseTen(int num, char *str);

char *ItoaAnyBase (int num, char *str, int base);

void AppearOnlyFirstTwo(const char *first_arr,const char *sec_arr, const char *third_arr);

int IsLittleEndian();

#endif /* __WS9__ */
