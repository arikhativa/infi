
#ifndef __SEM_V_H__
#define __SEM_V_H__

#include <sys/types.h>

int SemVCreate(key_t key, int flags);
int SemVDestroy(int sem_id);
int SemVInit(int sem_id);
int SemVWait(int sem_id);
int SemVPost(int sem_id);
int SemVGetVal(int sem_id);


#endif
