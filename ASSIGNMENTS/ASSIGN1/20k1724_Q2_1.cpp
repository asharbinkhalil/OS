#include <iostream>
#include <string>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
using namespace std;
void write_shared_memory(int shmid, const std::string& data) {
    char* memptr = (char*)shmat(shmid, NULL, 0);
    if(memptr==(char*)1)
        exit(0);
    std::strcpy(memptr, data.c_str());
    shmdt(memptr);
}

std::string read_shared_memory(int shmid) {
    char* memptr = (char*)shmat(shmid, NULL, 0);
    if(memptr==(char*)1)
        exit(0);
    std::string data = memptr;
    shmdt(memptr);
    return data;
}
int main(int argc, char * args[])
{
    string buff;
    while(1)
    {
        cout<< "\n\n\nSelect option\n[1] Write\n[2] Read\n[3] exit\n";
        int option = 0;
        cin >> option;
        if(option == 1)
        {
            cin.ignore();
            cout<<"Enter your text : ";
            getline(cin,buff);
            buff = read_shared_memory(atoi(args[1])) +"\n" +string(args[2]) + "  :   "+buff;
            write_shared_memory(atoi(args[1]),buff);
            system("clear");
        }
        else if(option ==2)
        {
            system("clear");
            cout << read_shared_memory(atoi(args[1]));
        }
        else 
            break;
    }
    exit(0);
}
