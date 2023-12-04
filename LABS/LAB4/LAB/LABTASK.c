// Your task is to create a C program that enables two processes, a parent, and a child, to
// communicate using pipes. The program operates in an iterative fashion, allowing the parent
// process (P) to send messages to the child process (C) based on user input. Conversely, the child
// process (C) can respond with messages to the parent process (P) also based on user input. This
// communication loop continues until one of the processes decides to conclude the conversation
// by entering "exit" Your mission is to implement this interactive conversation mechanism
// effectively.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
int main()
{
    int fd[2];
    int fd2[2];
    pid_t child;

    char buff[1024];
    int pipe1 = pipe(fd);
    int pipe2 = pipe(fd2);
    if (pipe1 > 0 || pipe2 > 0)
    {
        printf("Error in pipe()\n");
        exit(0);
    }
    child = fork();

    while (1)
    {
        if (child == 0)
        {
            close(fd[0]);
            printf("Child writing to the pipe\n");
            fgets(buff, sizeof(buff), stdin);
            buff[strlen(buff) - 1] = '\0';

            write(fd[1], buff, sizeof(buff));
            if (strcmp("exit", buff) == 0)
                exit(0);

            close(fd2[1]);
            printf("Child reading from the pipe\n");
            read(fd2[0], buff, sizeof(buff));
            if (strcmp("exit", buff) == 0)
                exit(0);
            printf("%s from parent\n", buff);
        }
        else
        {
            close(fd[1]);
            printf("Parent reading from the pipe\n");
            read(fd[0], buff, sizeof(buff));
            if (strcmp("exit", buff) == 0)
                break;
            printf("%s from child\n", buff);

            close(fd2[0]);
            printf("Parent writing to the pipe\n");
            fgets(buff, sizeof(buff), stdin);
            buff[strlen(buff) - 1] = '\0';

            write(fd2[1], buff, sizeof(buff));
            if (strcmp("exit", buff) == 0)
                break;
        }
        wait(NULL);
    }

    return 0;
}
