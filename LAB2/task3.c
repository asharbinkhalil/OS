#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int numbers[5];
    int max = 0;

    printf("Enter 5 numbers:\n");
    for (int i = 0; i < 5; i++) {
        scanf("%d", &numbers[i]);
        if (numbers[i] > max) 
            max = numbers[i];
        
    }

    pid_t max_pid, evens_pid;
    max_pid = fork();

    if (max_pid == 0) { // Child process for finding maximal number
        printf("Maximal Number: %d\n", max);
        exit(0);
    } else if (max_pid > 0) {
        wait(NULL); // Wait for the child process to finish
        evens_pid = fork();
        
        if (evens_pid == 0) { // Child process for finding even numbers
            printf("Evens: ");
            for (int i = 0; i < 5; i++) {
                if (numbers[i] % 2 == 0) {
                    printf("%d ", numbers[i]);
                }
            }
            printf("\n");
            exit(0);
        } else if (evens_pid > 0) {
            wait(NULL); // Wait for the second child process to finish
        } else {
            perror("fork");
            exit(1);
        }
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
