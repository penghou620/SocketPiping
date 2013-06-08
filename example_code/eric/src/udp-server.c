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

#define MYPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 1024

#define SERVER_MSG "[udp-server] "
#define DEBUG_PRINT(fmt, ...) printf(SERVER_MSG fmt, ##__VA_ARGS__)

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    FILE *server_file = 0;
    while(1) {
        DEBUG_PRINT("waiting to recvfrom...\n");

        int total_bytes = 0;
        addr_len = sizeof their_addr;
        /* recv */
        while((numbytes = recvfrom(sockfd, buf, MAXBUFLEN , 0,
              (struct sockaddr *)&their_addr, &addr_len)) > 0) {

            /* open file */
            if(!server_file) {
                char fname[] = "data/dump.server";
                server_file = fopen(fname, "w");
                if(!server_file) {
                    fprintf(stderr, "failed to open %s\n", fname);
                    exit(1);
                }
            }

            fwrite(buf, sizeof(char), numbytes, server_file);
            //printf("%s\n", buf);
            total_bytes += numbytes;
        }

        /* close file */
        if(server_file) {
            fclose(server_file);
            server_file = 0;
        }

        DEBUG_PRINT("got packets from %s\n",
            inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s));
        DEBUG_PRINT("file is %d bytes long\n", total_bytes);
    }

    close(sockfd);

    return 0;
}