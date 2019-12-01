/* 
 *
 *   ver 1.1
 *
 * 	file names : task.c task.h task_test.c
 */

#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h> /* size_t */
#include <time.h> /* time_t */

#include "uid.h"


typedef struct task task_t;

/*
 *       (*action_func_t)():
 *
 *       Arguments:
 *               'data' - the data that the action would be applied to.
 *               'param' - user's parameters.
 *
 *       Return:
 *               On success retuns 0.
 *				 On fail return 1;
 */
	typedef int (*task_func_t)(void *param);

typedef enum run_status
{
    REPEAT = 0,
    DO_NOT_REPEAT = 1
}run_status_t;


struct task
{
    size_t interval;
    task_func_t Func;
    void *param;
    time_t time_to_run;
    unique_id_t uid;       
};

/*
 *	TaskCreate():                      O(1)
 *  create new task struct.
 *	Arguments:
 *		'interval' - time between function runs.
 *		'func' - function pointer.
 *      'param' - param.
 *      'time_to_run' - time the function should run.
 *	Return:
 *		Pointer to the new task.
 */
	task_t *TaskCreate(size_t interval, task_func_t func, void *param);

/*
 *	TaskDestroy():                      O(1)
 *	Destroy the task and free task sources.
 *	Arguments:
 *		'task' - pointer to task.
 */
	void TaskDestroy(task_t *task);

/*
 *	TaskUpdateTimeToRun:                      O(1)
 *	Set the time to run.
 *
 *	Arguments:
 *		'task' - pointer to task.
 *
 */
	void TaskUpdateTimeToRun(task_t *task);

/*
 *	TaskRun():                      O(1)
 *	Run the function of the current task
 *
 *	Arguments:
 *		'task' - pointer to task.
 *	Return:
 *		status to the next operation.
 *      REPEAT - run the task one more time.
 *      DO_NOT_REPEAT - do not run the task any more.
 */
	run_status_t TaskRun(task_t *task);

/*
 *	TaskGetUid                      O(1)
 *	Gets the uid from the task.
 *
 *	Arguments:
 *		'task' - pointer to task.
 * 	Return:
 *      the uid of the recieved task. 
 */
	unique_id_t TaskGetUid(task_t *task);

/*
 *	TaskIsMatch():                      O(1)
 *	check if the task's uid equal to the recived one
 *
 *	Arguments:
 *		'task' - pointer to task.
 * 	Return:
 *      0 - if uid's arent equal
 *      1 - if uid's are equal 
 *
 */
	int TaskIsMatch(task_t *task, unique_id_t uid);
/*
 *	TaskGetTimeToRun():                      O(1)
 *	gets the time to run of the task.
 *
 *	Arguments:
 *		'task' - pointer to task.
 *
 *	Return:
 *		time to run variable.
 */
	time_t TaskGetTimeToRun(task_t *task);


#endif /* __TASK__H__ */

