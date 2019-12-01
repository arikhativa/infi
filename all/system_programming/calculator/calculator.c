
#include <stdlib.h> 	/*	malloc(), free()	*/
#include <string.h>	/*	memset()		*/
#include <assert.h>	/*	assert()		*/
#include <ctype.h>	/*	isalpha(), isdigit()	*/

#include "calculator.h"
#include "stack.h"

#define NUM_OF_STATES 3
#define NUM_OF_INPUT 5

/*I_NUM		I_OP			I_SPACE		I_END		I_ERROR*/
#define D_START		\
AddNumToStack,	CheckSpecialOperator,	GoToStart,	Error,		Error

#define D_NUM		\
Error,		CheckOperator,		GoToNum,	End,		Error

#define D_OP		\
AddNumToStack,	CheckSpecialOperator,	GoToOperator, CheckSpecialOperator, Error

typedef enum priority
{
	L4,	/*	'(' && ')'	*/
	L1,	/*	'+' && '-'	*/
	L2,	/*	'*' && '/'	*/
	L3	/*	'^'		*/
}priority_t;

typedef enum machin_input
{
	I_NUM,
	I_OP,
	I_SPACE,
	I_END,
	I_ERROR
} input_t;

typedef enum machin_status
{
	START,
	NUM,
	OP,
	END,
	ERROR

} stt_t;

typedef struct calc calc_t;
typedef stt_t (*fp_t)(calc_t *calc);

struct calc
{
	char *str;
	status_t status;
	fp_t (*lut)[NUM_OF_INPUT];
	stack_t *nums;
	stack_t *ops;
};

/*
 * 	Main Functions - Line 142
 */
static calc_t *CalcCreate(const char *str, fp_t (*lut)[NUM_OF_INPUT], size_t len);
static void CalcDestroy(calc_t *calc);
static status_t RunCalc(calc_t *calc);
static input_t CharToInput(calc_t *calc);

/*
 * 	State Machine Functions - Line 239
 */
static stt_t CheckOperator(calc_t *calc);
static stt_t CheckSpecialOperator(calc_t *calc);
static stt_t GoToNum(calc_t *calc);
static stt_t GoToOperator(calc_t *calc);
static stt_t GoToStart(calc_t *calc);
static stt_t End(calc_t *calc);
static stt_t Error(calc_t *calc);
static stt_t AddNumToStack(calc_t *calc);

/*
 * 	Manege Math Ruls Functions - Line 423
 */
static status_t WriteInResult(double *result, calc_t *calc);
static status_t CalcIfCan(calc_t *calc, priority_t lvl);
static status_t ManagePara(calc_t *calc, stt_t *ret);
static status_t CalcInPara(calc_t *calc);
static status_t SumStack(calc_t *calc);
static stt_t AddMultiplayByMinusOne(calc_t *calc);

/*
 * 	Check Functions - Line 570
 */
static int IsOperator(char ch);
static priority_t GetPriority(char ch);

/*
 * 	Math Functions - Line 611
 */
static status_t Calculate(calc_t *calc);
static void Plus(calc_t *calc, double num1, double num2);
static void Minus(calc_t *calc, double num1, double num2);
static void Multiplay(calc_t *calc, double num1, double num2);
static void Divide(calc_t *calc, double num1, double num2);
static void Power(calc_t *calc, double num1, double num2);




status_t Calculator(const char *str, double *result)
{
	fp_t lut[NUM_OF_STATES][NUM_OF_INPUT] = {
		{D_START},
		{D_NUM},
		{D_OP}
	};
	status_t status = 0;
	calc_t *calc = NULL;

	calc = CalcCreate(str, lut, strlen(str));
	if (!calc)
	{
		return MALLOC_FAIL;
	}

	status = RunCalc(calc);

	*result = 0;
	if (SUCCESS == status)
	{
		status = WriteInResult(result, calc);
	}

	CalcDestroy(calc);
	calc = NULL;

	return status;
}

/*
 * 	Main Functions:
 */

static calc_t *CalcCreate(const char *str, fp_t (*lut)[NUM_OF_INPUT], size_t len)
{
	calc_t *calc = NULL;

	assert(str);
	assert(lut);

	calc = (calc_t*)malloc(sizeof(calc_t));
	if (!calc)
	{
		return NULL;
	}

	calc->nums = CreateStack(len, sizeof(double));
	if (NULL == calc->nums)
	{
		free(calc);
		return NULL;
	}

	calc->ops = CreateStack(len, sizeof(char));
	if (!(calc->ops))
	{
		DestroyStack(calc->nums);
		free(calc);
		return NULL;
	}

	calc->str = (char *)str;
	calc->lut = lut;

	return calc;
}

