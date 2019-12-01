
#include <stdlib.h>     /* malloc(), free() */

#include "write_print.h"    /*  Print() */
#include "knights_tour.h"


#define COUNTER_INDEX       64
#define BOARD_SIZE          64
#define BOARD_SIDE_SIZE     (BOARD_SIZE / 4)

/*
    The MASK_...'s macros are look up tables for masking each index on the board.
    (from 0-63). There is mask for the posibale moves (MASK_LUT), and masks
    for checking the 'grade' of the move (MSAK_TOP/BOTTOM/RIGHT etc..).
*/

#define MASK_LUT    132096u, 329728u, 659712u, 1319424u, \
                    2638848u, 5277696u, 10489856u, 4202496u, \
                    33816580u, 84410376u, 168886289u, 337772578u, \
                    675545156u, 1351090312u, 2685403152u, 1075839008u, \
                    8657044482u, 21609056261u, 43234889994u, 86469779988u, \
                    172939559976u, 345879119952u, 687463207072u, 275414786112u, \
                    2216203387392u, 5531918402816u, 11068131838464u, 22136263676928u, \
                    44272527353856u, 88545054707712u, 175990581010432u, 70506185244672u, \
                    567348067172352u, 1416171111120896u, 2833441750646784u, 5666883501293568u, \
                    11333767002587136u, 22667534005174272u, 45053588738670592u, 18049583422636032u, \
                    145241105196122112u, 362539804446949376u, 725361088165576704u, 1450722176331153408u, \
                    2901444352662306816u, 5802888705324613632u, 11533718717099671552u, 4620693356194824192u, \
                    288234782788157440u, 576469569871282176u, 1224997833292120064u, 2449995666584240128u, \
                    4899991333168480256u, 9799982666336960512u, 1152939783987658752u, 2305878468463689728u, \
                    1128098930098176u, 2257297371824128u, 4796069720358912u, 9592139440717824u, \
                    19184278881435648u, 38368557762871296u, 4679521487814656u, 9077567998918656u

#define MASK_TOP    18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                    18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                    1u, 2u, 4u, 8u, \
                    16u, 32u, 64u, 128u, \
                    256u, 512u, 1024u, 2048u, \
                    4096u, 8192u, 16384u, 32768u, \
                    65536u, 131072u, 262144u, 524288u, \
                    1048576u, 2097152u, 4194304u, 8388608u, \
                    16777216u, 33554432u, 67108864u, 134217728u, \
                    268435456u, 536870912u, 1073741824u, 2147483648u, \
                    4294967296u, 8589934592u, 17179869184u, 34359738368u, \
                    68719476736u, 137438953472u, 274877906944u, 549755813888u, \
                    1099511627776u, 2199023255552u, 4398046511104u, 8796093022208u, \
                    17592186044416u, 35184372088832u, 70368744177664u, 140737488355328u, \
                    281474976710656u, 562949953421312u, 1125899906842624u, 2251799813685248u, \
                    4503599627370496u, 9007199254740992u, 18014398509481984u, 36028797018963968u

#define MASK_BOTTOM 256u, 512u, 1024u, 2048u, \
                    4096u, 8192u, 16384u, 32768u, \
                    65536u, 131072u, 262144u, 524288u, \
                    1048576u, 2097152u, 4194304u, 8388608u, \
                    16777216u, 33554432u, 67108864u, 134217728u, \
                    268435456u, 536870912u, 1073741824u, 2147483648u, \
                    4294967296u, 8589934592u, 17179869184u, 34359738368u, \
                    68719476736u, 137438953472u, 274877906944u, 549755813888u, \
                    1099511627776u, 2199023255552u, 4398046511104u, 8796093022208u, \
                    17592186044416u, 35184372088832u, 70368744177664u, 140737488355328u, \
                    281474976710656u, 562949953421312u, 1125899906842624u, 2251799813685248u, \
                    4503599627370496u, 9007199254740992u, 18014398509481984u, 36028797018963968u, \
                    72057594037927936u, 144115188075855872u, 288230376151711744u, 576460752303423488u, \
                    1152921504606846976u, 2305843009213693952u, 4611686018427387904u, 9223372036854775808u, \
                    18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                    18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u

