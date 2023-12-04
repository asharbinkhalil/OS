#include <atomic>
#include <pthread.h>
#include <iostream>

using namespace std;

// num of threads available
const int num = 5;

class Ticket{
    public:
    atomic<int> token[num];
    atomic<bool> choosing[num];

    Ticket()
    {
        for ( int i = 0; i < num; i++)
        {
            token[i] = 0;
            choosing[i] = false;
        }
    }
};

class BakeryAlgo{
    Ticket tickets;
    public:

    int Lock(int i){
        tickets.choosing[i] = true;
        int result = 0;
        for ( int m = 0; m < num; m++)
        {
            int r = tickets.token[m];
            result = r > result ? r : result;
        }
        tickets.token[i] = result + 1;
        tickets.choosing[i] = false;

        for ( int j = 0; j < num; j++){

            while(tickets.choosing[j]);
            while( (tickets.token[j] != 0) && ((tickets.token[j] < tickets.token[i]) || ((tickets.token[j] == tickets.token[i]) && (j < i))) );
            
        }
        
        return tickets.token[i];
    }

    void Unlock(int i){
        cout << "Process " << i+1 << " with token no " << tickets.token[i] << " done with CS" << endl;
        tickets.token[i] = 0;
    }
};

class arguments{
    public:
    int ids;
    BakeryAlgo* algo;
};

void* func(void * args)
{
    arguments* ag = static_cast<arguments*>(args);
    int id = ag->ids;
    BakeryAlgo* process = ag->algo;

    int t = process->Lock(id);

    process->Unlock(id);
    return nullptr;
}

int main()
{
    pthread_t threads[num];
    BakeryAlgo* al = new BakeryAlgo();
    arguments* args = new arguments[num];
    
    for ( int i = 0; i < num; i++)
    {
        args[i].algo = al;
        args[i].ids = i;
        pthread_create(&threads[i],NULL,func,static_cast<void*>(&args[i]));
    }

    for ( int i = 0; i < num; i++)
    {
        pthread_join(threads[i],NULL);
    }

    delete[] args;
    delete al;
    return 0;
}