static void CalcDestroy(calc_t *calc)
{
	assert(calc);

	DestroyStack(calc->nums);
	DestroyStack(calc->ops);

	memset(calc, 0, sizeof(calc_t));
	free(calc);
	calc = NULL;
}

static status_t RunCalc(calc_t *calc)
{
	stt_t status = START;
	input_t input = 0;

	assert(calc);

	while (status != ERROR && status != END)
	{
		input = CharToInput(calc);
		status = calc->lut[status][input](calc);
	}

	return calc->status;
}

static input_t CharToInput(calc_t *calc)
{
	char ch = 0;

	assert(calc);

	ch = *(calc->str);

	if (isdigit(ch))
	{
		return I_NUM;
	}

	if (isspace(ch))
	{
		return I_SPACE;
	}

	if (IsOperator(ch))
	{
		return I_OP;
	}

	if ('\0' == ch)
	{
		return I_END;
	}

	return I_ERROR;
}

/*
 * 	State Machine Functions:
 */
static stt_t CheckOperator(calc_t *calc)
{
	char op = 0;
	int op_lvl = 0;
	status_t status = 0;
	stt_t ret = OP;

	assert(calc);

	op = *calc->str;
	if (')' == op)
	{
		return CheckSpecialOperator(calc);
	}

	op_lvl = GetPriority(op);
	status = CalcIfCan(calc, op_lvl);

	StackPush(calc->ops, &op);
	/* The Stack would never be full.
	 * there is no need to check the return value */

	if (SUCCESS != status)
	{
		calc->status = status;
		return ERROR;
	}

	++calc->str;

	return ret;
}

static stt_t CheckSpecialOperator(calc_t *calc)
{
	char op = 0;
	status_t status = 0;
	stt_t ret = OP;

	assert(calc);

	op = *calc->str;

	if (')' == op)
	{
		if (IsStackEmpty(calc->ops))
		{
			calc->status = SYNTAX_ERROR;
			return Error(calc);
		}
	}

	switch (op)
	{
		case ')':
		case '(':
			status = ManagePara(calc, &ret);
			break ;
		case '-':

			if (isdigit(*(calc->str + 1)))
			{
				return AddNumToStack(calc);
			}

			if ('(' == (*(calc->str + 1)))
			{
				return AddMultiplayByMinusOne(calc);
			}

			status = SYNTAX_ERROR;
			break ;
		default:
			return Error(calc);
	}

	if (SUCCESS != status)
	{
		calc->status = status;
		return ERROR;
	}

	++calc->str;

	return ret;
}

static stt_t GoToNum(calc_t *calc)
{
	assert(calc);

	while (isspace(*calc->str))
	{
		++calc->str;
	}

	return NUM;
}

static stt_t GoToOperator(calc_t *calc)
{
	assert(calc);

	while (isspace(*calc->str))
	{
		++calc->str;
	}

	return OP;
}

static stt_t GoToStart(calc_t *calc)
{
	assert(calc);

	while (isspace(*calc->str))
	{
		++calc->str;
	}

	return START;
}

static stt_t End(calc_t *calc)
{
	assert(calc);

	calc->status = SUCCESS;

	return END;
}

static stt_t Error(calc_t *calc)
{
	assert(calc);

	calc->status = SYNTAX_ERROR;

	return ERROR;
}

static stt_t AddNumToStack(calc_t *calc)
{
	char *run = NULL;
	double num = 0;
	int flag = 0;	/* check for only one '.' */

	assert(calc);

	num = strtod((calc->str), &calc->str);

	StackPush(calc->nums, &num); 
	/* The Stack would never be full.
	 * there is no need to check the return value */

	return NUM;
}

/*
 * 	Manege Math Ruls Functions:
 */
static status_t WriteInResult(double *result, calc_t *calc)
{
	status_t status = 0;

	assert(result);
	assert(calc);

	status = SumStack(calc);
	if (SUCCESS != status)
	{
		return status;
	}

	*result += *(double*)StackPeek(calc->nums);

	return SUCCESS;
}

static status_t CalcIfCan(calc_t *calc, priority_t lvl)
{
	char op = 0;
	priority_t stack_op_lvl = 0;

	assert(calc);

	if (!IsStackEmpty(calc->ops))
	{
		op = *(char*)StackPeek(calc->ops);
		stack_op_lvl = GetPriority(op);
	}

	/* If the priority lvl of the current op,
	 * is lower then the priority of the one in the stack.
	 * The stack should be emptyed. */
	while (lvl <= stack_op_lvl && !IsStackEmpty(calc->ops))
	{
		if (DIVIDE_BY_ZERO == Calculate(calc))
		{
			return DIVIDE_BY_ZERO;
		}

		StackPop(calc->ops);

		if (!IsStackEmpty(calc->ops))
		{
			op = *(char*)StackPeek(calc->ops);
			stack_op_lvl = GetPriority(op);
		}
	}

	return SUCCESS;
}

