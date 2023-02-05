#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main (int argc, char *argv[])
{
	int my_sock, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buff[1024];

	printf("TCP testing\n");

	if (argc != 3)
	{
		printf("Usage: %s <hostname> <Port>\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);

	my_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(my_sock < 0)
	{
		error("Error opening socket");
	}

	server = gethostbyname(argv[1]);

	if(server == NULL)
	{
		error("Error, no such host");
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char*)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htons(portno);

	if(connect(my_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Error connecting");
	}

 	for(int i = 1; i<=4; i++)
 	{
 		n = recv(my_sock, &buff[0], sizeof(buff) - 1, 0);

 		buff[n] = 0;

 		if(i == 4)
 		{
 			printf("%s", buff);
 			break;
 		}

 		printf("For input=> :%s", buff);

 		printf("parameter: ");
 		fgets(&buff[0], sizeof(buff) - 1, stdin);

 		if(!strcmp(&buff[0], "quit\n"))
 		{
 			printf("Exit...");
 			send(my_sock, &buff[0], strlen(&buff[0]), 0);
 			close(my_sock);
 			return 0;
 		}

 		send(my_sock, &buff[0], strlen(&buff[0]), 0);
 	}

 	return 0;
}