#define MASK_RIGHT  18446744073709551615u, 1u, 2u, 4u, \
                    8u, 16u, 32u, 64u, \
                    18446744073709551615u, 256u, 512u, 1024u, \
                    2048u, 4096u, 8192u, 16384u, \
                    18446744073709551615u, 65536u, 131072u, 262144u, \
                    524288u, 1048576u, 2097152u, 4194304u, \
                    18446744073709551615u, 16777216u, 33554432u, 67108864u, \
                    134217728u, 268435456u, 536870912u, 1073741824u, \
                    18446744073709551615u, 4294967296u, 8589934592u, 17179869184u, \
                    34359738368u, 68719476736u, 137438953472u, 274877906944u, \
                    18446744073709551615u, 1099511627776u, 2199023255552u, 4398046511104u, \
                    8796093022208u, 17592186044416u, 35184372088832u, 70368744177664u, \
                    18446744073709551615u, 281474976710656u, 562949953421312u, 1125899906842624u, \
                    2251799813685248u, 4503599627370496u, 9007199254740992u, 18014398509481984u, \
                    18446744073709551615u, 72057594037927936u, 144115188075855872u, 288230376151711744u, \
                    576460752303423488u, 1152921504606846976u, 2305843009213693952u, 4611686018427387904u

#define MASK_LEFT   2u, 4u, 8u, 16u, \
                    32u, 64u, 128u, 18446744073709551615u, \
                    512u, 1024u, 2048u, 4096u, \
                    8192u, 16384u, 32768u, 18446744073709551615u, \
                    131072u, 262144u, 524288u, 1048576u, \
                    2097152u, 4194304u, 8388608u, 18446744073709551615u, \
                    33554432u, 67108864u, 134217728u, 268435456u, \
                    536870912u, 1073741824u, 2147483648u, 18446744073709551615u, \
                    8589934592u, 17179869184u, 34359738368u, 68719476736u, \
                    137438953472u, 274877906944u, 549755813888u, 18446744073709551615u, \
                    2199023255552u, 4398046511104u, 8796093022208u, 17592186044416u, \
                    35184372088832u, 70368744177664u, 140737488355328u, 18446744073709551615u, \
                    562949953421312u, 1125899906842624u, 2251799813685248u, 4503599627370496u, \
                    9007199254740992u, 18014398509481984u, 36028797018963968u, 18446744073709551615u, \
                    144115188075855872u, 288230376151711744u, 576460752303423488u, 1152921504606846976u, \
                    2305843009213693952u, 4611686018427387904u, 9223372036854775808u, 18446744073709551615u

#define MASK_TOP_LEFT  18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                        18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                        0u, 1u, 2u, 4u, \
                        8u, 16u, 32u, 18446744073709551615u, \
                        128u, 256u, 512u, 1024u, \
                        2048u, 4096u, 8192u, 18446744073709551615u, \
                        32768u, 65536u, 131072u, 262144u, \
                        524288u, 1048576u, 2097152u, 18446744073709551615u, \
                        8388608u, 16777216u, 33554432u, 67108864u, \
                        134217728u, 268435456u, 536870912u, 18446744073709551615u, \
                        2147483648u, 4294967296u, 8589934592u, 17179869184u, \
                        34359738368u, 68719476736u, 137438953472u, 18446744073709551615u, \
                        549755813888u, 1099511627776u, 2199023255552u, 4398046511104u, \
                        8796093022208u, 17592186044416u, 35184372088832u, 18446744073709551615u, \
                        140737488355328u, 281474976710656u, 562949953421312u, 1125899906842624u, \
                        2251799813685248u, 4503599627370496u, 9007199254740992u, 18446744073709551615u

#define MASK_TOP_RIGHT   18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                        18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                        18446744073709551615u, 4u, 8u, 16u, \
                        32u, 64u, 128u, 256u, \
                        18446744073709551615u, 1024u, 2048u, 4096u, \
                        8192u, 16384u, 32768u, 65536u, \
                        18446744073709551615u, 262144u, 524288u, 1048576u, \
                        2097152u, 4194304u, 8388608u, 16777216u, \
                        18446744073709551615u, 67108864u, 134217728u, 268435456u, \
                        536870912u, 1073741824u, 2147483648u, 4294967296u, \
                        18446744073709551615u, 17179869184u, 34359738368u, 68719476736u, \
                        137438953472u, 274877906944u, 549755813888u, 1099511627776u, \
                        18446744073709551615u, 4398046511104u, 8796093022208u, 17592186044416u, \
                        35184372088832u, 70368744177664u, 140737488355328u, 281474976710656u, \
                        18446744073709551615u, 1125899906842624u, 2251799813685248u, 4503599627370496u, \
                        9007199254740992u, 18014398509481984u, 36028797018963968u, 72057594037927936u

