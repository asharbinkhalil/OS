#include<iostream>
#include<pthread.h>

using namespace std;
pthread_t tid[5];
int count=0;
pthread_mutex_t mymutex;

void *counter(void *arg)
{
    pthread_mutex_lock ( &mymutex ) ;  //lock mutex
    cout<<"Thread id: "<<pthread_self();
    cout<<" Count: "<<++count<<endl;
    pthread_mutex_unlock ( &mymutex ) ; //unlock mutex
    pthread_exit(NULL);
}

int main()
{
    pthread_mutex_init(&mymutex,NULL);
    
    for(int i=0; i<5; i++)
        pthread_create(&tid[i], NULL, counter, NULL);
    pthread_mutex_destroy(&mymutex) ;

    pthread_exit(NULL);
    return 0;
    
}