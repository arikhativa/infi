
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
    msg_t m2 = {0};
    msg_t m3 = {0};

    key = ftok("/tmp/key", 42);

    msg_id = msgget(key, 0666 | IPC_CREAT);

    m1.type = 1;
    strcpy(m1.text, "this is msg num 1");
    m2.type = 2;
    strcpy(m2.text, "this is msg num 2");
    m3.type = 3;
    strcpy(m3.text, "this is msg num 3");

    msgsnd(msg_id, &m1, sizeof(msg_t), 0);
    msgsnd(msg_id, &m2, sizeof(msg_t), 0);
    msgsnd(msg_id, &m3, sizeof(msg_t), 0);

    return 0;
}
