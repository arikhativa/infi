
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
#define PORT 4200

int main()
{
    int sockfd = 0;
    char buff[BUFF] = {0};
    char msg[] = "-- Ping";
    struct sockaddr_in servaddr;
    int n = 0;
    int len = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
	    printf("client: error socket\n");
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while (1)
    {
        ssize_t tmp = sendto(sockfd, msg, sizeof(msg), MSG_CONFIRM, (struct sockaddr *)&servaddr, sizeof(servaddr));

        printf("client: ret from sendto %ld\n", tmp);
        printf("client: msg snet\n");
        n = recvfrom(sockfd, buff, BUFF, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        buff[n] = '\0';
        printf("client: recv this :\n%s\n", buff);
    }

    close(sockfd);


    return 0;
}
