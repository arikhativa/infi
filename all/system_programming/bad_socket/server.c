
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>


#define BUFF 100
#define PORT "4200"


void sigchld_handler(int s)
{
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *GetAddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int ac, char** av)
{
    int sockfd = 0;
    int new_fd = 0;
    int numbytes = 0;
    int rv = 0;
    char buf[BUFF];
    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char msg[] = "heeey there\n";


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    rv = getaddrinfo(NULL, PORT, &hints, &servinfo);

    printf("hey\n\n");

    p = servinfo;
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);


    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    bind(sockfd, p->ai_addr, p->ai_addrlen);

    freeaddrinfo(servinfo);

    listen(sockfd, 10);


    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    printf("server: waiting for connections...\n\n");


    while (1)
    {
        sin_size - sizeof(their_addr);

        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        inet_ntop(their_addr.ss_family, GetAddr((struct sockaddr *)&their_addr), s,sizeof(s));

        printf("server: got connection from %s\n", s);

        if (!fork())
        {
            close(sockfd);
            send(new_fd, msg, sizeof(msg), 0);
            close(new_fd);
            exit(0);
        }
    }
    close(new_fd);

    return 0;
}
