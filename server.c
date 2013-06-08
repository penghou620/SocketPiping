#include <time.h>
#include "stdio.h"
#include "strings.h"
#include "sys/socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "stdlib.h"
#include "unistd.h"

#define STDOUT 1
#define STDIN 1
#define READ 0
#define WRITE 1

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




	/*
	 * Creating two Child Process
	 */
	pid_t sendChildProcess;
	pid_t receiveChildProcess;
	sendChildProcess = fork();
	ReceiveChildProcess = fork();

	for(;;){
		connfd = accept(listenfd,(struct sockaddr *)NULL,NULL);
		printf("Connected");
		ticks = time(NULL);
		write(connfd,ctime(&ticks),strlen(ctime(&ticks)));

		/*
		 * When sending data arrives, compress it, the output of compressor goes to stdout
		 */
		 if(connfd == -1)
		 {
			printf("acception error");
		 }
		 else
		 {
		 	/*
		 	 * Children's processes
		 	 */
		 	//sending child process
			if(sendChildProcess == -1)
			{
				printf("fork error\n");
				return -1;
			}
			if(sendChildProcess == 0)
			{
				dup2(outPipefd[READ],STDIN);//read in data from xz (xz give compressed data to stdout, pipe read in from stdin)
				close(outPipefd[READ]);
				dup2(outPipefd[READ],listenfd);//pipe out to socket	
				close(outPipefd[WRITE]);
				execl("/bin/gzip","gzip",NULL);
			}
			else
			{
				close(outPipefd[READ]);
			}

		 	//Reveiving Child Process 2
			if(ReceiveChildProcess == -1){
				printf("fork error\n");
				return -1;
			}
			if(ReceiveChildProcess == 0)
			{

			}
			else
			{
				
			}
		 }
		close(connfd);
	}
}
