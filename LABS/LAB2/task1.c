#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int P1, P2;
    
    // Create the first child process P1
    P1 = fork();
    
    if (P1 < 0) {
        perror("Failed to create P1");
        return 1;
    } else if (P1 == 0) {
        // This code block will be executed by P1
        printf("P1 (PID=%d) is the child of P (PID=%d)\n", getpid(), getppid());
        
        // Create the second child process P2
        P2 = fork();
        
        if (P2 < 0) {
            perror("Failed to create P2");
            return 1;
        } else if (P2 == 0) {
            // This code block will be executed by P2
            printf("P2 (PID=%d) is the child of P1 (PID=%d)\n", getpid(), getppid());
        } else {
            // This code block will be executed by P1
            wait(NULL); // Wait for P2 to finish
        }
    } else {
        // This code block will be executed by the initial parent process P
        printf("P (PID=%d) is the parent of P1 (PID=%d)\n", getpid(), P1);
        wait(NULL); // Wait for P1 to finish
    }
    
    return 0;
}
