#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include<string.h>
#include<sys/wait.h>
int main()
{

    char str[30];
    printf("Enter str: ");
    fgets(str, sizeof(str), stdin);
    str[strlen(str)-1] = '\0';
    pid_t a = fork();
    if (a == 0)
    {
        printf("Child for Capitalize: ");
        char *args[] = {"binaries/capitalize", str, NULL};
        execvp(args[0], args);
    }
    else if (a > 0)
    {
        pid_t b = fork();
        if (b == 0)
        {
            printf("Child for Reverse: ");
            char *args[] = {"binaries/reverse", str, NULL};
            execvp(args[0], args);
        }

        else if (b > 0)
        {
            pid_t c = fork();
            if (c == 0)
            {
                printf("Child for Length: ");
                char *args[] = {"binaries/length", str, NULL};
                execvp(args[0], args);
            }
            else if(c>0)
            {
                wait(NULL);
                wait(NULL);
                wait(NULL);      
            }
        }
    }

    return 0;
}
