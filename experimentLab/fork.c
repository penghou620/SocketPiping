#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1
#define IN 0
#define OUT 1

int
main(int argc, char *argv[])
{
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

    // printf("pc = [%d, %d]\n", pc[0], pc[1]);
    // printf("cp = [%d, %d]\n", cp[0], cp[1]);

    c1pid = fork();
    printf("PID is %d\n",c1pid);
    if (c1pid == -1) {
        perror("fork1");
        exit(EXIT_FAILURE);
    }
    if (c1pid == 0) {    
        printf("First Child\n");
        printf("PID is %d\n",c1pid);
        // close(cp[READ]);    
        // dup2(cp[WRITE],OUT);
        // close(cp[WRITE]);

        // close(pc[WRITE]);
        // dup2(pc[READ],IN);
        // close(pc[READ]);

        // while(read(IN,&buf1,1) > 0)
        // execl("/bin/gzip","gzip",NULL); 
 
    } else {            
        printf("Parent of child 1\n");
        printf("PID is %d\n",c1pid);
        // close(pc[READ]);
        // write(pc[WRITE],"abcdefghij\n",11);
        // close(pc[WRITE]);

        // close(cp[WRITE]);

        // dup2(cp[READ],IN);
        // close(cp[READ]);

        // execl("/bin/gzip","gzip","-df",NULL); 

        // // close(pc[WRITE]);
        exit(EXIT_SUCCESS);

    }
    c2pid = fork();
    if (c2pid == -1) {
        perror("fork2");
        exit(EXIT_FAILURE);
    }
    if (c2pid == 0) {    
        printf("Second Child Process\n");
        
        // close(cp[READ]);    
        // dup2(cp[WRITE],OUT);
        // close(cp[WRITE]);

        // close(pc[WRITE]);
        // dup2(pc[READ],IN);
        // close(pc[READ]);

        // // while(read(IN,&buf1,1) > 0)
        // execl("/bin/gzip","gzip",NULL); 
        // exit(EXIT_SUCCESS);
 
    } else {            
        printf("Parent of child 2\n");

        // close(pc[READ]);
        // write(pc[WRITE],"abcdefghij\n",11);
        // close(pc[WRITE]);

        // close(cp[WRITE]);

        // dup2(cp[READ],IN);
        // close(cp[READ]);

        // execl("/bin/gzip","gzip","-df",NULL); 

        // // close(pc[WRITE]);
        // exit(EXIT_SUCCESS);

    }
        printf("PID is %d\n",c1pid);
        printf("Finish\n");
    return 0;
}