static status_t ManagePara(calc_t *calc, stt_t *ret)
{
	char para = 0;
	status_t status = SUCCESS;

	assert(calc);

	para = *calc->str;

	if (')' == para)
	{
		status = CalcInPara(calc);
		*ret = NUM;
	}
	else
	{
		StackPush(calc->ops, calc->str);
	}

	return status;
}

static status_t CalcInPara(calc_t *calc)
{
	char op = 0;

	assert(calc);

	op = *(char*)StackPeek(calc->ops);

	while ('(' != op)
	{
		if (DIVIDE_BY_ZERO == Calculate(calc))
		{
			return DIVIDE_BY_ZERO;
		}

		StackPop(calc->ops);

		if (IsStackEmpty(calc->ops))
		{
			return SYNTAX_ERROR;
		}

		op = *(char*)StackPeek(calc->ops);
	}

	StackPop(calc->ops);

	return SUCCESS;
}

static status_t SumStack(calc_t *calc)
{
	char op = 0;

	assert(calc);

	while (!IsStackEmpty(calc->ops))
	{
		op = *(char*)StackPeek(calc->ops);

		if ('(' == op)
		{
			return SYNTAX_ERROR;
		}

		if (DIVIDE_BY_ZERO == Calculate(calc))
		{
			return DIVIDE_BY_ZERO;
		}

		StackPop(calc->ops);
	}

	return SUCCESS;
}

static stt_t AddMultiplayByMinusOne(calc_t *calc)
{
	double num = -1;
	char multiplay = '*';

	StackPush(calc->ops, &multiplay);
	StackPush(calc->nums, &num);

	++calc->str;

	return OP;
}

/*
 * 	Check Functions:
 */
static int IsOperator(char ch)
{
	int ret = 0;

	switch (ch)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '^':
			ret = 1;
	}

	return ret;
}

static priority_t GetPriority(char ch)
{
	assert(ch);

	switch (ch)
	{
		case '+':
		case '-':
			return L1;
		case '*':
		case '/':
			return L2;
		case '^':
			return L3;
	}

	return L4;
}

/*
 * 	Math Functions:
 */
static status_t Calculate(calc_t *calc)
{
	char op = 0;
	double num1 = 0;
	double num2 = 0;

	num1 = *(double*)StackPeek(calc->nums);
	StackPop(calc->nums);
	num2 = *(double*)StackPeek(calc->nums);
	StackPop(calc->nums);

	op = *(char*)StackPeek(calc->ops);

	switch (op)
	{
		case '+':
			Plus(calc, num1, num2);
			break ;
		case '-':
			Minus(calc, num1, num2);
			break ;
		case '*':
			Multiplay(calc, num1, num2);
			break ;
		case '/':

			if (0 == num1)
			{
				return DIVIDE_BY_ZERO;
			}

			Divide(calc, num1, num2);
			break ;

		case '^':
			Power(calc, num1, num2);
			break ;
	}

	return SUCCESS;
}

static void Plus(calc_t *calc, double num1, double num2)
{
	assert(calc);

	num1 += num2;
	StackPush(calc->nums, &num1);
}

static void Minus(calc_t *calc, double num1, double num2)
{
	assert(calc);

	num2 -= num1;
	StackPush(calc->nums, &num2);
}

static void Multiplay(calc_t *calc, double num1, double num2)
{
	assert(calc);

	num1 *= num2;
	StackPush(calc->nums, &num1);
}

static void Divide(calc_t *calc, double num1, double num2)
{
	assert(calc);

	num2 /= num1;
	StackPush(calc->nums, &num2);
}

static double PosPower(double num1, double num2)
{
	double origin = num2;

	if (0 == num1)
	{
		num2 = 1;
	}
	while (1 < num1)
	{
		num2 *= origin;
		--num1;
	}

	return num2;
}

static double NegPower(double num1, double num2)
{
	double origin = num2;

	while (1 > num1)
	{
		num2 /= origin;
		++num1;
	}

	return num2;
}

static void Power(calc_t *calc, double num1, double num2)
{
	assert(calc);

	if (0 > num1)
	{
		num2 = NegPower(num1, num2);
	}
	else
	{
		num2 = PosPower(num1, num2);
	}

	StackPush(calc->nums, &num2);
}

