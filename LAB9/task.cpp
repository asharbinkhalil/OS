// Question # 01
// Write a program to implement this scenario:
// There is a water filter which is adding 10L water in one iteration and five persons that
// are getting water from filter each person getting 5L in one iteration.
// Implement this using Semaphores.
// First solve with binary semaphore then you can consider that three persons can take water at
// the same time.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;
sem_t sem;
int WATER=0;
void *water_filter(void *arg)
{

    int i = 1, value;
    sem_wait(&sem);
    sem_getvalue(&sem, &value);
    WATER+=10;
    WATER-=5;
    printf(" Thread with ID %ld return from wait () and Sem state is :%d and added water and released water, available water : %d\n", pthread_self(), value, WATER);
    pthread_exit(0);
}

int mainforthree()
{
    pthread_t tid[5];
    int value,i;
    sem_init(&sem, 0, 3);
    for (i = 0; i < 5; i++)
    {
        pthread_create(&tid[i], NULL, water_filter, NULL);
    }
    sleep(3);
    printf("Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem,&value);
    printf("Main : After Post sem state :%d \n", value);
    sleep(3);
    printf("Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem, &value);
    printf("Main : After Post sem state :%d \n", value);
    pthread_exit(0);
}

int mainfortwo()
{
    pthread_t tid[3];
    int value,i;
    sem_init(&sem, 0, 1);
    for (i = 0; i <1; i++)
    {
        pthread_create(&tid[i], NULL, water_filter, NULL);
    }
    sleep(3);
    printf("Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem,&value);
    printf("Main : After Post sem state :%d \n", value);
    sleep(3);
    printf("Main : Going to Post\n");
    sem_post(&sem);
    sem_getvalue(&sem, &value);
    printf("Main : After Post sem state :%d \n", value);
    pthread_exit(0);
}


int main()
{
    int choice;
    printf("Press 1 for binary, 2 for more than two\n");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        mainfortwo();    
        break;
    default:
        mainforthree();
        break;
    }

}