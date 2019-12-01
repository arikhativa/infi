#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "bitarray.h"
#include "color.h"

static int t = 1;

void PrintBitS(size_t number, int digits)
{
	size_t one_bit = 1;
	--digits;
	while (digits + 1)
	{
		(number & (one_bit << (digits))) ? printf("1") : printf("0");
		--digits;
	}
	printf("\n");
}

void PrintBit(size_t number, int digits)
{
	size_t one_bit = 1;
	--digits;
	while (digits + 1)
	{
		(number & (one_bit << (digits))) ? printf("1") : printf("0");
		--digits;
		if ((digits + 1) != 0 && (digits+1) % 8 == 0)
		{
			printf("-");
		}
	}
	printf("\n");
}

void PrintPass()
{
	printf("-- Test"GRAY" %d"RESET GREEN" - Pass :)\n"RESET, t++);
}

void PrintFail(char *str)
{
	printf("-- Test"GRAY" %d"RESET RED" - Fail :( "RESET, t++);
	printf("%s\n", str);
}

void TestSetOn(int print)
{
	bit_array_t old = 0;
	bit_array_t arr = 0;
	bit_array_t one = 1;

	printf(CYAN"\n-- SetOn():\n\n"RESET);
	if (print)
	{
		printf("-- old_arr is:	");
		PrintBit(old, 64);
		arr = SetOn(old, 32);
		printf("-- arr is:	");
		PrintBit(arr, 64);
	}
	else
		arr = SetOn(old, 32);
	if (arr & one << 32)
		PrintPass();
	else
		PrintFail(" -- Bit 32 IS set OFF.");
	if ((old ^ arr) == one << 32)
		PrintPass();
	else
		PrintFail(" -- Other bits have changed.");
}

void TestSetOff(int print)
{
	bit_array_t old = ~0;
	bit_array_t arr = 0;
	bit_array_t one = 1;

	printf(CYAN"\n-- SetOff():\n\n"RESET);
	if (print)
	{
		printf("-- old_arr is:	");
		PrintBit(old, 64);
		arr = SetOff(old, 32);
		printf("-- arr is:	");
		PrintBit(arr, 64);
	}
	else
		arr = SetOff(old, 32);
	if (!(arr & one << 32))
		PrintPass();
	else
		PrintFail(" -- Bit 33 IS set ON.");
	if ((old ^ arr) == one << 32)
		PrintPass();
	else
		PrintFail(" -- Other bits have changed.");
}

void TestIsOn(int print)
{
	bit_array_t one = 1;
	bit_array_t arr = 0;
	int index = 63;

	arr = (one << 32);

	printf(CYAN"\n-- IsOn():\n\n"RESET);
	if (print)
	{
		printf("-- arr is:	");
		PrintBit(arr, 64);
	}
	if (IsOn(arr, 32))
		PrintPass();
	else
		PrintFail(" -- Returned 0 when should have return 1.");
	while (index)
	{
		if ((IsOn(arr, index)))
		{
			PrintFail(" -- Returned 1 when should have return 0.");
			break ;
		}
		--index;
		if (index == 32)
			--index;
	}
	if (!index)
		PrintPass();
}

void TestIsOff(int print)
{
	bit_array_t one = 1;
	bit_array_t arr = 0;
	int index = 63;

	arr = ~(one << 32);

	printf(CYAN"\n-- IsOff():\n\n"RESET);
	if (print)
	{
		printf("-- arr is:	");
		PrintBit(arr, 64);
	}
	if (IsOff(arr, 32))
		PrintPass();
	else
		PrintFail(" -- Returned 0 when should have return 1.");
	while (index)
	{
		if (!(IsOff(arr, index)))
		{
			PrintFail(" -- Returned 1 when should have return 0.");
			break ;
		}
		--index;
		if (index == 32)
			--index;
	}
	if (!index)
		PrintPass();
}

