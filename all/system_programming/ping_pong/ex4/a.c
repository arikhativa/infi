
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
    char text1[BUFF_S] = "msg num 1 :)";
    char text2[BUFF_S] = "msg num 2 :)";
    char text3[BUFF_S] = "msg num 3 :)";

    key = ftok("/tmp/key", 42);

    shm_id = shmget(key, PAGE, 0666 | IPC_CREAT);

    str = shmat(shm_id, (void*)0, 0);

    strcpy(str, text1);
    strcpy(str + BUFF_S, text2);
    strcpy(str + (BUFF_S * 2), text3);

    shmdt(str);

    return 0;
}
