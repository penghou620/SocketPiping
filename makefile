all : server client
.PHONY : all

server.o : server.c
	cc -c server.c
server : server.o
	cc -o server server.o
client.o : client.c
	cc -c client.c
client : client.o
	cc -o client client.o

cleano : 
	rm client.o server.o
