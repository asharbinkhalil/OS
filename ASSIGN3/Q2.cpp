#include <atomic>
#include <pthread.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <unistd.h>
using namespace std;

//Total num of threads
const int num = 20;

//counters
int waiting_cars_l = 0;
int waiting_busses_l = 0;
int waiting_cars_r = 0;
int waiting_busses_r = 0;
int busses_on_bridge_r = 0;
int cars_on_bridge_r = 0;
int busses_on_bridge_l = 0;
int cars_on_bridge_l = 0;
int curr_dir = 0;
int counter = 0;

//Mutex to control exclusion
pthread_mutex_t mutex;

//var to cancel threads at particular time
bool endtraffic = false;

//Vehicle class to store all the vehicles
class Vehicle
{
    public:
    int id;
    int type;
    int direction;
};

//Func to handle arrival to bridge
void ArriveAtBridge(int direction, int type)
{

    pthread_mutex_lock(&mutex);
    if ( direction == curr_dir )
    {
        
        cout << (type == 0 ? "Car": "Bus") << " arrived to cross the bridge" << endl;
        if ( type == 0 )
        {
            if ( ((cars_on_bridge_l < 4 && busses_on_bridge_l == 0) || (cars_on_bridge_l < 2 && busses_on_bridge_l == 1) ) ) {
               
                if ( curr_dir == 0 ){
                     cars_on_bridge_l++;
                }
                
            }
            if ((cars_on_bridge_r < 4 && busses_on_bridge_r == 0) || (cars_on_bridge_r < 2 && busses_on_bridge_r == 1) ) 
            {
                if ( curr_dir == 1 )
                {
                    cars_on_bridge_r++;
                }
        
            }
        }

        else
        {
            if ( (cars_on_bridge_l < 3 && busses_on_bridge_l == 0 ) ){
                if ( curr_dir == 0 )
                {
                    busses_on_bridge_l++;
                }
                   
            }

             if (cars_on_bridge_r < 3 && busses_on_bridge_r == 0 )
            {
                if ( curr_dir == 1 )
                {
                    busses_on_bridge_r++;
                }
            }

        }

        

    }

    else
    {
        cout << (type == 0 ? "Car": "Bus") << " is waiting on other side of the bridge" << endl;
        if ( type == 0 ){
             if ( curr_dir == 0 ){
                    waiting_cars_r++;
                    if ( (cars_on_bridge_l < 4 && busses_on_bridge_l == 0) || (cars_on_bridge_l < 2 && busses_on_bridge_l == 1) )
                 {
                    if ( waiting_cars_l > 0 )
                    {cars_on_bridge_l++;
                    waiting_cars_l--;}
                 }
                  if ( (cars_on_bridge_l < 3 && busses_on_bridge_l == 0) )
                 {
                    if ( waiting_busses_l > 0 )
                    {busses_on_bridge_l++;
                    waiting_busses_l--;}
                 }
                }
                else{
                     waiting_cars_l++;
                     if ( (cars_on_bridge_r < 4 && busses_on_bridge_r == 0) || (cars_on_bridge_r < 2 && busses_on_bridge_r == 1) )
                 {
                    if ( waiting_cars_r > 0 )
                    {cars_on_bridge_r++;
                    waiting_cars_r--;}
                 }
                  if ( (cars_on_bridge_r < 3 && busses_on_bridge_r == 0) )
                 {
                    if ( waiting_busses_r > 0 )
                    {busses_on_bridge_r++;
                    waiting_busses_r--;}
                 }
                }
        }
        else{
            if ( curr_dir == 0 ){
                    waiting_busses_r++;
                    if ( (cars_on_bridge_l < 3 && busses_on_bridge_l == 0) )
                 {
                    if ( waiting_busses_l> 0 )
                    {busses_on_bridge_l++;
                    waiting_busses_l--;}
                 }
                 if ( (cars_on_bridge_l < 4 && busses_on_bridge_l == 0) || (cars_on_bridge_l < 2 && busses_on_bridge_l == 1) )
                 {
                    if ( waiting_cars_l > 0 )
                    {cars_on_bridge_l++;
                    waiting_cars_l--;}
                 }
                }
                else{
                     waiting_busses_l++;
                      if ( (cars_on_bridge_r < 3 && busses_on_bridge_r == 0) )
                 {
                    if ( waiting_busses_r> 0 )
                    {busses_on_bridge_r++;
                    waiting_busses_r--;}
                 }
                 if ( (cars_on_bridge_r < 4 && busses_on_bridge_r == 0) || (cars_on_bridge_r < 2 && busses_on_bridge_r == 1) )
                 {
                    if ( waiting_cars_r > 0 )
                    {cars_on_bridge_r++;
                    waiting_cars_r--;}
                 }
                }
        }
        
    }
    if (!curr_dir == 0){
            cars_on_bridge_l == 0;
            busses_on_bridge_l = 0;
        }
        else if (!curr_dir == 1){
            cars_on_bridge_r == 0;
            busses_on_bridge_r = 0;
        }
    cout << "*****************************" << endl;
                cout << "Direction : " << curr_dir << endl;
                cout << "Total cars on bridge in 0 direction : " << cars_on_bridge_l << endl << " Total busses on bridge in 0 direction : " << busses_on_bridge_l << endl;
                cout << " Total cars waiting on 0 side: " << waiting_cars_l << endl << " Total busses waiting on 0 side: " << waiting_busses_l << endl; 
                cout << "Total cars on bridge in 1 direction : " << cars_on_bridge_r << endl << " Total busses on bridge in 1 direction : " << busses_on_bridge_r << endl;
                cout << " Total cars waiting on 1 side: " << waiting_cars_r << endl << " Total busses waiting on 1 side : " << waiting_busses_r << endl; 
                cout << "*****************************" << endl;
   
   counter++;
    pthread_mutex_unlock(&mutex);
}

