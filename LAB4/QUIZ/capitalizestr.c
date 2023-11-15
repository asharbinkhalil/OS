#include<stdio.h>
#include<string.h>
int main(int argc, char *argv[]) 
{

    char str[15];
    strcpy (str, argv[1]);
    printf("String in Lower Case: ");
    puts(str); 
    int i;
    for (i = 0; str[i]!='\0'; i++) {
        if(str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] -32;
        }
    }
    printf("String in Upper Case ");
    puts(str); 
    printf("\n\n");

}
