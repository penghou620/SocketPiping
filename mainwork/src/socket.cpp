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
}


/*
 * Common Functions
 */
char const* SocketPipe::getAddr(){
	return addr;
}
int SocketPipe::getPort(){
	return port;
}

/*
 * ServerFunctions
 */
void SocketPipe::createServer(){
	initAddrStruct(&sv_addr);
	createSocket(&sv_socket);
	bindListen();
}
void SocketPipe::bindListen(){
	if(bind(sv_socket, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0){
		printf("Error: Failed to bind\n");
		exit(-1);
	}
		
	if(listen(sv_socket, 4) < 0){
		printf("Error: Failed to Listen\n");
		exit(-1);
	}
}
void SocketPipe::receive(){		
	printf("Listen for connection\n");
	for(;;){
		struct sockaddr_in sv_client;
		socklen_t sv_client_len = sizeof(sv_client);	
		sv_client_len = sizeof(sv_client);
		int client_socket = accept(sv_socket, (struct sockaddr *)&sv_client,&sv_client_len);

		if(client_socket < 0){
			printf("Error: Failed to accept\n");
			exit(-1);
		}
		char sv_client_addr[50];
		inet_ntop(sv_client.sin_family, &sv_client.sin_addr, sv_client_addr,sizeof(sv_client_addr));
		printf("Accepted %s\n",sv_client_addr);
		char packet[MAX_LEN];
		memset(packet, 0, sizeof(char) * MAX_LEN);
		int packet_len;
		while( (packet_len = recv(client_socket, packet, MAX_LEN, 0)) > 0) {
			fputs(packet, stdout);
			if(fork() == 0){

				close(childToParent[WRITE]);
				close(childToParent[READ]);

				close(parentToChild[WRITE]);
				dup2(parentToChild[READ], IN);
				close(parentToChild[READ]);
				
				execl("/bin/gzip","gzip","-df",NULL);
				exit(0);
			} else {
				close(parentToChild[READ]);
				write(parentToChild[WRITE], packet, packet_len);
				close(parentToChild[WRITE]);

				close(childToParent[WRITE]);
				close(childToParent[READ]);
			}
		}
	}
}

/*
 * Client Functions
 */
void SocketPipe::createClient(){
	/* Client */
	initAddrStruct(&servaddr);
	createSocket(&server_socket);	
}
void SocketPipe::initAddrStruct(struct sockaddr_in *addr){
	memset(addr, 0, sizeof(&addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
}
void SocketPipe::createSocket(int *mySocket){
	if((*mySocket = socket(AF_INET, SOCK_STREAM, 0))< 0){
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
