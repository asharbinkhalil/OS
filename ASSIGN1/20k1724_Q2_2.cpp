#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<stdio.h>
#include<sys/wait.h>
#include<vector>
using namespace std;
int main() {
  string name;
  vector<pid_t> child_pids;
  int shmid = shmget(key_t(1234), 4096, IPC_CREAT | 0666);
  bool flag = true;
  while(flag)
  {
    system("clear");
    while(flag)
    {
      cout<<"\nchoose option\n[1] Create client\n[2] End\n";
      int str;
      cin>>str;
      if(str == 1)
        {
          cout<<"Enter the name for the client : ";
          cin>>name;
          name.resize(20, ' ');
          break;
        }
      else if(str == 2)
      {
        flag =false;
      }
    }
    if(flag )
    {
      for(auto i : child_pids)
        waitpid(i,NULL,WNOHANG);
      pid_t pid= fork();
      if(pid>=1)
      {
        child_pids.push_back(pid);
      }
      if(pid== 0)
      {
          char * keyC = const_cast<char*> (to_string(int(shmid)).c_str());
          execl("/usr/bin/gnome-terminal", "gnome-terminal", "--", "./a", keyC, const_cast<char*> (name.c_str()), NULL);
      }
    }
  }
  shmctl(shmid, IPC_RMID, NULL);
  while(wait(NULL) >= 1);
  return 1;
}