void TestFlipBit(int print, bit_array_t num)
{
	bit_array_t one = num;
	bit_array_t arr = 0;
	bit_array_t old = 0;

	arr = one << 32;
	old = arr;

	printf(CYAN"\n-- FlipBit(%ld):\n\n"RESET, num);
	if (print)
	{
		printf("-- Bitarray befor:	");
		PrintBit(arr, 64);
		arr = FlipBit(arr, 32);
		printf("-- Bitarray after:	");
		PrintBit(arr, 64);
	}
	else
		arr = FlipBit(arr, 32);
	num = (one) ? 0 : 1;
	one = 1;
	if (arr == num << 32)
		PrintPass();
	else
		PrintFail(" -- Didn't flip the bit.");
	if ((arr ^ old) == (one << 32))
		PrintPass();
	else
		PrintFail(" -- Other bits have changed.");
}

void TestSetBit(int print)
{
	bit_array_t arr = 0;

	printf(CYAN"\n-- SetBit():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = SetBit(arr, 0, 1);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = SetBit(arr, 0, 1);
	if (arr == 1)
		PrintPass();
	else
		PrintFail(" -- Didn't set bit On.");
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = SetBit(arr, 0, 0);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = SetBit(arr, 0, 0);
	if (arr == 0)
		PrintPass();
	else
		PrintFail(" -- Didn't set bit OFF.");
}

void TestSetAll(int print)
{
	bit_array_t arr = 12124;

	printf(CYAN"\n-- SetAll():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = SetAll(arr);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = SetAll(arr);
	if (~((bit_array_t)0) == arr)
		PrintPass();
	else
		PrintFail(" -- Not all bits are set ON.");
}

void TestResetAll(int print)
{
	bit_array_t arr = 12124;

	printf(CYAN"\n-- ResetAll():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = ResetAll(arr);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = ResetAll(arr);
	if ((bit_array_t)0 == arr)
		PrintPass();
	else
		PrintFail(" -- Not all bits are set OFF.");
}

void TestCountOn(int print)
{
	bit_array_t arr = (bit_array_t)0xf000000000000000;
	size_t n = 0;

	printf(CYAN"\n-- CountSetBitsOn():\n\n"RESET);
	n = CountSetBitsOn(arr);
	if (print)
	{
		printf("-- Array :	");
		PrintBit(arr, 64);
		printf("-- Returned %ld\n", n);
	}
	if (4 == n)
		PrintPass();
	else
		PrintFail(" -- Didn't count the correct number of bits.");
}

void TestCountOnLUT(int print)
{
	bit_array_t arr = (bit_array_t)0xf000000000000000;
	size_t n = 0;

	printf(CYAN"\n-- CountSetBitsOnLUT():\n\n"RESET);
	n = CountSetBitsOnLUT(arr);
	if (print)
	{
		printf("-- Array :	");
		PrintBit(arr, 64);
		printf("-- Returned %ld\n", n);
	}
	if (4 == n)
		PrintPass();
	else
		PrintFail(" -- Didn't count the correct number of bits.");
}

void TestCountOff(int print)
{
	bit_array_t arr = (bit_array_t)0xf000000000000000;
	size_t n = 0;

	printf(CYAN"\n-- CountSetBitsOff():\n\n"RESET);
	n = CountSetBitsOff(arr);
	if (print)
	{
		printf("-- Array :	");
		PrintBit(arr, 64);
		printf("-- Returned %ld\n", n);
	}
	if (60 == n)
		PrintPass();
	else
		PrintFail(" -- Didn't count the correct number of bits.");
}

void TestSwapBits(int print)
{
	bit_array_t arr = (bit_array_t)0x8000000000000000;

	printf(CYAN"\n-- SwapBits():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = SwapBits(arr, 63, 0);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = SwapBits(arr, 63, 0);
	if (arr == (bit_array_t)0x0000000000000001)
		PrintPass();
	else
		PrintFail(" -- Changed other bits.");
	arr = 0;
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = SwapBits(arr, 63, 0);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = SwapBits(arr, 63, 0);
	if (arr == 0)
		PrintPass();
	else
		PrintFail(" -- Changed other bits.");

}

void TestToString(int print)
{
	bit_array_t arr = (bit_array_t)0xf0840ab7ff025410;
	bit_array_t end = (bit_array_t)1 << 63;
	char *dest = 0;
	size_t tmp1 = 0;
	size_t tmp2 = 0;
	int i = 0;

	dest = (char*)malloc(sizeof(char) * 65);
	printf(CYAN"\n-- ToString():\n\n"RESET);
	ToString(dest, arr);
	if (print)
	{
		printf("-- Array:	");
		PrintBitS(arr, 64);
		printf("-- String:	%s\n", dest);
	}	
	for (i = 0; i < 64; ++i)
	{
		tmp1 = ((arr & (end >> i))) ? 1 : 0;
		tmp2 = dest[i] - '0';
		if (tmp1 != tmp2)
		{
			PrintFail(" -- The string doesn't match");
			printf("%d\n", i);
			printf("%s\n", dest);
			break ;
		}
	}
	if (i == 64)
		PrintPass();
	free(dest);
	dest = NULL;
}

void TestRotateLeft(int print)
{
	bit_array_t arr = (bit_array_t)0xf000000000000000;
	bit_array_t res = (bit_array_t)0x000000000000000f;

	printf(CYAN"\n-- RotateLeft():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = RotateLeft(arr, 68);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = RotateLeft(arr,68);
	if (res == arr)
		PrintPass();
	else
		PrintFail(" -- Bad rotation.");
}

void TestRotateRight(int print)
{
	bit_array_t arr = (bit_array_t)0x0000000000000f01;
	bit_array_t res = (bit_array_t)0xf010000000000000;

	printf(CYAN"\n-- RotateRight():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = RotateRight(arr, 12 + 64);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = RotateRight(arr, 12 + 64);
	if (res == arr)
		PrintPass();
	else
		PrintFail(" -- Bad rotation.");
}

void TestMirror(int print)
{
	bit_array_t arr = (bit_array_t)0x00000001000e0f01;
	bit_array_t res = (bit_array_t)0x80f0700080000000;

	printf(CYAN"\n-- Mirror():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = BitMirror(arr);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = BitMirror(arr);
	if (res == arr)
		PrintPass();
	else
		PrintFail(" -- Bad Mirror.");
}

void TestMirrorLUT(int print)
{
	bit_array_t arr = (bit_array_t)0x00000001000e0f01;
	bit_array_t res = (bit_array_t)0x80f0700080000000;

	printf(CYAN"\n-- MirrorLUT():\n\n"RESET);
	if (print)
	{
		printf("-- Array before:	");
		PrintBit(arr, 64);
		arr = BitMirrorLUT(arr);
		printf("-- Array after:		");
		PrintBit(arr, 64);
	}
	else
		arr = BitMirrorLUT(arr);
	if (res == arr)
		PrintPass();
	else
		PrintFail(" -- Bad Mirror.");
}

int main(int ac, char **av)
{
	int c = 0;
	int print = 0;

	if (2 <= ac)
	{
		c = atoi(av[1]);
		if (!strcmp(av[1], "-p"))
		{
			print = 1;
			if (3 == ac)
				c = atoi(av[2]);
		}
	}
	switch (c)
	{
		case 1:
			TestSetOn(print);
			break ;
		case 2:
			TestSetOff(print);
			break ;
		case 3:
			TestIsOn(print);
			break ;
		case 4:
			TestIsOff(print);
			break ;
		case 5:
			TestFlipBit(print, 0);
			TestFlipBit(print, 1);
			break ;
		case 6:
			TestSetBit(print);
			break ;
		case 7:
			TestSetAll(print);
			break ;
		case 8:
			TestResetAll(print);
			break ;
		case 9:
			TestCountOn(print);
			break ;
		case 10:
			TestCountOnLUT(print);
			break ;
		case 11:
			TestCountOff(print);
			break ;
		case 12:
			TestSwapBits(print);
			break ;
		case 13:
			TestToString(print);
			break ;
		case 14:
			TestRotateLeft(print);
			break ;
		case 15:
			TestRotateRight(print);
			break ;
		case 16:
			TestMirror(print);
			break ;
		case 17:
			TestMirrorLUT(print);
			break ;


		default:
			TestSetOn(print);
			TestSetOff(print);
			TestIsOn(print);
			TestIsOff(print);
			TestFlipBit(print, 0);
			TestFlipBit(print, 1);
			TestSetBit(print);
			TestSetAll(print);
			TestResetAll(print);
			TestCountOn(print);
			TestCountOnLUT(print);
			TestCountOff(print);
			TestSwapBits(print);
			TestToString(print);
			TestRotateLeft(print);
			TestRotateRight(print);
			TestMirror(print);
			TestMirrorLUT(print);
	}
	return 0;
}

