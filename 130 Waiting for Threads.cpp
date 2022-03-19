#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

int main()
{
    atomic<bool> ready = false;

    thread t1([&](){
        this_thread::sleep_for(chrono::milliseconds(2000));
        ready = true;
    });

    t1.join();

    while(!ready)
    {
        this_thread::sleep_for(chrono::milliseconds(2000));
    }

    cout << "ready " << ready << endl;

    return 0;
}