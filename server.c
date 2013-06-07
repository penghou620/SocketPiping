#include <time.h>
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
	int MAXLINE = 1024;
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;
	listenfd = socket(AF_INET, SOCK_STREAM,0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(5000);
	bind(listenfd,(struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 1024);
	for(;;){
		connfd = accept(listenfd,(struct sockaddr *)NULL,NULL);
		printf("Connected");
		ticks = time(NULL);
		write(connfd,ctime(&ticks),strlen(ctime(&ticks)));
		close(connfd);
	}
}