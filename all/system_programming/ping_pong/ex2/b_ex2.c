

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
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
    char* path1 = "/tmp/fifo1";
    char* path2 = "/tmp/fifo2";
    int fd_r = 0;
    int fd_w = 0;
    int i = 0;

    mkfifo(path2, 0666);

    fd_w = open(path2, O_RDWR);
    fd_r = open(path1, O_RDWR);


    for(i=0; i < 5; ++i)
    {
        if (!i)
            printf("-- b.out in loop\n");
        WritePong(fd_w);
        ReadPipe(fd_r);
    }

    close(fd_r);
    close(fd_w);

    return 0;
}
