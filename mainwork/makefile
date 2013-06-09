all : server.o client.o

.PHONY : all

server.o : src/server.c
	cc src/server.c -o bin/server
#server : bin/server.o
#	cc -o bin/server bin/server.o
client.o : src/client.c
	cc src/client.c -o bin/client
#client : bin/client.o
#	cc -o bin/client bin/client.o

clean : 
	rm bin/client bin/server