#define MASK_BOTTOM_RIGHT   18446744073709551615u, 256u, 512u, 1024u, \
                            2048u, 4096u, 8192u, 16384u, \
                            18446744073709551615u, 65536u, 131072u, 262144u, \
                            524288u, 1048576u, 2097152u, 4194304u, \
                            18446744073709551615u, 16777216u, 33554432u, 67108864u, \
                            134217728u, 268435456u, 536870912u, 1073741824u, \
                            18446744073709551615u, 4294967296u, 8589934592u, 17179869184u, \
                            34359738368u, 68719476736u, 137438953472u, 274877906944u, \
                            18446744073709551615u, 1099511627776u, 2199023255552u, 4398046511104u, \
                            8796093022208u, 17592186044416u, 35184372088832u, 70368744177664u, \
                            18446744073709551615u, 281474976710656u, 562949953421312u, 1125899906842624u, \
                            2251799813685248u, 4503599627370496u, 9007199254740992u, 18014398509481984u, \
                            18446744073709551615u, 72057594037927936u, 144115188075855872u, 288230376151711744u, \
                            576460752303423488u, 1152921504606846976u, 2305843009213693952u, 4611686018427387904u, \
                            18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                            18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u
#define MASK_BOTTOM_LEFT    512u, 1024u, 2048u, 4096u, \
                            8192u, 16384u, 32768u, 18446744073709551615u, \
                            131072u, 262144u, 524288u, 1048576u, \
                            2097152u, 4194304u, 8388608u, 18446744073709551615u, \
                            33554432u, 67108864u, 134217728u, 268435456u, \
                            536870912u, 1073741824u, 2147483648u, 18446744073709551615u, \
                            8589934592u, 17179869184u, 34359738368u, 68719476736u, \
                            137438953472u, 274877906944u, 549755813888u, 18446744073709551615u, \
                            2199023255552u, 4398046511104u, 8796093022208u, 17592186044416u, \
                            35184372088832u, 70368744177664u, 140737488355328u, 18446744073709551615u, \
                            562949953421312u, 1125899906842624u, 2251799813685248u, 4503599627370496u, \
                            9007199254740992u, 18014398509481984u, 36028797018963968u, 18446744073709551615u, \
                            144115188075855872u, 288230376151711744u, 576460752303423488u, 1152921504606846976u, \
                            2305843009213693952u, 4611686018427387904u, 9223372036854775808u, 18446744073709551615u, \
                            18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u, \
                            18446744073709551615u, 18446744073709551615u, 18446744073709551615u, 18446744073709551615u

enum grade_index
{
    TOP_INDEX = 0,
    BOTTOM_INDEX,
    RIGHT_INDEX,
    LEFT_INDEX,
    TOP_LEFT_INDEX,
    TOP_RIGHT_INDEX,
    BOTTOM_LEFT_INDEX,
    BOTTOM_RIGHT_INDEX
};

/************************************************************* Global var:   */

static char *g_result = 0;
static size_t *g_mask_lut = 0;
static size_t *g_mask_grade_lut[8];

/******************************************************* Static functions:   */

static size_t RecFillBoard(size_t map, size_t index);
static size_t GetNextMove(size_t map, size_t options);
static size_t GetNextIndex(size_t next_move);
static size_t GetGrade(size_t map, size_t point);

static void PrintResulat(void);
static int CountDigit(char num);


/*
KTstatus_t KnightTour(int x, int y);

int main(int ac, char **av)
{
    if (ac < 3)
    {
        PutStr("more args");
        return 0;
    }
        tested till 7
        hard index 0 , 4, 7
    KnightTour(atoi(av[1]), atoi(av[2]));
    return 0;
}
*/


/************************************************************ KnightTour()   */

