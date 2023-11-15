#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    pid_t childpid;
    char string[] = "Hello, world\n";
    char readbuffer[80];

    int result = pipe(fd);
    if (result < 0)
    {
        printf("Error while creating file\n");
        exit(1);
    }

    childpid = fork();
    if (childpid == -1)
    {
        printf("Error while fork\n");
        exit(1);
    }

    if (childpid == 0)
    {
        close(fd[0]);
        printf("Child writing to the pipe\n");
        write(fd[1], string, sizeof(string));
        printf("Written to a file\n");
        exit(0);
    }
    else
    {
        close(fd[1]);
        wait(NULL);
        printf("Parent reading from the pipe\n");
        read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s \n", readbuffer);
        exit(0);
    }

    return 0;
}
