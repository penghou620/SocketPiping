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

#define WRITE 1
#define READ 0
#define IN 0
#define OUT 1
#define ERR 2
#define MAX_LEN 1024

class SocketPipe{
private:
	char const* addr;
	int port;
	int server_socket;
	struct sockaddr_in server_sa;
	struct sockaddr_in client_sa;
	socklen_t client_len;
	int client_socket;
	char client_addr[50];
	int childToParent[2];
	int parentToChild[2];
	int packet_len;
	char packet[MAX_LEN];
	void createPipe();
public:
	SocketPipe(char const* , int);
	char const* getAddr();
	int getPort();
	void connect();
	void send(char* buf, int len);
	void receive();
};
