#include<stdio.h>
#include<string.h>
int main(int argc, char *argv[]) 
{

    char str[15];
    strcpy (str, argv[1]);
    printf("Length of %s is %ld characters \n\n",str,strlen(str));
    printf("\n\n");
}
