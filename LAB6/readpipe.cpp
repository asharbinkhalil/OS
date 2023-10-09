#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
using namespace std;

int main()
{
    int fd = open("pipe1", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }


    mkfifo("pipe2", 0666);
    int fd1 = open("pipe2", O_WRONLY);
    if (fd1 == -1)
    {
        perror("open");
        return 1;
    }

    while (1)
    {
        char buff[80];
        ssize_t bytesRead = read(fd, buff, sizeof(buff));
        if (bytesRead == -1)
        {
            perror("read");
            break;
        }
        else if (bytesRead == 0 || !strcmp(buff,"exit"))
        {
            cout << "Writer closed the pipe. Exiting." << endl;
            break;
        }
        else
        {
            buff[bytesRead] = '\0';
            cout << "Message received: " << buff << endl;
        }


        //write
        char buff1[80];
        cout << "Enter message: ";
        cin.getline(buff1, sizeof(buff1)); // Read the entire line
        write(fd1, buff1, strlen(buff1) + 1);
    }

    close(fd);

    close(fd1);



    return 0;
}
