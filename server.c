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

	//Create pipes; 
	//xPipefd[0] refers to the read end
	//xPipefs[1] refers to the write end
	int inPipefd[2];
	int outPipefd[2];
	char inBuffer;
	char outBuffer;

	if(pipe(inPipefd) == -1){
		printf("input Pipe creating error");
		return -1;
	}
	if(pipe(outPipefd) == -1){
		printf("output Pipe creating error");
		return -1;
	}


	/*
	 * Duplicate FD to connect two ends of input Pipe
	 */ 
	dup2(inPipefd[0],listenfd);//read in data from socket
	dup2(inPipefd[1],stdout);//pipe out compressed data from xz

	dup2(outPipefd[0],stdin);//read in data from xz (xz give compressed data to stdout, pipe read in from stdin)
	dup2(outPipefd[1],listenfd);//pipe out to socket

	for(;;){
		connfd = accept(listenfd,(struct sockaddr *)NULL,NULL);
		printf("Connected");
		ticks = time(NULL);
		write(connfd,ctime(&ticks),strlen(ctime(&ticks)));

		/*
		 * When data arrives, compress it with xz,
		 * the output of xz goes to stdout
		 */
		 if(connfd == -1)
		 {
			printf("acception error");
		 }
		 else
		 {
		 	/*
		 	 * Fork children processes
		 	 */
		 	//Child Process 1
			pid_t child1Pid;
			child1Pid = fork();
			if(child1Pid == -1)
			{
				printf("fork error\n");
				return -1;
			}
			if(child1Pid == 0)
			{

			}
			else
			{

			}
			
		 	//Child Process 2
			pid_t child2Pid;
			child2Pid = fork();
			if(child2Pid == -1){
				printf("fork error\n");
				return -1;
			}
			if(child1Pid == 0)
			{

			}
			else
			{
				
			}
		 }
		close(connfd);
	}
}
