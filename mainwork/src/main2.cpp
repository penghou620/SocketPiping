#include "socket.h"
#include <string>
#include <iostream>
int main()
{
char const* addr = "127.0.0.1";
SocketPipe mySocket1(addr,4500);
mySocket1.createClient();
mySocket1.connectServer();
char const* buf = "Hello world";
mySocket1.send(buf, 11);
return 0;
}
