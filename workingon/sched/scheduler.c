
#include <stdlib.h>     /* malloc() */
#include <string.h>     /* memset() */
#include <time.h>       /* time_t */
#include <assert.h>     /* assert() */
#include <unistd.h>     /* sleep() */

#include "scheduler.h"
#include "task.h"
#include "pque.h"
#include "uid.h"


struct operation_scheduler
{
	pque_t *que;
	int to_stop;
};


static int IsBefore(const void *new_data, const void *list_data, void *param)
{
	task_t *new = 0;
	task_t *list = 0;

	(void)param;

	assert(new_data);
	assert(list_data);

	new = (task_t*)new_data;
	list = (task_t*)list_data;

	return (TaskGetTimeToRun(new) < TaskGetTimeToRun(list));
}

op_sch_t *OperSchedCreate()
{
	op_sch_t *scher = 0;

	scher = (op_sch_t*)malloc(sizeof(op_sch_t));
	if (!scher)
	{
		return NULL;
	}

	scher->que = PqueCreate(IsBefore, 0);
	if (!scher->que)
	{
		free(scher);
		return NULL;
	}
	scher->to_stop = 1;

	return scher;
}

void OperSchedDestroy(op_sch_t *oper_sch)
{
	assert(oper_sch);

	OperSchedClear(oper_sch);

	PqueDestroy(oper_sch->que);
	memset(oper_sch, 0 , sizeof(op_sch_t));
	free(oper_sch);
	oper_sch = NULL;
}


unique_id_t OperSchedAdd(op_sch_t *oper_sch, size_t interval, func_t Func, void *param)
{
	task_t *task = 0;

	assert(oper_sch);
	assert(Func);

	task = TaskCreate(interval, Func, param);
	if (!task)
	{
		return bad_uid;
	}

	if (PEnqueue(oper_sch->que, task))
	{
		TaskDestroy(task);
		return bad_uid;
	}

	return TaskGetUid(task);
}

size_t OperSchedSize(op_sch_t *oper_sch)
{
	assert(oper_sch);

	return PqueSize(oper_sch->que);
}

int OperSchedIsEmpty(op_sch_t *oper_sch)
{
	assert(oper_sch);

	return PqueIsEmpty(oper_sch->que);
}

void OperSchedClear(op_sch_t *oper_sch)
{
	assert(oper_sch);

	while (!PqueIsEmpty(oper_sch->que))
	{
		TaskDestroy(PquePeek(oper_sch->que));
		PDequeue(oper_sch->que);
	}
}

static int RunTaskIsMatch(const void *data, void *param)
{
	unique_id_t *uid = param;
	task_t *task = (task_t*)data;

	if (TaskIsMatch(task, *uid))
	{
		TaskDestroy(task);
		return 0;
	}

	return 1;
}

int OperSchedRemove(op_sch_t *oper_sch, unique_id_t uid)
{
	size_t size_before = 0;
	size_t size_after = 0;

	assert(oper_sch);

	size_before = OperSchedSize(oper_sch);

	PqueErase(oper_sch->que, RunTaskIsMatch, &uid);

	size_after = OperSchedSize(oper_sch);

	if (size_before == size_after)
	{
		return 1;
	}

	return 0;
}

void OperSchedStop(op_sch_t *oper_sch)
{
	assert(oper_sch);

	oper_sch->to_stop = 0;
}

status_t OperSchedRun(op_sch_t *oper_sch)
{
	task_t *task = 0;
	unsigned int run_time = 0;
	run_status_t check = 0;

	assert(oper_sch);

	while (oper_sch->to_stop && !OperSchedIsEmpty(oper_sch))
	{
		task = PquePeek(oper_sch->que);
		PDequeue(oper_sch->que);
		run_time = TaskGetTimeToRun(task);

		run_time = (run_time < time(0)) ? 0 : (run_time - time(0));

		while (run_time)
		{
			run_time = sleep(run_time);
		}

		check = TaskRun(task);
		if (DO_NOT_REPEAT == check)
		{
			TaskDestroy(task);
		}
		else
		{
			TaskUpdateTimeToRun(task);
			if (PEnqueue(oper_sch->que, task))
			{
				return FAIL;
			}
		}
	}

	oper_sch->to_stop = 1;
	if (OperSchedIsEmpty(oper_sch))
	{
		return IS_EMPTY;
	}

	return STOPPED;
}
