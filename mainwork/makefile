all : server.o client.o socket.o main.o main main2.o main2

.PHONY : all

server.o : src/server.c
	cc src/server.c -o bin/server
#server : bin/server.o
#	cc -o bin/server bin/server.o
client.o : src/client.c
	cc src/client.c -o bin/client
#client : bin/client.o
#	cc -o bin/client bin/client.o
socket.o : src/socket.cpp src/socket.h
	c++ -c src/socket.cpp -o bin/socket.o

main.o : bin/socket.o src/main.cpp 
	c++ -c src/main.cpp -o bin/main.o 
main : bin/main.o bin/socket.o
	c++ -o bin/main bin/main.o bin/socket.o
main2.o : bin/socket.o src/main2.cpp 
	c++ -c src/main2.cpp -o bin/main2.o 
main2 : bin/main2.o bin/socket.o
	c++ -o bin/main2 bin/main2.o bin/socket.o


clean : 
	rm bin/client bin/server
