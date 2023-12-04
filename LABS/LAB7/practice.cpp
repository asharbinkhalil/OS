#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
void *thread1(void *)
{
    for(int i=0; i<5; i++)
    {
        cout<<"*";
    }
    pthread_exit(NULL);
}
pthread_t tid,tid1;


int main()
{
    pthread_attr_t attr,attr1;    // attribute
    pthread_attr_init(&attr);   // initialize
    pthread_attr_init(&attr1);   // initialize
    
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); // set attribute to detached
    pthread_attr_setdetachstate(&attr1,PTHREAD_CREATE_JOINABLE); // set attribute to jinable

    
    pthread_create(&tid,&attr,thread1,NULL); // create thread
    pthread_create(&tid1,&attr1,thread1,NULL); // create thread
    
    if(pthread_join(tid,NULL)==0)
    {
        cout<<"Thread 1 is joinable\n"<<endl;
    }
    else
        cout<<"Thread 1 is Detached\n";


    if(pthread_join(tid1,NULL)==0)
    {
        cout<<"Thread 2 is joinable\n"<<endl;
    }
    else
        cout<<"Thread 2 is Detached";
    return 0;

    
}
