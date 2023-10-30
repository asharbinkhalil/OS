#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;


void *mythread(void *)
{
cout<<"My first thread."<<endl;
}
int main()
{
//cout<<"PROCESS ID: "<<getpid()<<endl;
//cout<<"THREAD ID: "<<pthread_self()<<endl;
pthread_t tid;
pthread_create(&tid, NULL, mythread, NULL);
cout<<"First thread created."<<endl;
pthread_exit(NULL);
}
