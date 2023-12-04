#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    printf("PID of example.c = %d\n", getpid());
    pid_t p;
    p = fork();
    if(p==-1)
    {
        printf("There is an error while calling fork()");
    }
    if(p==0)
    {
    printf("We are in the child process\n");
    printf("Calling hello.c from child process\n");
   
    //1
    //execl("./hello","ls","-la", NULL);
    
    
    //2
    //execlp("./hello","ls","-la", NULL);
    
    
    //3
    //char *args[] = {"-lh", "/home", NULL};
    //execv("./hello", args);
    
    
    
    //4
    //char *programName = "ls";
    //char *args[] = {programName, "-lh", "/home", NULL};
    //execvp(programName, args);
    
    
    
    //5
    //char *binaryPath = "/bin/bash";
    //char *arg2 = "-c";
    //char *arg1 = "echo";
    //char *const env[] = {"HOSTNAME=www.linuxhint.com", "PORT=8080", NULL};
 
    //execle(binaryPath,arg1, arg2, NULL, env);
    
    
    
    //6
    char *binaryPath = "/bin/bash";
    char *const args[] = {binaryPath, "echo -c Visit $HOSTNAME:$PORT   from your browser.", NULL};
    char *const env[] = {"HOSTNAME=www.linuxhint.com", "PORT=8080", NULL};
 
    execve(binaryPath, args, env);
    }
    else
    {
        printf("We are in the parent process");
    }
    return 0;
}
