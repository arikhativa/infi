
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define BUFF_S 20
#define PAGE 1024

int main()
{
    key_t key = {0};
    int shm_id = 0;
    char* str = 0;

    key = ftok("/tmp/key", 42);

    shm_id = shmget(key, PAGE, 0666 | IPC_CREAT);

    str = shmat(shm_id, (void*)0, 0);

    printf("--d.out\n%s\n", str + (BUFF_S * 2));

    shmdt(str);
    shmctl(shm_id, IPC_RMID, NULL);


    return 0;
}
