#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int main() {
    int data[100]; 
    int num_values;

    printf("Enter the number of values: ");
    scanf("%d", &num_values);

    printf("Enter the values separated by spaces:\n");
    for (int i = 0; i < num_values; i++) {
        scanf("%d", &data[i]);
    }
    double mean=0;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { // Child process
        double sum = 0.0;

        // Calculate mean
        for (int i = 0; i < num_values; i++) {
            sum += data[i];
        }
        mean = sum / num_values;

        // Calculate median
        double median;
        if (num_values % 2 == 0) {
            median = (data[num_values / 2 - 1] + data[num_values / 2]) / 2.0;
        } else {
            median = data[num_values / 2];
        }

        printf("Mean=%.2lf Median=%.2lff\n", mean, median);

        exit(0); // Child process exits
    } else { // Parent process
        wait(NULL); // Wait for the child process to finish
        printf("Parent Process: Child process has completed.\n");
    }


    pid_t pid_median = fork();

    if (pid_median == -1) {
        perror("fork");
        exit(1);
    } else if (pid_median == 0) { // Child process

        // Calculate median
        double median;
        if (num_values % 2 == 0) {
            median = (data[num_values / 2 - 1] + data[num_values / 2]) / 2.0;
        } else {
            median = data[num_values / 2];
        }

        printf("Median=%.2lff\n", median);

        exit(0); // Child process exits
    } else { // Parent process
        wait(NULL); // Wait for the child process to finish
        printf("Parent Process: Child process has completed.\n");
    }



    pid_t pid_std = fork();

    if (pid_std == -1) {
        perror("fork");
        exit(1);
    } else if (pid_std == 0) { 

        double squaredDifferences[num_values];
        for (int i = 0; i < num_values; i++) {
            squaredDifferences[i] = (data[i] - mean) * (data[i] - mean);
        }
        double sumSquaredDifferences = 0;
        for (int i = 0; i < num_values; i++) {
            sumSquaredDifferences += squaredDifferences[i];
        }
        double variance = sumSquaredDifferences / num_values;
        double stdDeviation = sqrt(variance);

        printf("Standard Deviation: %lf\n", stdDeviation);

        exit(0); // Child process exits
    } else { // Parent process
        wait(NULL); // Wait for the child process to finish
        printf("Parent Process: Child process has completed.\n");
    }

    return 0;
}