//Func to handle departure 
void ExitBridge(int type)
{
    pthread_mutex_lock(&mutex);
 
    //Exiting bridge and controlling all the constraints
    if ( type == 0 ){
        if ( (cars_on_bridge_l > 0 && curr_dir == 0 ) ){
                 cars_on_bridge_l--;
                 if ( (cars_on_bridge_l < 4 && busses_on_bridge_l == 0) || (cars_on_bridge_l < 2 && busses_on_bridge_l == 1) )
                 {
                    if ( waiting_cars_l > 0 )
                    {cars_on_bridge_l++;
                    waiting_cars_l--;}
                 }
                 if ( (cars_on_bridge_l < 3 && busses_on_bridge_l == 0) )
                 {
                    if ( waiting_busses_l > 0 )
                    {busses_on_bridge_l++;
                    waiting_busses_l--;}
                 }

        }
        else if (cars_on_bridge_r > 0 && curr_dir == 1 ) {
                cars_on_bridge_r--;
                if ( (cars_on_bridge_r < 4 && busses_on_bridge_r == 0) || (cars_on_bridge_r < 2 && busses_on_bridge_r == 1) )
                 {
                    if ( waiting_cars_r > 0 )
                    {cars_on_bridge_r++;
                    waiting_cars_r--;}
                 }
                  if ( (cars_on_bridge_r < 3 && busses_on_bridge_r == 0) )
                 {
                    if ( waiting_busses_r > 0 )
                    {busses_on_bridge_r++;
                    waiting_busses_r--;}
                 }
        }
    }

    else{
        if (busses_on_bridge_l > 0 && curr_dir == 0 ) {
                busses_on_bridge_l--;
                if ( (cars_on_bridge_l < 3 && busses_on_bridge_l == 0) )
                 {
                    if ( waiting_busses_l > 0 )
                    {busses_on_bridge_l++;
                    waiting_busses_l--;}
                 }
                 if ( (cars_on_bridge_l < 4 && busses_on_bridge_l == 0) || (cars_on_bridge_l < 2 && busses_on_bridge_l == 1) )
                 {
                    if ( waiting_cars_l > 0 )
                    {cars_on_bridge_l++;
                    waiting_cars_l--;}
                 }
        } 
        else if (busses_on_bridge_r > 0 && curr_dir == 1 ) {
            busses_on_bridge_r--;
            if ( (cars_on_bridge_r < 3 && busses_on_bridge_r == 0) )
                 {
                    if ( waiting_busses_r > 0 )
                    {busses_on_bridge_r++;
                    waiting_busses_r--;}
                 }
                 if ( (cars_on_bridge_r < 4 && busses_on_bridge_r == 0) || (cars_on_bridge_r < 2 && busses_on_bridge_r == 1) )
                 {
                    if ( waiting_cars_r > 0 )
                    {cars_on_bridge_r++;
                    waiting_cars_r--;}
                 }
        }
    }

    cout << "*****************************" << endl;
                cout << "Direction : " << curr_dir << endl;
                cout << "Total cars on bridge in 0 direction : " << cars_on_bridge_l << endl << " Total busses on bridge in 0 direction : " << busses_on_bridge_l << endl;
                cout << " Total cars waiting on 0 side: " << waiting_cars_l << endl << " Total busses waiting on 0 side: " << waiting_busses_l << endl; 
                cout << "Total cars on bridge in 1 direction : " << cars_on_bridge_r << endl << " Total busses on bridge in 1 direction : " << busses_on_bridge_r << endl;
                cout << " Total cars waiting on 1 side: " << waiting_cars_r << endl << " Total busses waiting on 1 side : " << waiting_busses_r << endl; 
                cout << "*****************************" << endl;
   
    if ( (cars_on_bridge_l == 0 && busses_on_bridge_l == 0 && curr_dir == 0 && waiting_cars_l < 1 ) || (cars_on_bridge_r == 0 && busses_on_bridge_r == 0 && waiting_cars_r < 1 && curr_dir == 1 
|| counter == 10)){
        
            cars_on_bridge_l = 0;
            busses_on_bridge_l = 0;
            cars_on_bridge_r = 0;
            busses_on_bridge_r = 0;
        counter = 0;
        curr_dir = !curr_dir;
        cout << "Direction of the bridge changed from " << !curr_dir << " to " << curr_dir << endl;
    }

    pthread_mutex_unlock(&mutex);
}

//Main func to control traffic flow
void* trafficflow(void* args)
{
    Vehicle* V = static_cast<Vehicle*>(args);
    int d = V->direction;
    int t = V->type;
  // cout << "Vehicle no " << V->id+1 << (t == 0 ? " Car ": " Bus ") << "entered the traffic flow" << endl;
  while (true && !endtraffic)
  { ArriveAtBridge(d,t);
    ExitBridge(t);
}
    pthread_exit(NULL);
}


int main()
{
    //Initializing mutex
    pthread_mutex_init(&mutex, NULL);

    //creating threads
    pthread_t threads[num];
    Vehicle v[num];
    
    //Randomly assigning vehicle type and direction
    for ( int i = 0; i < num; i++)
    {
        v[i].id = i;
        v[i].direction = rand() % 2; 
        v[i].type = rand() % 2;
    }

    for ( int i = 0; i < num; i++)
    {
        pthread_create(&threads[i],NULL,trafficflow,static_cast<void*>(&v[i]));
    }


    sleep(5);
   endtraffic = true;
    for ( int i = 0; i < num; i++)
    {
        pthread_join(threads[i],NULL);
    }

    return 0;
}