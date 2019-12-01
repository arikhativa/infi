
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef union sem_un
{
    int val;
    struct semid_ds *buf;
    unsigned short *arr;
    struct seminfo *__buf;
} sem_un_t;

int SemVCreate(key_t key, int flags)
{
    return semget(key, 1, flags);
}

int SemVDestroy(int sem_id)
{
    sem_un_t dump;

    return semctl(sem_id, 1, IPC_RMID, dump);
}

int SemVInit(int sem_id)
{
    sem_un_t arg;
    unsigned short val = 1;
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
