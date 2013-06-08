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
    // printf("cpid is %d\n", cpid);
    // printf("cpid is %d\n", cpid);
    if (c1pid == -1) {
        perror("fork1");
        exit(EXIT_FAILURE);
    }
    if (c1pid == 0) {    
        printf("Child\n");

        dup2(cp[1],1);
        close(cp[0]);
        dup2(pc[0],0);
        close(pc[1]);
        // execl ("/bin/ls", "ls", "-1", (char *)0);
        while(read(pc[0],&buf1,1)>0)
        {
            printf("Child Write\n");
            // write(1,&buf1,1);
            // execl ("/bin/ls", "ls", "-1", (char *)0);
            execl("/opt/local/bin/xz","xz", (char *)0);
        }
       
        printf("Writing\n");


        // close(1);
        // dup(cp[1]);
        // close(0);
        // dup(pc[0]);
        // close(pc[1]);
        // close(cp[0]);
        // execl("/bin/ls","ls","-l",(char *)0);
        // perror("No exec");
        // exit(1);

        // close(pipe1fd[0]);
        // write(pipe1fd[1],argv[1],strlen(argv[1]));
        // close(pipe1fd[1]);
        // printf("Writing\n");
      



        // _exit(EXIT_SUCCESS);
        // while (read(pipefd[0], &buf, 1) > 0)
        //     write(STDOUT_FILENO, &buf, 1);
        // write(STDOUT_FILENO, "\n", 1);
        // close(pipefd[0]);
        // _exit(EXIT_SUCCESS);

        // printf("    cpid is %d\n", cpid);
    } else {            /* Parent writes argv[1] to pipe */
        printf("Parent\n");

        close(pc[0]);
        write(pc[1],"abcdefghij",10);
        close(pc[1]);
        printf("Parent Write\n");

        close(cp[1]);
        while(read(cp[0],&buf1,1)>0)
        {
            // printf("Parent decompress\n");
            write(1,&buf1,1);
            // write(1,"\n",1);
            // execl("/opt/local/bin/xz","xz","--decompress", (char *)0);
        }
        exit(EXIT_SUCCESS);



        // printf("Input to child:\n");
        // while(read(0,&ch,1) >0)
        // {
        //     write(pc[1],&ch,1);
        //     write(1,&ch,1);
        // }
        // close(pc[1]);
        // printf("\nOutput from child:\n");
        // close(cp[1]);
        // while(read(cp[0],&ch,1)==1)
        // {
        //     write(1,&ch,1);
        // }
        // exit(0);


        // close(pipe1fd[1]);
        // while(read(pipe1fd[0],&buf1,1)>0)
        // {
        //     printf("\nReading\n");
        //     write(STDOUT_FILENO,&buf1,1);
        // }
        // close(pipe1fd[0]);
        // exit(EXIT_SUCCESS);



        // close(pipefd[0]);          /* Close unused read end */
        // write(pipefd[1], argv[1], strlen(argv[1]));
        // close(pipefd[1]);          /* Reader will see EOF */
        // wait(NULL);                /* Wait for child */
        // exit(EXIT_SUCCESS);
        // printf("    cpid is %d\n", cpid);
    }
        printf("Finish\n");
    return 0;
}