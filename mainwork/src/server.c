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
#include <unistd.h>

#define SERVER_MSG "[tcp-server] "
#define DEBUG_PRINT(fmt, ...) printf(SERVER_MSG fmt, ##__VA_ARGS__)

#define WRITE 1
#define READ 0
#define IN 0
#define OUT 1
#define ERR 2


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

		FILE *server_feedback_file = fopen("server_feedback.txt", "w");
		if(server_feedback_file == 0) {
			DEBUG_PRINT("Error: Failed to open file feedback file\n");
			exit(EXIT_FAILURE);
		}

		/* 8. packet */
		int max_len = 1024;
		char packet[max_len];
		memset(packet, 0, sizeof(char) * max_len);

		char feedback[1024];
		memset(feedback, 0, sizeof(char) * max_len);

		/* 9. recv */
		int packet_len;
		int bytes_recv = 0;
		char ack[] = "ACK";

		int childToParent[2];
		int ParentTochild[2];
		if(pipe(ParentTochild) == -1)
		{
		    perror("ParentTochild pipe initialization error");
		    exit(-1);
		}
		if(pipe(childToParent) == -1)
		{
		    perror("childToParent pipe initialization error");
		    exit(-1);
		}

		char buf[1024];
		int len;
		while( (packet_len = recv(client_socket, packet, max_len, 0)) > 0 ) {
			if(fork() == 0){
				close(childToParent[READ]);
				dup2(childToParent[WRITE], OUT);
				close(childToParent[WRITE]);

				close(ParentTochild[WRITE]);
				dup2(ParentTochild[READ], IN);
				close(ParentTochild[READ]);
				
				execl("/bin/gzip","gzip","-df",NULL);
				exit(0);
			} else {
				close(ParentTochild[READ]);
				write(ParentTochild[WRITE], packet, packet_len);
				close(ParentTochild[WRITE]);

				close(childToParent[WRITE]);
				len = read(childToParent[READ], buf, 1024);
				close(childToParent[READ]);
				fwrite(buf, sizeof(char), len, server_file);
				bytes_recv += packet_len;
				fwrite(buf,sizeof(char), len, server_feedback_file);
			}
			fclose(server_feedback_file);

			FILE *server_feedback_file_read = fopen("server_feedback.txt", "r");
			if(server_feedback_file == 0) {
				DEBUG_PRINT("Error: Failed to open file feedback file\n");
				exit(EXIT_FAILURE);
		}

			int feedback_len;
			printf("Entering Feedback Process\n");
			if( (feedback_len = fread(feedback,sizeof(char),1024, server_feedback_file_read))>0){
				printf("Feedback Process\n");
				if(fork() == 0){
					printf("Child Process\n");
					close(ParentTochild[WRITE]);
					dup2(server_socket, OUT);
					dup2(ParentTochild[READ],IN);
					close(ParentTochild[READ]);
					close(server_socket);

//					int len = read(ParentTochild[READ],buf,1024);
//					fputs(buf,stdout);

					execl("/bin/gzip","gzip","-cf",NULL);
				} else {
					printf("Parent Process\n");
					close(ParentTochild[READ]);
					write(ParentTochild[WRITE],feedback, feedback_len);
					close(ParentTochild[WRITE]);
				}
			}
		}
		DEBUG_PRINT("Received %d bytes\n", bytes_recv);

		/* 10. shutdown */
		close(client_socket);
		fclose(server_file);
	}

	close(server_socket);
}
