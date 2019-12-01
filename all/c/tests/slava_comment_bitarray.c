
#include <assert.h> /* assert() */
#include <limits.h> /* CHAR_BIT */
#include "bitarray.h"

/*
 *	Macro for Count LUT.
 */
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define COUNT_BITS B6(0), B6(1), B6(1), B6(2)

#define NUM_OF_BITS(arr) sizeof(arr) * 8
CHAR_BIT
#define SIZE_OF_LUT 256
#define BIT_ON(index) (1UL << index)

bit_array_t SetOn(bit_array_t arr, size_t index)
{
	return (arr | BIT_ON(index));
}

bit_array_t SetOff(bit_array_t arr, size_t index)
{
	bit_array_t mask = 1;

	arr &= ~(mask << index);

	return arr;
}

int IsOn(bit_array_t arr, size_t index)
{
	bit_array_t mask = 1;

	return (!!(arr & (mask << index)));
}

int IsOff(bit_array_t arr, size_t index)
{
	bit_array_t mask = 1;

	return (0 != (arr ^ mask << index));
}

bit_array_t FlipBit(bit_array_t arr, size_t index)
{
	if (IsOn(arr, index))
	{
		arr = SetOff(arr, index);
	}
	else
	{
		arr = SetOn(arr, index);
	}

	return arr;
}

bit_array_t SetBit(bit_array_t arr, size_t index, int val)
{
	return ((val) ? SetOn(arr, index) : SetOff(arr, index));
}

bit_array_t SetAll(bit_array_t arr)
{
	arr = (~((bit_array_t)0));

	return arr;
}

bit_array_t ResetAll(bit_array_t arr)
{
	(void)arr;

	return 0;
}

size_t CountSetBitsOn(bit_array_t arr)
{
	size_t count = 0;

	while (arr)
	{
		arr &= (arr - 1);
		++count;
	}

	return count;
}

size_t CountSetBitsOnLUT(bit_array_t arr)
{
	static const unsigned int LUT[SIZE_OF_LUT] = { COUNT_BITS };
	size_t count = 0;
	unsigned char tmp = 0;

	while (arr)
	{
		tmp = arr;
		count += LUT[tmp];
		arr >>= BITS_IN_BYTE;
	}

	return count;
}

size_t CountSetBitsOff(bit_array_t arr)
{
	size_t count = NUM_OF_BITS_IN_ARR;

	count -= CountSetBitsOn(arr);

	return count;
}

bit_array_t SwapBits(bit_array_t arr, size_t bit1, size_t bit2)
{
	bit_array_t tmp = arr;

	if (IsOn(arr, bit1))
	{
		tmp = SetOn(tmp, bit2);
	}
	else
	{
		tmp = SetOff(tmp, bit2);
	}
	if (IsOn(arr, bit2))
	{
		tmp = SetOn(tmp, bit1);
	}
	else
	{
		tmp = SetOff(tmp, bit1);
	}

	return tmp;
}

char *ToString(char *dest, bit_array_t arr)
{
	bit_array_t mask = (bit_array_t)1 << (NUM_OF_BITS_IN_ARR - 1);
	char *start_of_str = dest;

	assert(dest);

	while (mask)
	{
		*dest = (arr & mask) ? '1' : '0';
		++dest;
		mask >>= 1;
	}
	*dest = '\0';

	return start_of_str;
}

static bit_array_t RotateOnceLeft(bit_array_t arr)
{
	if (IsOn(arr, NUM_OF_BITS_IN_ARR - 1))
	{
		arr <<= 1;
		arr = SetOn(arr, 0);
	}
	else
	{
		arr <<= 1;
	}

	return arr;
}

bit_array_t RotateLeft(bit_array_t arr, size_t n)
{
	n %= NUM_OF_BITS_IN_ARR;
	for (; n; --n)
	{
		arr = RotateOnceLeft(arr);
	}

	return arr;
}

static bit_array_t RotateOnceRight(bit_array_t arr)
{
	if (IsOn(arr, 0))
	{
		arr >>= 1;
		arr = SetOn(arr, (NUM_OF_BITS_IN_ARR - 1));
	}
	else
	{
		arr >>= 1;
	}

	return arr;
}

bit_array_t RotateRight(bit_array_t arr, size_t n)
{
	if (NUM_OF_BITS_IN_ARR < n)
	{
		n %= NUM_OF_BITS_IN_ARR;
	}
	for (; n; --n)
	{
		arr = RotateOnceRight(arr);
	}

	return arr;

}

bit_array_t BitMirror(bit_array_t arr)
{
	size_t index = 0;

	while ((NUM_OF_BITS_IN_ARR / 2) > index)
	{
		arr = SwapBits(arr, index, (NUM_OF_BITS_IN_ARR - 1) - index);
		++index;
	}

	return arr;
}

static unsigned char ByteMirror(unsigned char byte)
{
	int index = 0;

	while ((BITS_IN_BYTE / 2) > index)
	{
		byte = SwapBits(byte, index, (BITS_IN_BYTE - 1) - index);
		++index;
	}

	return byte;
}

static void InitLUT(unsigned char LUT[])
{
	unsigned char byte = 0;

	while ((unsigned char)(SIZE_OF_LUT - 1) > byte)
	{
		LUT[byte] = ByteMirror(byte);
		++byte;
	}
	LUT[byte] = ByteMirror(byte);
}

bit_array_t BitMirrorLUT(bit_array_t arr)
{
	static unsigned char LUT[SIZE_OF_LUT] = { 0 };
	size_t index = 0;
	bit_array_t mirror = 0;


	if (LUT[1] != 126)
	{
		InitLUT(LUT);
	}
	for (index = BITS_IN_BYTE; NUM_OF_BITS_IN_ARR > index; index += BITS_IN_BYTE)
	{
		mirror += (unsigned char)LUT[(unsigned char)arr];
		arr >>= BITS_IN_BYTE;
		mirror <<= BITS_IN_BYTE;
	}

	return mirror;
}
