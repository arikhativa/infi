/* 
 *   ver 1.1
 *
 * 	file names : scheduler.c scheduler.h scheduler_test.c
 */

#ifndef __OPERATION_SCHEDULER_H__
#define __OPERATION_SCHEDULER_H__

#include <stddef.h> /* size_t */
#include "uid.h"

typedef struct operation_scheduler op_sch_t;

/*
 *       (*action_func_t)():
 *
 *       Arguments:
 *               'param' - user's parameters.
 *
 *       Return:
 *               On success retuns 0.
 *				 On fail return 1;
 */
	typedef int (*func_t)(void *param);



typedef enum status
{
    IS_EMPTY = 0,
    STOPPED = 1,
    FAIL = 2
}status_t;

/*
 *	OperSchedCreate():                      O(1)
 *	    Creates priority oper_schue.
 *
 *	Return:
 *		Pointer to the new scheduler.
 */
	op_sch_t *OperSchedCreate();

/*
 *	OperSchedDestroy():                      O(1)
 *	    Destroy a priority oper_schue.
 *
 *	Arguments:
 *		'oper_sch' - The oper_schue to destroy.
 */
	void OperSchedDestroy(op_sch_t *oper_sch);

/*
 *	OperSchedAdd():                      O(1)
 *	Addes task to the oper_schue.
 *
 *	Arguments:
 *		'interval' - time between function runs.
 *		'func' - function pointer.
 *      'param' - param.
 *
 *	Return:
 *      uid of the created task.
 */
	unique_id_t OperSchedAdd(op_sch_t *oper_sch, size_t interval, func_t func, void *param);

/*
 *	OperSchedRemove():                      O(1)
 *	    Removes an element from the oper_schue.
 *
 *	Arguments:
 *		'oper_sch' - The oper_schue to remove from.
 *      'uid' - key to the task for removal.
 *  Return:
 *       On success retuns 0.
 *	     On fail return 1;
 */
	int OperSchedRemove(op_sch_t *oper_sch, unique_id_t uid);

/*
 *	OperSchedRun():                      O(?)
 *	Gets a pointer to the scheduler and run all functions by thier priority .
 *
 *	Arguments:
 *		'oper_sch' - The oper_schue.
 *  Return:
 *      IS_EMPTY -On success retuns 0.
 *      STOPPED - if the user stopped the run.
 *		FAIL - On fail of one of the function;
 *
 */
	status_t OperSchedRun(op_sch_t *oper_sch);
/*
 *	OperSchedStop():                      O(1)
 *	Gets a pointer to the scheduler and stop the function run.
 *
 *	Arguments:
 *		'oper_sch' - The oper_schueduler.
 *
 */
	void OperSchedStop(op_sch_t *oper_sch);
/*
 *	OperSchedIsEmpty():                      O(1)
 *	Check if the oper_sch is empty.
 *
 *	Arguments:
 *		'oper_sch' - The oper_schue to check.
 *
 *	Return:
 *		'1' - if the oper_schue is empty.
 *		'0' - if the oper_schue is NOT empty.
 */
	int OperSchedIsEmpty(op_sch_t *oper_sch);

/*
 *	OperSchedSize():                      O(n)
 *	Count the number of elements in the oper_sch.
 *
 *	Arguments:
 *		'oper_sch' - The oper_schue to count.
 *
 *	Return:
 *		Number of elements.
 */
	size_t OperSchedSize(op_sch_t *oper_sch);

/*
 *	OperSchedClear():                      O(n)
 *	Removes all the elements in the oper_schue.
 *	Does not free the oper_schue, only the elements.
 *
 *	Arguments:
 *		'oper_sch' - The oper_schue to clear.
 */
	void OperSchedClear(op_sch_t *oper_sch);

#endif /* __SCHEDULER__H__ */

