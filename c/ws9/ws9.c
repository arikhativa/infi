
#include <stddef.h> /* size_t */
#include <assert.h> /* assert() */
#include <string.h> /* strcmp() */
#include <ctype.h> /* isalpha() */
#include <stdio.h> /* printf() */

#define WORD_ALIGNED(ADDRESS) ((size_t)ADDRESS % sizeof(size_t))
#define IS_LITTLE_ENDIAN ((*(short*)&("\1\0")) == 1)
#define WORD sizeof(size_t)

static int gg = 0;

static size_t MakeWordFromByte(char c)
{
	int n = WORD;
	size_t ret = c;

	while (n)
	{
		ret |= ret << 8;
		--n;
	}

	return ret;
}

static size_t *SetByChar(char *runner, char c, size_t n)
{
	while (n)
	{
		*runner = (char)c;
		++runner;
		--n;
	}

	return (size_t*)runner;
}

static char *SetByWord(size_t *runner, size_t c, size_t *n)
{
	while (WORD < *n)
	{
		*runner = c;
		++runner;
		*n -= WORD;
	}

	return (char*)runner;
}

void *Memset(void *s, int c, size_t n)
{
	void *runner = s;
	size_t tmp = 0;

	assert(s);

	tmp = MakeWordFromByte((char)c);
	if (WORD < n && (WORD_ALIGNED(runner)))
	{
		n -= WORD - (WORD_ALIGNED(runner));
		runner = (size_t*)SetByChar((char*)runner, (char)c, WORD - WORD_ALIGNED(runner));
	}
	runner = (char*)SetByWord((size_t*)runner, tmp, &n);
	SetByChar((char*)runner, (char)c, n);

	return s;
}

static size_t CpyByChar(char **dest, const char **src, size_t n)
{
	while (n)
	{
		**dest = **src;
		++*dest;
		++*src;
		--n;
	}

	return n;
}

static size_t CpyByWord(size_t **dest, const size_t **src, size_t n)
{
	while (WORD <= n)
	{
		**dest = **src;
		++*dest;
		++*src;
		n -= sizeof(size_t);
	}

	return n;
}
void *Memcpy(void *dest, const void *src, size_t n)
{
	void *run_dest = dest;
	const void *run_src = src;

	assert(src);
	assert(dest);

	n -= WORD_ALIGNED(run_src);
	CpyByChar((char**)&run_dest, (const char**)&run_src, WORD_ALIGNED(run_src));
	n = CpyByWord((size_t**)&run_dest, (const size_t**)&run_src, n);
	CpyByChar((char**)&run_dest, (const char**)&run_src, n);

	return dest;
}

void *Memmove(void *dest, const void *src, size_t n)
{
	void *run_dest = dest;
	const void *run_src = src;

	assert(src);
	assert(dest);

	if (dest < src)
	{
		return Memcpy(dest, src, n);
	}
	while (WORD <= n)
	{
		*((size_t*)run_dest + (n - 1)) = *((const size_t*)run_src + (n - 1));
		n -= sizeof(size_t);
	}
	while (n)
	{
		*((char*)run_dest + n - 1) = *((const char*)run_src + n - 1);
		--n;
	}

	return dest;
}

int Atoi(const char*str)
{
	int ret = 0;
	int sign = 1;

	if (0 == strcmp("-2147483648", str))
	{
		return -2147483648;
	}
	if ('-' == *str)
	{
		++str;
		sign = -1;
	}
	while (*str)
	{
		ret = (ret * 10) + (*str - '0');
		++str;
	}

	return sign * ret;
}

static int CountDigitsBase(int number, int base)
{
	int count = 0;

	while (number)
	{
		number /= base;
		++count;
	}

	return count;
}

char *ItoaBaseTen(int number, char *str)
{
	int digits = 0;

	assert(str);

	if (!number)
	{
		*str = '0';
	}
	digits = CountDigitsBase(number, 10) - 1;
	if (0 > number)
	{
		str[digits + 1] = ((number % 10) * -1) + '0';
		number = (number / 10) * -1;
		*str = '-';
	}
	while (number)
	{
		str[digits] = (number % 10) + '0';
		number /= 10;
		--digits;
	}

	return str;
}

char *ItoaAnyBase(int number, char *str, int base)
{
	int digits = 0;
	char array_base[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	assert(str);

	if (!number)
	{
		*str = '0';
	}
	digits = CountDigitsBase(number, base) - 1;
	if (0 > number)
	{
		str[digits + 1] = array_base[(number % base) * -1];
		number = (number / base) * -1;
		*str = '-';
	}
	while (number)
	{
		str[digits] = array_base[(number % base)];
		number /= base;
		--digits;
	}

	return str;
}

static void FillLut(char *lut, const char *s)
{
	assert(s);

	while (*s)
	{
		if (isalpha(*s))
		{
			lut[(int)*s] = *s;
		}
		++s;
	}
}

void AppearOnlyFirstTwo(const char *first_arr,const char *sec_arr, const char *third_arr)
{
	char lut1[123] = { 0 };
	char lut3[123] = { 0 };

	assert(first_arr);
	assert(sec_arr);
	assert(third_arr);

	FillLut(lut1, first_arr);
	FillLut(lut3, third_arr);

	while (*sec_arr)
	{
		if (0 != lut1[(int)*sec_arr] && 0 == lut3[(int)*sec_arr])
		{
			printf("%c", *sec_arr);
			lut1[(int)*sec_arr] = 0;
		}
		++sec_arr;
	}
}

int IsLittleEndian()
{
	if (IS_LITTLE_ENDIAN)
	{
		return 1;
	}

	return 0;
}
