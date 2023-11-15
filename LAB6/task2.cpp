#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;
class rectangle
{
public:
    int length;
    int width;
    int area;
    int perimeter;
    void calculate_area()
    {
        area = length * width;
    }
    void calculate_perimeter()
    {
        perimeter = 2 * (length + width);
    }
};

void *thread_function(void *arg)
{
    rectangle *r = (rectangle *)arg;
    r->calculate_area();
    r->calculate_perimeter();
    cout << "Area of rectangle is: " << r->area << endl;
    cout << "Perimeter of rectangle is: " << r->perimeter << endl;
}

int main()
{
    rectangle r;
    r.length = 5;
    r.width = 10;
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, (void *)&r);
    pthread_join(thread, NULL);
    return 0;
}
