all: tcp-source udp-source

# Binary files
bin: bin-source
	bin/bin-maker

bin-source: bin-maker.o

bin-maker.o: src/bin-maker.c
	gcc src/bin-maker.c -o bin/bin-maker

# TCP
tcp-source: tcp-server.o tcp-client.o

tcp-server.o: src/tcp-server.c
	gcc src/tcp-server.c -o bin/tcp-server

tcp-client.o: src/tcp-client.c
	gcc src/tcp-client.c -o bin/tcp-client

# UDP
udp-source: udp-server.o udp-client.o

udp-server.o: src/udp-server.c
	gcc src/udp-server.c -o bin/udp-server

udp-client.o: src/udp-client.c
	gcc src/udp-client.c -o bin/udp-client