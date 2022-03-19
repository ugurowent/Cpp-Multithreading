#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace std;

class App
{
private:
    int count = 0;
    mutex mtx;
public:
    void operator()()
    {
        for (int i = 0; i < 1E6; i++)
        {
            const lock_guard<mutex> guard(mtx);
            //unique_lock<mutex> guard(mtx);
            ++count;
        }
    }

    int getCount()
    {
        return count;
    }
};


void work(int &count, mutex& mtx)
{
    
}

int main()
{
    App app;

    mutex mtx;

    thread t1(ref(app));
    thread t2(ref(app));

    t1.join();
    t2.join();

    cout << app.getCount() << endl;


    return 0;
}
