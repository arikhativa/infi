
/*
    ver 1.3     added PrintIntArray() && PrintLongArray().
    ver 1.2     Print is working with '%'.
    ver 1.1     Nice is nice.
*/

/*
    Comments for the futre:
*/

#include <unistd.h>     /*  write() */
#include <stdarg.h>     /* va_start(), va_args(), va_end()  */

#include "color.h"      /* ver 1.2 is needed */

#define BASE_HEXA 16

enum spacial_chars
{
    CHAR = 1,
    LONG
};

enum actions
{
    DECIMAL     = 10,
    UN_DECIMAL  = 20,
    HEXA        = 30,
    STR         = 40,
    BINARY      = 50,
    ARRAY       = 60,
    BIT_MAP     = 70
};

/*************************************************************** PutChar():  */
static void PutChar(char ch)
{
    int dump;

    dump = write(1, &ch, 1);

    (void)dump;
}


/***************************************************************** PutStr():  */
static void PutStr(char *str)
{
    while (*str)
    {
        PutChar(*str);
        ++str;
    }
}

/******************************************************** PutUnsignedNum():  */
static void PutUnsignedNum(size_t num)
{
    if (num / 10)
        PutUnsignedNum(num / 10);
    PutChar((num % 10) + '0');
}

/***************************************************************** PutNum():  */
static void PutNum(ssize_t num)
{
    if (0 > num)
    {
        PutChar('-');
        num = -num;
    }
    if (num / 10)
        PutUnsignedNum(num / 10);
    PutChar((num % 10) + '0');
}

/********************************************************** PrintIntArray():  */
static void PrintIntArray(int *arr, size_t size)
{
    size_t i = 0;

    PutChar('[');

    PutNum(arr[i]);

    for (i = 1; i < size; ++i)
    {
        PutStr(", ");
        PutNum(arr[i]);
    }
    PutChar(']');

    PutChar('\n');
}

/******************************************************** PrintLongArray():  */
static void PrintLongArray(size_t *arr, size_t size)
{
    size_t i = 0;

    PutChar('[');
    PutUnsignedNum(arr[i]);

    for (i = 1; i < size; ++i)
    {
        PutStr(", ");
        PutUnsignedNum(arr[i]);
    }
    PutChar(']');

    PutChar('\n');
}

/************************************************************ PrintBinary():  */
static void PrintBinary(size_t num, size_t bytes)
{
    size_t mask = 1;

    mask <<= (bytes * __CHAR_BIT__) - 1;

    do
    {
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        (num & mask) ? PutChar('1') : PutChar('0');
        mask >>= 1;
        --bytes;
        if (bytes)
        {
            PutChar(' ');
        }
    } while (bytes);
}

/************************************************************** PrintHexa():  */
static void RecPrintHexa(size_t num, char *array_base)
{
    if (num / BASE_HEXA)
    {
        RecPrintHexa(num / BASE_HEXA, array_base);
    }
    PutChar(array_base[(num % BASE_HEXA)]);
}

static void PrintHexa(size_t num)
{
    char array_base[] = "0123456789abcdef";

    PutStr("0x");
    RecPrintHexa(num, array_base);
}


/********************************************************* PrintBitMap():    */
static void PrintLine(size_t num)
{
    size_t mask = 1;

    mask <<= __CHAR_BIT__ - 1;

    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);
    mask >>= 1;
    (num & mask) ? PutStr(BIT_ON"1"RESET) : PutStr(BIT_OFF"0"RESET);

    PutChar('\n');
}

static void PrintBitMap(size_t map)
{
    size_t i = 0;

    for (i = 0; i < sizeof(size_t); ++i)
    {
        PrintLine(map);
        map >>= __CHAR_BIT__;
    }
}


/***************************************************************** Print():  */
static int CheakSpacialChar(char ch)
{
    return ('%' == ch);
}

static int GetType(char **str)
{
    int ret = 0;

    switch(**str)
    {
        case 'c':
            ret = CHAR;
            ++*str;
            break ;

        case 'l':
            ret = LONG;
            ++*str;
            break ;
    }

    return ret;
}

static int GetAction(char ch)
{
    switch(ch)
    {
        case 's':
        return STR;

        case 'd':
        return DECIMAL;

        case 'u':
        return UN_DECIMAL;

        case 'b':
        return BINARY;

        case 'x':
        return HEXA;

        case 'm':
        return BIT_MAP;

        case 'a':
        return ARRAY;
    }

    return 0;
}

static void PrintAndLookForSpacialChar(char **str)
{
    while (**str)
    {
        if (CheakSpacialChar(**str))
        {
            ++*str;
            return;
        }

        PutChar(**str);
        ++*str;
    }
}

void Print(char *info, ...)
{
    size_t *size_t_ptr = 0;
    size_t size = 0;
    int *int_ptr = 0;
    int key = 0;
    va_list ap;

    va_start(ap, info);

    PrintAndLookForSpacialChar(&info);

    while (*info)
    {
        key = GetType(&info);
        key += GetAction(*info);

        switch (key)
        {
            case STR:
                PutStr(va_arg(ap, char*));
                break ;

            case DECIMAL:
                PutNum(va_arg(ap, int));
                break ;

            case DECIMAL + LONG:
                PutNum(va_arg(ap, ssize_t));
                break ;

            case UN_DECIMAL:
                PutUnsignedNum(va_arg(ap, int));
                break ;

            case UN_DECIMAL + LONG:
                PutUnsignedNum(va_arg(ap, size_t));
                break ;

            case HEXA:
                PrintHexa(va_arg(ap, size_t));
                break ;

            case BINARY:
                PrintBinary(va_arg(ap, int), sizeof(unsigned int));
                break ;

            case BINARY + CHAR:
                PrintBinary(va_arg(ap, int), sizeof(unsigned char));
                break ;

            case BINARY + LONG:
                PrintBinary(va_arg(ap, size_t), sizeof(size_t));
                break ;

            case BIT_MAP:
                PrintBitMap(va_arg(ap, size_t));
                break ;

            case ARRAY:
                int_ptr = va_arg(ap, int*);
                size = va_arg(ap, size_t);
                PrintIntArray(int_ptr, size);
                break ;

            case ARRAY + LONG:
                size_t_ptr = va_arg(ap, size_t*);
                size = va_arg(ap, size_t);
                PrintLongArray(size_t_ptr, size);
                break ;

        }
        ++info;
        PrintAndLookForSpacialChar(&info);
    }

    va_end(ap);
}

/*
int main()
{
    size_t i[] = {7,2, 3, 41, 2};

    Print("-- %la\n", i, 5);

    return 0;
}
*/
