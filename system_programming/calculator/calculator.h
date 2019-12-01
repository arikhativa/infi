
#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

typedef enum status
{
        SUCCESS,
        SYNTAX_ERROR,
        MALLOC_FAIL,
        DIVIDE_BY_ZERO
} status_t;


status_t Calculator(const char *str, double *result);

#endif /*__CALCULATOR_H__*/
