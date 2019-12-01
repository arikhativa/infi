
#ifndef __SEM_V_H__
#define __SEM_V_H__

#include <sys/sem.h>        /* key_t */

int SemVCreate(key_t key);

int SemVDestroy(int sem_id);

int SemVInit(int sem_id, unsigned short value);

int SemVGetID(key_t key);

int SemVWait(int sem_id);

int SemVPost(int sem_id);

int SemVGetVal(int sem_id);

#endif
