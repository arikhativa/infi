
#include <sys/types.h>      /* semget(), semctl(), semop() */
#include <sys/ipc.h>        /* semget(), semctl(), semop() */
#include <sys/sem.h>        /* semget(), semctl(), semop() */

#include "sem5.h"

typedef union sem_un
{
    int val;
    struct semid_ds *buf;
    unsigned short *arr;
    struct seminfo *__buf;
} sem_un_t;

int SemVCreate(key_t key)
{
    return semget(key, 1, 0666 | IPC_CREAT);
}

int SemVGetID(key_t key)
{
    return semget(key, 1, 0);
}

int SemVDestroy(int sem_id)
{
    sem_un_t dump;

    return semctl(sem_id, 1, IPC_RMID, dump);
}

int SemVInit(int sem_id, unsigned short value)
{
    sem_un_t arg;
    unsigned short val = value;
    arg.arr = &val;

    return semctl(sem_id, 0, SETVAL, arg);
}

int SemVWait(int sem_id)
{
    struct sembuf ops = {0};

    ops.sem_num = 0;
    ops.sem_op = -1;
    ops.sem_flg = 0;

    return semop(sem_id, &ops, 1);
}

int SemVPost(int sem_id)
{
    struct sembuf ops = {0};

    ops.sem_num = 0;
    ops.sem_op = 1;
    ops.sem_flg = 0;

    return semop(sem_id, &ops, 1);
}

int SemVGetVal(int sem_id)
{
    struct sembuf arg = {0};

    return semctl(sem_id, 0, GETVAL, &arg);
}
