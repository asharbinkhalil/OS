#include <stdio.h>
#include <unistd.h>
int main()
{
    char buffer[10];
    // Read from standard input ( by default it is keyboard)
    read(0, buffer, 10);
    // Write to standard output ( by default it is monitor )
    write(1, buffer, 10);
    // By changing the file descriptors we can write to files
    return 0;
}
