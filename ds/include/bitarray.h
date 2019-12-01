
/*
 * 	var 1.2
 */

#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>

typedef size_t bit_array_t;

/*
 *	SetOn(), SetOff(), FlipBit(), SetBit().
 *	Change the status of a specific bit.
 *
 *	Argument:
 *			'arr'	- The bit array.
 *			'index'	- The index of the bit.
 *	(for SetBit)	'val'	- The value to set.
 *	Return:
 *			The array after the change.
 */
	bit_array_t SetOn(bit_array_t arr, size_t index);
	bit_array_t SetOff(bit_array_t arr, size_t index);
	bit_array_t FlipBit(bit_array_t arr, size_t index);
	bit_array_t SetBit(bit_array_t arr, size_t index, int val);

/*
 *	SetAll(), ResetAll()
 *	Changes all of the array to 1 or to 0.
 *
 *	Argument:
 *			'arr'	- The bit array.
 *	Return:
 *			SetAll() - Array with all the bits set to 1.
 *			ResetAll() - Array with all the bits set to 0.
 */
	bit_array_t SetAll(bit_array_t arr);	
	bit_array_t ResetAll(bit_array_t arr);

/*
 *	IsOn(), IsOff()
 *	Check if a bit is set on/off.
 *
 *	Argument:
 *			'arr'	- The bit array.
 *			'index'	- The index of the bit.
 *	Return:
 *			IsOn() - 1 for set on.
 *			IsOff() - 1 for set off.
 */
	int IsOn(bit_array_t arr, size_t index);
	int IsOff(bit_array_t arr, size_t index);

/*
 *	CountSetBitsOn(), CountSetBitsOff(), CountSetBitsLUT()
 *	Count the number of set on/off bits.
 *
 *	Argument:
 *			'arr'	- The bit array.
 *	Return:
 *			Number of bits.
 */
	size_t CountSetBitsOn(bit_array_t arr);
    size_t CountSetBitsOnLUT(bit_array_t arr);
	size_t CountSetBitsOff(bit_array_t arr);

/*
 *	SwapBits()
 *	Swap two bits.
 *
 *	Argument:
 *			'bit1'	- The index of the first bit.
 *			'bit2'	- The index of the second bit.
 *			'arr'	- The bit array.
 *	Return:
 *			The array after the swap.
 */
	bit_array_t SwapBits(bit_array_t arr, size_t bit1, size_t bit2);

/*	
 *	ToString()
 *	Make a string from the array.
 *
 *	Argument:
 *			'dest'	- The addres to write in to.
 *			'arr'	- The bit array.
 *	Return:
 *			'dest'.
 */
	char *ToString(char *dest, bit_array_t arr);

/*
 * 	RotateLeft(), RotateRight()
 *	Rotate the array 'n' times to the left/right.
 *
 *	Argument:
 *			'arr'	- The bit array.
 *			'n'	- Times of rotation.
 *	Return:
 *			The rotated array.
 */
	bit_array_t RotateLeft(bit_array_t arr, size_t n);
	bit_array_t RotateRight(bit_array_t arr, size_t n);

/*
 * 	BitMirror()
 *	Mirrors the array.
 *
 *	Argument:
 *			'arr'	- The bit array.
 *	Return:
 *			The mirrored array.
 */
	bit_array_t BitMirror(bit_array_t arr);
	bit_array_t BitMirrorLUT(bit_array_t arr);

#endif /* _BIT_ARRAY_H__ */

