#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

#define __BUFLEN__ 1024

int main(int argc, char * argv[])
{
	int portno = 0;
	int in_socket = 0;
	int cl_socket = 0;
	socklen_t cli_addr_size;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[__BUFLEN__] = {0};

	if(argc == 2)
	{
		portno = atoi(argv[1]);
		in_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (in_socket != -1)
		{
			bzero((char *) &serv_addr, sizeof(serv_addr));
			serv_addr.sin_port = htons(portno);
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = INADDR_ANY;
			if (bind(in_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			{
				cli_addr_size = sizeof(struct sockaddr_in);
				if(listen(in_socket, 20) == 0)
				{
					cl_socket = accept(in_socket, (struct sockaddr *) &cli_addr,&cli_addr_size);
					if(cl_socket > 0)
					{
						 fprintf(stdout, "Client connected: %s\n",
											inet_ntoa(cli_addr.sin_addr));
						if(send(cl_socket, "login:", strlen("login:")+1, 0) > 0)
						{
							int recv_size = recv(cl_socket, buffer, __BUFLEN__, 0);
							if(recv_size > 0)
							{
								fprintf(stdout, "resv |%s|\n", buffer );
							}
							else
							{
								error_at_line(0, errno, __FILE__, __LINE__, "recv()");
							}
						}
						else
						{
								error_at_line(0, errno, __FILE__, __LINE__, "send()");
						}
					}
					else
					{
						error_at_line(0, errno, __FILE__, __LINE__, "accept()");
					}
				}
				else
				{
					error_at_line(0, errno, __FILE__, __LINE__, "listen()");
				}
			}
			else
			{
				error_at_line(0, errno, __FILE__, __LINE__, "bind()");
			}
			close(in_socket);
		}
		else 
		{
			error_at_line(0, errno, __FILE__, __LINE__, "socket()");
		}
	}
	else 
	{
		printf("Usage %s <port>\n", __FILE__);
	}
	return 0;
}