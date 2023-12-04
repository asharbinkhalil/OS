#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    printf("PID of example.c = %d\n", getpid());
    pid_t p;
    p = fork();

    if (p == -1) {
        perror("fork() failed");
        return 1;
    }

    if (p == 0) {
        printf("We are in the child process\n");
        char buffer[100];

        while (1) {
            printf("\nEnter a command or 'EXIT' to terminate: ");
            scanf("%s", buffer);

            if (strcmp(buffer, "EXIT") == 0) {
                break;
            }

            if (strcmp(buffer, "execl") == 0) {
                execl("/bin/ls", "ls", "-lh", "/home", NULL);
            } else if (strcmp(buffer, "execle") == 0) {
                char *envp[] = { "PATH=/bin", NULL };
                execle("/bin/ls", "ls", "-lh", "/home", NULL, envp);
            } else if (strcmp(buffer, "execv") == 0) {
                char *args[] = { "ls", "-lh", "/home", NULL };
                execv("/bin/ls", args);
            } else if (strcmp(buffer, "execvp") == 0) {
                char *command[] = { "ls", "-lh", "/home", NULL };
                execvp("ls", command);
            } else {
                printf("Invalid command: %s\n", buffer);
            }

            perror("exec failed");  // This will only be reached if exec functions fail.
            exit(1);
        }
    } else {
        // Wait for the child process to finish
        wait(NULL);
        printf("We are in the parent process\n");
    }

    return 0;
}
