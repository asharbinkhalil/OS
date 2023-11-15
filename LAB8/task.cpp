#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_t tid[5];
int balance = 0;
pthread_mutex_t withdrawl_mutex;
pthread_mutex_t deposit_mutex;



void *withdrawl(void *arg)
{
    pthread_mutex_lock(&withdrawl_mutex); // lock mutex
    int prevbalance=0;
    if (balance > 10)
    {
        prevbalance=balance;
        balance -= 10;
        sleep(1);
        printf("Ammount withdraw Previous Balance %d , New Balance: %d\n", prevbalance,balance);
    }
    else
        printf("Ammount couldn't withdraw, Balance: %d\n", balance);
    pthread_mutex_unlock(&withdrawl_mutex); // unlock mutex
    pthread_exit(NULL);
}

void *deposit(void *arg)
{
    pthread_mutex_lock(&deposit_mutex); // lock mutex
    balance += 11;
    sleep(1);
    printf("Amount Deposited Balance: %d\n", balance);
    pthread_mutex_unlock(&deposit_mutex); // unlock mutex
    pthread_exit(NULL);
}

int main()
{
    pthread_mutex_init(&deposit_mutex, NULL);
    pthread_mutex_init(&withdrawl_mutex, NULL);

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&tid[i], NULL, deposit, NULL);
        for (int j = 0; j < 1; j++)
            pthread_create(&tid[i], NULL, withdrawl, NULL);
    }

    pthread_mutex_destroy(&deposit_mutex);
    pthread_mutex_destroy(&withdrawl_mutex);


    pthread_exit(NULL);
    return 0;
}