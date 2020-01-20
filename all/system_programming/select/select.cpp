
#include <sys/select.h>     /*  select() */
#include <unistd.h>    		/*  close() */
#include <stdio.h>			// printf()
#include <stdlib.h>			// exit()
#include <cstring>			// strcmp()

#include "udp_server.hpp"
#include "tcp_server.hpp"
#include "broadcast_server.hpp"

using namespace hrd11;

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}

int main()
{
	struct timeval tv = {0};
	BroadcastServer bcast;
	UDPServer udp;
	TCPServer tcp;
    int fds = 1;
    fd_set rset;

    FD_ZERO(&rset);

	tv.tv_sec = 5;

	fds = max(fds - 1, udp.m_sockfd) + 1;
	fds = max(fds - 1, tcp.m_sockfd) + 1;
	fds = max(fds - 1, bcast.m_sockfd) + 1;
	fds = max(fds - 1, STDIN_FILENO) + 1;

	int i = 0;
	while (i < 5)
	{
		FD_SET(udp.m_sockfd, &rset);
		FD_SET(tcp.m_sockfd, &rset);
		FD_SET(bcast.m_sockfd, &rset);
		FD_SET(STDIN_FILENO, &rset);

		select(fds, &rset, NULL, NULL, &tv);

	    if (FD_ISSET(udp.m_sockfd, &rset))
	    {
			printf("-- in UDP\n");
	        udp.RecvSend();
	    }
	    if (FD_ISSET(tcp.m_sockfd, &rset))
	    {
			printf("-- in TCP\n");
	        tcp.AcceptAndSend();
			// tcp.SSS();
	    }
	    if (FD_ISSET(bcast.m_sockfd, &rset))
	    {
			printf("-- in Broadcast\n");
			bcast.RecvSend();
	    }
	    if (FD_ISSET(STDIN_FILENO, &rset))
	    {
			char msg[BUFF_SIZE] = {0};
			read(STDIN_FILENO, msg, BUFF_SIZE -1);

			printf("-- in sdin\n");
			if (!strcmp(msg, "ping\n"))
			{
				printf("-- %s\n", "pong");
			}
			if (!strcmp(msg, "quit\n"))
			{
				printf("-- %s\n", "quiting the program..\n");
				exit(0);
			}

	    }

		++i;
	}

    return 0;
}
