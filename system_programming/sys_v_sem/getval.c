
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "sem5.h"
#include "write_print.h"

#define KEY 0x1111


int main()
{
    int sem_id = SemVCreate(KEY, 0);
    ssize_t val = 0;

    val = SemVGetVal(sem_id);
    Print("-- val is %ld\n\n", val);
}
