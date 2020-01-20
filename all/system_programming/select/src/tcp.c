
#include <sys/types.h>      // socket()
#include <sys/socket.h>     // socket()
#include <string.h>         // memset()
#include <stddef.h>         // size_t
#include <netdb.h>          // struct sockaddr_in()
#include <stdio.h>          // printf()
#include <stdlib.h>           // exit()
#include <unistd.h>         // close()

#include "tcp.h"

int TCPSocket()
{
    int ret = socket(AF_INET, SOCK_STREAM, 0);

    if (0 > ret)
    {
        printf("bad socket\n\n");
        exit(0);
    }

    return ret;
}

void TCPInitAddr(struct sockaddr_in *addr)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(PORT);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    // addr->sin_addr.s_addr = INADDR_ANY;
}

int BindAndListen(int *sockfd, struct sockaddr_in *addr)
{
    int ret = bind(*sockfd, (struct sockaddr *)addr, sizeof(struct sockaddr));

    if (ret)
    {
        perror("bad bind TCP\n\n");
        exit(0);
    }

    listen(*sockfd, 10);

    return ret;
}

void Accept(int sockfd)
{
    char msg[] = "From Server";
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int connfd = accept(sockfd, (struct sockaddr*)&addr, &len);

    TCPRecv(connfd);
    TCPSend(connfd, msg);

    close(connfd);
}

void TCPSend(int sockfd, char *msg)
{
    printf("-- in tcp send \n\n");
    ssize_t ret = send(sockfd, msg, strlen(msg), 0);

    printf("-- in send ret is %ld\n\n", ret);
    if (0 >= ret)
    {
        printf("bad send\n\n");
        exit(0);
    }
}

void TCPRecv(int sockfd)
{
    ssize_t n = 0;
    char buff[BUFF_SIZE];

    n = recv(sockfd, buff, BUFF_SIZE - 1, 0);
    buff[n] = '\0';
    printf("Recv:\t '%s'\n", buff);
}

void TCPConnect(int sockfd, struct sockaddr_in *addr)
{
    socklen_t len = sizeof(struct sockaddr);

    if (connect(sockfd, (const struct sockaddr *)addr, len))
    {
        printf("-- bad TCP connect\n\n");
    }
}
