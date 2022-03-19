#include <iostream>
#include <thread>

using namespace std;

template<typename T>
class blocking_queue
{
public:
    void push(T t)
    {
        cout << "push" << endl;
    }

    void pop()
    {
        cout << "pop" << endl;
    }
};

int main()
{
    blocking_queue<int> qu;

    thread t1(&blocking_queue<int>::push, &qu, 7);
    thread t2(&blocking_queue<int>::pop, &qu);

    t1.join();
    t2.join();

    return 0;
}