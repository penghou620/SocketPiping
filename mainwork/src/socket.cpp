#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "socket.h"

#define WRITE 1
#define READ 0
#define IN 0
#define OUT 1
#define ERR 2
#define MAX_LEN 1024

SocketPipe::SocketPipe(char const* addr, int port){
	
	this->addr = addr;
	this->port = port;
	createPipe();
	/* Client */
	initAddrStruct();
	createSocket();	
	connectServer();
}

char const* SocketPipe::getAddr(){
	return addr;
}
int SocketPipe::getPort(){
	return port;
}
void SocketPipe::initAddrStruct(){
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
}
void SocketPipe::createSocket(){
	if((server_socket = socket(AF_INET, SOCK_STREAM, 0))< 0){
		printf("Socket Create error\n");
	}
}
void SocketPipe::send(char const* buf, int buf_len){
	if(fork() == 0){
		close(parentToChild[WRITE]);
		dup2(server_socket, OUT);
		dup2(parentToChild[READ], IN);
		close(parentToChild[READ]);
		close(server_socket);
		execl("/bin/gzip","gzip","-c",NULL);
	} else {
		close(parentToChild[READ]);
		write(parentToChild[WRITE], buf, buf_len);
		close(parentToChild[WRITE]);
	}
}

void SocketPipe::connectServer(){
	if(inet_pton(AF_INET, addr, &(servaddr.sin_addr)) <= 0)
	{
		perror("inet_pton error\n");
		exit(-1);
	}
	if(connect(server_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		printf("Connect Error\n");
		exit(-1);
	}

}

void SocketPipe::createPipe(){
 	if(pipe(parentToChild) == -1)
 	{
 	    perror("ParentTochild pipe initialization error");
 	    exit(-1);
 	}
 	if(pipe(childToParent) == -1)
 	{
 	    perror("childToParent pipe initialization error");
 	    exit(-1);
 	}	
}
