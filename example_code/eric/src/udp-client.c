#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "timer.h"

#define SERVERPORT "4950"    // the port users will be connecting to

#define SERVER_MSG "[udp-client] "
#define DEBUG_PRINT(fmt, ...) printf(SERVER_MSG fmt, ##__VA_ARGS__)

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    FILE *client_file = fopen(argv[2], "r");
    if(client_file == NULL) {
        perror("invalid file");
        return 1;
    }

    int max_len = 1024;
    char packet[max_len];
    int packet_len;
    int total_bytes = 0;
    start_timeofday();
    start_timer();
    while( (packet_len = fread(packet, sizeof(char), max_len, client_file)) > 0 ) {
        if((numbytes = sendto(sockfd, packet, packet_len, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }
        total_bytes += numbytes;
    }
    numbytes = sendto(sockfd, packet, 0, 0,
             p->ai_addr, p->ai_addrlen);
    DEBUG_PRINT("Transfered %d bytes\n", total_bytes);
    DEBUG_PRINT("Elapsed time %f\n", end_timer());
    DEBUG_PRINT("Elapsed time %f\n", end_timeofday());

    // if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
    //          p->ai_addr, p->ai_addrlen)) == -1) {
    //     perror("talker: sendto");
    //     exit(1);
    // }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", total_bytes, argv[1]);
    close(sockfd);

    return 0;
}