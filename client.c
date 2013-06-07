#include "stdio.h"
#include "strings.h"
#include "sys/socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "stdlib.h"
#include "unistd.h"


int main(int argc, char **argv)
{
	int sockfd;
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("socket error");		
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5000);
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		printf("inet_pton error\n");
		return -1;
	}
	if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		printf("Connect Error\n");
	}
	int n;
	const int MAXLINE = 1024;
	char recvline[MAXLINE];
	int sum = 0;
	while((n = read(sockfd,recvline,MAXLINE)) >0)
	{
		recvline[n] = 0;
		if(fputs(recvline,stdout) == EOF)
			printf("fputs error");
	}	
	if(n < 0)
		printf("read error");
	return 0;
}
