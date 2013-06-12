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
	
	createPipe();
	/* Server */
	this->addr = addr;
	this->port = port;
	memset(&server_sa, 0, sizeof(server_sa));
	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(port);
	inet_pton(server_sa.sin_family, (char *)addr, &(server_sa.sin_addr));
	client_len = sizeof(client_sa);
	inet_ntop(client_sa.sin_family, &client_sa.sin_addr, client_addr, sizeof(client_addr));
	memset(this->packet, 0, sizeof(char) * MAX_LEN);

	/* Client */
	
}
char const* SocketPipe::getAddr(){
	return addr;
}
int SocketPipe::getPort(){
	return port;
}
void SocketPipe::send(char* buf, int len){
	
}

void SocketPipe::receive(){
	while( (packet_len = recv(client_socket, packet, MAX_LEN, 0)) > 0 ) {
			if(fork() == 0){
				close(childToParent[READ]);
				//dup2(childToParent[WRITE], OUT);
				close(childToParent[WRITE]);

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
				//len = read(childToParent[READ], buf, 1024);
				close(childToParent[READ]);
			}
		}
}

void SocketPipe::connect(){
 	server_socket = socket(AF_INET, SOCK_STREAM, 0);

 	if(server_socket < 0) 
 	{
 		perror("Error: Failed to create socket!\n");
 		exit(EXIT_FAILURE);
 	}	
 	if(bind(server_socket, (struct sockaddr *)&server_sa, sizeof(server_sa)) < 0) 
 	{
 		perror("Error: Failed to bind!\n");
 		exit(EXIT_FAILURE);
 	}
 	if(listen(server_socket, 4) < 0) 
 	{
 		perror("Error: Failed to listen\n");
 		exit(EXIT_FAILURE);
 	}
 	client_socket = accept(server_socket, (struct sockaddr *) &(client_sa), &(client_len));	
 	if(this->client_socket < 0) {
 		perror("Error: Failed to accept!\n");
 		exit(EXIT_FAILURE);
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
