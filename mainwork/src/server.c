#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>

#define SERVER_MSG "[tcp-server] "
#define DEBUG_PRINT(fmt, ...) printf(SERVER_MSG fmt, ##__VA_ARGS__)

void usage() {
  printf(
    "usage: tcp-server [OPTIONS] FILE\n"
    "\n"
    "OPTIONS:\n"
    "  -a                                IP address\n"
    "  -p                                Port number\n"
    "  -H                                See this message\n"
    );
}

int main(int argc, char** argv) {
	/* 1. cmd line args */
	char *addr = "127.0.0.1";
	int port = 4500;
	char *fname = 0;
	int opt, option_index;
	while( (opt = getopt(argc, argv, "a:p:H")) != -1 ) {
		switch(opt) {
		case 'a': addr = optarg; break;
		case 'p': port = atoi(optarg); break;
		case 'H': usage(); exit(0); break;
		default:  usage(); exit(1); break;
		}
	}
	while (optind < argc) {
		if (!fname) {
		  fname = argv[optind++];
		} else {
		  printf("Error: extra arguments on command line.\n\n");
		  usage();
		  exit(1);
		}
	}
	if (!fname) {
    	printf("Error: Please supply a file!\n\n");
    	usage();
    	exit(1);
  	}
	DEBUG_PRINT("input address = %s\n", addr);
	DEBUG_PRINT("input port    = %d\n", port);
	DEBUG_PRINT("file name 	   = %s\n", fname);
	DEBUG_PRINT("\n");

	/* 2. data structures */
	struct sockaddr_in server_sa;
	memset(&server_sa, 0, sizeof(server_sa));

	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(port);
	inet_pton(server_sa.sin_family, addr, &server_sa.sin_addr);

	/* 3. TCP */
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0) {
		DEBUG_PRINT("Error: Failed to create socket!\n");
		exit(EXIT_FAILURE);
	}
	DEBUG_PRINT("Created socket\n");

	/* 4. bind */
	if(bind(server_socket, (struct sockaddr *)&server_sa, sizeof(server_sa)) < 0) {
		DEBUG_PRINT("Error: Failed to bind!\n");
		exit(EXIT_FAILURE);
	}
	DEBUG_PRINT("Bound socket\n");

	/* 5. listen */
	if(listen(server_socket, 4) < 0) {
		DEBUG_PRINT("Error: Failed to listen\n");
		exit(EXIT_FAILURE);
	}
	DEBUG_PRINT("Listening for connections\n");

	while(1) {
		/* 6. accept */
		struct sockaddr_in client_sa;
		socklen_t client_len = sizeof(client_sa);
		int client_socket = accept(server_socket, (struct sockaddr *) &client_sa,
								   &client_len);
		if(client_socket < 0) {
			DEBUG_PRINT("Error: Failed to accept!\n");
			exit(EXIT_FAILURE);
		}
		char client_addr[50];
		inet_ntop(client_sa.sin_family, &client_sa.sin_addr, client_addr,
				  sizeof(client_addr));
		DEBUG_PRINT("Accepted %s\n", client_addr);

		/* 7. file */
		FILE *server_file = fopen(fname, "w");
		if(server_file == 0) {
			DEBUG_PRINT("Error: Failed to open file %s\n", fname);
			exit(EXIT_FAILURE);
		}

		/* 8. packet */
		int max_len = 1024;
		char packet[max_len];
		memset(packet, 0, sizeof(char) * max_len);

		/* 9. recv */
		int packet_len;
		int bytes_recv = 0;
		char ack[] = "ACK";
		while( (packet_len = recv(client_socket, packet, max_len, 0)) > 0 ) {
			fprintf(stdout, "%s\n", packet);
			write(client_socket,packet,packet_len);
			fwrite(packet, sizeof(char), packet_len, server_file);
			bytes_recv += packet_len;
			//send(client_socket, ack, 4, 0);
		}
		DEBUG_PRINT("Received %d bytes\n", bytes_recv);

		/* 10. shutdown */
		close(client_socket);
		fclose(server_file);
	}

	close(server_socket);
}