
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
#define PORT 8080

int main()
{
    int sockfd = 0;
    char buff[BUFF] = {0};
    char msg[] = "from yoav";
    struct sockaddr_in servaddr;
    int n = 0;
    int len = 0;
    int broadcast = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
	    printf("client: error socket\n");
    }
    setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast));

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_BROADCAST;

    while (1)
    {
        sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        
        n = recvfrom(sockfd, buff, BUFF, 0, (struct sockaddr *)&servaddr, &len);
        buff[n] = '\0';
        printf("client: recv this - %s\n", buff);
    }

    close(sockfd);


    return 0;
}
