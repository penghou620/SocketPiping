#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int
main(int argc, char *argv[])
{
    int pipe1fd[2];
    int pipe2fd[2];
    int pc[2];
    int cp[2];
    
    
    pid_t c1pid;
    pid_t c2pid;
    char buf1;
    char buf2;
    char ch;

   if (pipe(pc) == -1) {
        perror("pipe pc");
        exit(EXIT_FAILURE);
    }
    if (pipe(cp) == -1) {
        perror("pipe cp");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe1fd) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2fd) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    c1pid = fork();
    if (c1pid == -1) {
        perror("fork1");
        exit(EXIT_FAILURE);
    }
    if (c1pid == 0) {    
        printf("Child\n");

        dup2(pc[0],0);
        close(pc[1]);
        while(read(pc[0],&buf1,1)>0)
        {
            write(1,&buf1,1);
        }
 
    } else {            
        printf("Parent\n");

        close(pc[0]);
        write(pc[1],"abcdefghij",10);
        close(pc[1]);
        exit(EXIT_SUCCESS);

    }
        printf("Finish\n");
    return 0;
}