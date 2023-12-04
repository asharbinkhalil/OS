// Write a C++ program in which create an array of 100 fill it from 1-100. Take a number from user
// and search it in an array using four threads. You have to divide searching portion between threads.
// If any thread found a number, print found message and cancel all other threads.

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>
using namespace std;
// thread to check number
int FLAG = 0;
int datafff=0;
int *array = new int[100];
struct data
{
    int start=0, end=0, find=0;
};
void *checknumber(void *arg)
{
    struct data *data = (struct data *)arg;
    for (int i = data->start; i < data->end; i++)
    {
        if (array[i] == datafff)
        {
            cout << "Number is found by thread " << pthread_self() << endl;
            FLAG = 1;
            pthread_cancel(pthread_self());
        }
    }
    pthread_exit(NULL);
}
pthread_t tid, tid1, tid2, tid3;
int main()
{
    pthread_attr_t attr;                                         // attribute
    pthread_attr_init(&attr);                                    // initialize
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // set attribute to joinable

    for (int i = 0; i < 100; i++)
    {
        if (i < 5)
            cin >> array[i];
        else
            array[i] = rand() % 10 + 17;
    }

    // for (int i = 0; i < 100; i++)
    //     cout << array[i] << " ";
    int flag = 0;
    struct data d1;
    d1.end = 25;
    d1.start = 0;
    cout << "\nEnter number to find: ";
    cin >> datafff;
    pthread_create(&tid, &attr, checknumber, &d1);
    if (flag==1)
    {    d1.start = 25;
        d1.end = 50;
        pthread_create(&tid1, &attr, checknumber, &d1);
        d1.start = 50;
        d1.end = 75;
        pthread_create(&tid2, &attr, checknumber, &d1);
        d1.start = 75;
        d1.end = 100;
        pthread_create(&tid3, &attr, checknumber, &d1);
    }



    return 0;
}
