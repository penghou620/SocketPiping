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
	/* Common Variables */
	const char *addr;
	int port;

	/* Server Variables */
	struct sockaddr_in sv_addr;
	int sv_socket, sv_connfd;

	/* Client Variables */
	int server_socket;
	struct sockaddr_in servaddr;
	int parentToChild[2];	
	int childToParent[2];	
	void createPipe();
public:
	SocketPipe(char const* , int);
	char const* getAddr();
	int getPort();

	/* Common */
	void initAddrStruct(struct sockaddr_in *addr);
	void createSocket(int *mySocket);

	/* Server */
	void createServer();
	void bindListen();
	void receive();

	/* Client */
	void createClient();
	void connectServer();
	void send(char const* buf, int len);
};
