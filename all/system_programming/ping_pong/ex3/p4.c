
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define BUFF_S 20

typedef struct msg
{
    size_t type;
    char text[BUFF_S];
} msg_t;

int main()
{
    key_t key = {0};
    int msg_id = 0;
    msg_t m1 = {0};

    key = ftok("/tmp/key", 42);

    msg_id = msgget(key, 0666 | IPC_CREAT);

    msgrcv(msg_id, &m1, sizeof(msg_t), 0, 0);

    printf("-- P4 rsev this msg:\n");
    printf("-- %s\n\n", m1.text);

    msgctl(msg_id, IPC_RMID, NULL);


    return 0;
}
