
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define BUFF_SIZE 5

void WritePing(int write_fd)
{
    write(write_fd, "Ping", BUFF_SIZE);
}

void WritePong(int write_fd)
{
    write(write_fd, "Pong", BUFF_SIZE);
}

void ReadPipe(int read_fd)
{
    char buff[BUFF_SIZE] = {0};

    read(read_fd, buff, BUFF_SIZE);

    printf("-- pid %d\t%s\n", getpid(), buff);
}

int main()
{
    pid_t child = {0};

    int pipes1[2] = {0};
    int pipes2[2] = {0};

    int ch_read = 0;
    int ch_write = 0;
    int pa_read = 0;
    int pa_write = 0;

    int i =0;

    pipe(pipes1);
    pipe(pipes2);

    ch_read = pipes1[0];
    ch_write = pipes2[1];

    pa_write = pipes1[1];
    pa_read = pipes2[0];

    child = fork();

    if (0 == child)
    {
        for(i = 0; i < 10; ++i)
        {
            WritePing(ch_write);
            ReadPipe(ch_read);
        }
    }
    else
    {
        for(i = 0; i < 10; ++i)
        {
            WritePong(pa_write);
            ReadPipe(pa_read);
        }
    }

    return 0;
}