KTstatus_t KnightTour(int x, int y)
{
    size_t mask[] = {MASK_LUT};
    size_t mask_top[] = {MASK_TOP};
    size_t mask_bottom[] = {MASK_BOTTOM};
    size_t mask_right[] = {MASK_RIGHT};
    size_t mask_left[] = {MASK_LEFT};
    size_t mask_top_left[] = {MASK_TOP_LEFT};
    size_t mask_top_right[] = {MASK_TOP_RIGHT};
    size_t mask_bottom_left[] = {MASK_BOTTOM_LEFT};
    size_t mask_bottom_right[] = {MASK_BOTTOM_RIGHT};
    size_t map = 0;
    size_t first_move = 1;

    if (system("clear"))
    {
        return FAIL;
    }

    g_mask_lut = mask;
    g_mask_grade_lut[TOP_INDEX] = mask_top;
    g_mask_grade_lut[BOTTOM_INDEX] = mask_bottom;
    g_mask_grade_lut[RIGHT_INDEX] = mask_right;
    g_mask_grade_lut[LEFT_INDEX] = mask_left;
    g_mask_grade_lut[TOP_LEFT_INDEX] = mask_top_left;
    g_mask_grade_lut[TOP_RIGHT_INDEX] = mask_top_right;
    g_mask_grade_lut[BOTTOM_LEFT_INDEX] = mask_bottom_left;
    g_mask_grade_lut[BOTTOM_RIGHT_INDEX] = mask_bottom_right;

    g_result = (char*)malloc(sizeof(char) * (BOARD_SIZE + 1));

    /*  The extra byte in the malloc is for a counter that the print function use. */

    if (!g_result)
    {
        return FAIL;
    }
    g_result[COUNTER_INDEX] = COUNTER_INDEX;

    first_move <<= x + (BOARD_SIDE_SIZE * y);

/*    Print("-- starting point:\n%m\n", first_move);*/

    RecFillBoard(map | first_move, GetNextIndex(first_move));

    PrintResulat();

    free(g_result);
    g_result = NULL;

    return SUCCESS;
}

static size_t RecFillBoard(size_t map, size_t index)
{
    size_t next_move = 0;
    size_t moves = 0;
    size_t ret = FAIL;

    if (!(~map))
    {
        g_result[index] = g_result[COUNTER_INDEX];
        --g_result[COUNTER_INDEX];

        return SUCCESS;
    }

    moves = ~map & g_mask_lut[index];

    while (FAIL == ret)
    {
        next_move = GetNextMove(map, moves);
        if (0 == next_move)
        {
            return FAIL;
        }

        ret = RecFillBoard(map | next_move, GetNextIndex(next_move));

        /* In case ret is FAIL, we need to remove the 'move' we tested. */

        moves = moves ^ next_move;
    }

    if (SUCCESS == ret)
    {
        g_result[index] = g_result[COUNTER_INDEX];
        --g_result[COUNTER_INDEX];

        return SUCCESS;
    }

    return FAIL;
}

static size_t GetNextMove(size_t map, size_t options)
{
    size_t next_move = options;
    size_t grade = 0;
    size_t best_grade = 0;
    size_t best_move = 0;

    while (options)
    {
        /*  This while loop removes all moves from options except the one */

        while (next_move & (next_move - 1))
        {
            next_move = next_move & (next_move - 1);
        }

        grade = GetGrade(map, GetNextIndex(next_move));
        if (grade > best_grade)
        {
            best_move = next_move;
            best_grade = grade;
        }

        options = options ^ next_move;
        next_move = options;
    }

    return best_move;
}

static size_t GetNextIndex(size_t next_move)
{
    size_t i = 0;

    next_move >>= 1;
    while (next_move)
    {
        next_move >>= 1;
        ++i;
    }

    return i;
}

static size_t GetGrade(size_t map, size_t point)
{
    size_t grade = 1;

    grade += !!(map & g_mask_grade_lut[TOP_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[BOTTOM_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[RIGHT_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[LEFT_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[TOP_LEFT_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[TOP_RIGHT_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[BOTTOM_LEFT_INDEX][point]);
    grade += !!(map & g_mask_grade_lut[BOTTOM_RIGHT_INDEX][point]);

    return grade;
}

/************************************************************ PrintResulat()  */

static void PrintResulat(void)
{
    size_t a = 0;
    size_t b = 0;
    char tmp = 0;

    Print("\n\n");

    while (a < BOARD_SIZE)
    {
        Print("\t| ");

        for (b = 0; b < BOARD_SIDE_SIZE ;++b)
        {
            tmp = g_result[a + b];

            Print("%lu", tmp);

            if (b + 1 < BOARD_SIDE_SIZE)
            {
                if (1 == CountDigit(tmp))
                {
                    Print("  | ");
                }
                else
                {
                    Print(" | ");
                }
            }
        }

        Print("\t|");
        Print("\n\n");

        a += BOARD_SIDE_SIZE;
    }
}

static int CountDigit(char num)
{
    if (num >= 10)
    {
        return 2;
    }

    return 1;
}
