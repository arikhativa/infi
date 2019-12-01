
#include <stdlib.h>	/* malloc() */
#include <string.h>	/* memset() */
#include <time.h>	/* time_t */
#include <assert.h>	/* assert() */

#include "uid.h"	/* unique_id_t */
#include "task.h"	

task_t *TaskCreate(size_t interval, task_func_t Func, void *param)
{
	task_t *task = 0;

	assert(Func);

	task = (task_t*)malloc(sizeof(task_t));
	if (!task)
	{
		return NULL;
	}

	task->uid = UidCreate();
	if (UidIsBad(task->uid))
	{
		free(task);
		return NULL;
	}

	task->interval = interval;
	task->Func = Func;
	task->time_to_run = time(NULL) + interval;
	task->param = param;

	return task;
}

void TaskDestroy(task_t *task)
{
	assert(task);

	memset(task, 0, sizeof(task_t));
	free(task);
	task = NULL;
}

void TaskUpdateTimeToRun(task_t *task)
{
	assert(task);

	task->time_to_run = time(NULL) + task->interval;
}

run_status_t TaskRun(task_t *task)
{
	assert(task);

	if (task->Func(task->param))
	{
		return DO_NOT_REPEAT;
	}

	return REPEAT;
}

unique_id_t TaskGetUid(task_t *task)
{
	assert(task);

	return task->uid;
}

int TaskIsMatch(task_t *task, unique_id_t uid)
{
	assert(task);

	return UidIsSame(TaskGetUid(task), uid);
}

time_t TaskGetTimeToRun(task_t *task)
{
	assert(task);

	return task->time_to_run;
}
