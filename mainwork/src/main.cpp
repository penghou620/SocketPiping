#include "socket.h"
#include <string>
#include <iostream>
int main()
{
	//std::string buf ("Hello");
	char const* addr = "127.0.0.1";
	SocketPipe mySocket(addr,4500);
	mySocket.receive();
	return 0;
}
