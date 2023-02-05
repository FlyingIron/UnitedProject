#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/*В торопях взял и стер первое задание по UDP протоколу, Можете пожалуйста защитать это(доделанное)
И за первое так же, ведь в первом просто нужно было наладить работу, а это сразу усовершенствованное
первое*/

int main(int argc, char ** argv)
{
	int Sockfd;
	int N, Len, Port;
	char SendLine[1000], RecvLine[1000];
	struct sockaddr_in servaddr, cliaddr;

	if (argc != 3)
	{
		printf("Usage: ProgramName <IP address> <Port>\n");
		exit(1);
	}

	if((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(0);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(Sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0)
	{
		perror(NULL);
		close(Sockfd);
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));

	if(inet_aton(argv[1], &servaddr.sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(Sockfd);
		exit(1);
	}

	while(1)
	{
		printf("Print your string => ");
		fgets(SendLine, 1000, stdin);
		if(atoi(&SendLine[0]) == 1)
			{
				break;
			}
		if(sendto(Sockfd,  SendLine, strlen(SendLine) + 1, 0, (struct sockaddr*) &servaddr,
		sizeof(servaddr)) < 0)
		{
			perror(NULL);
			close(Sockfd);
			exit(1);
		}

		if((N = recvfrom(Sockfd, RecvLine, 1000, 0, (struct sockaddr *) NULL, NULL)) < 0)
		{
			perror(NULL);
			close(Sockfd);
			exit(1);
		}

		printf("Client Recieved from server: %s", RecvLine);
	}

	close(Sockfd);
	return 0;
}