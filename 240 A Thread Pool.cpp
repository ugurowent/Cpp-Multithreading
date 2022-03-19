#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <mutex>
#include <queue>
#include <cmath>

using namespace std;

mutex g_mtx;

template<typename T>
class blocking_queue
{
private:
    mutex _mtx;
    condition_variable _cond;
    int _max_size;
    queue<T> _queue;

public:
    
    blocking_queue(int max_size) : _max_size(max_size) { }

    void push(T t)
    {
        unique_lock<mutex> lock(_mtx);
        _cond.wait(lock, [this](){
            return _queue.size() < _max_size;
        });
        _queue.push(t);

        lock.unlock();
        _cond.notify_one();
    }

    T front()
    {
        unique_lock<mutex> lock(_mtx);
        _cond.wait(lock, [this](){
            return !_queue.empty();
        });

        return _queue.front();
    }

    void pop()
    {
        unique_lock<mutex> lock(_mtx);
        _cond.wait(lock, [this](){
            return !_queue.empty();
        });

        _queue.pop();

        lock.unlock();
        _cond.notify_one();

    }

    int size()
    {
        lock_guard<mutex> lock(_mtx);
        return _queue.size();
    }
};

int work(int id)
{
    unique_lock<mutex> lock(g_mtx);
    cout << "Starting " << id << endl;
    lock.unlock();

    int seconds = int((5.0 * rand()) / RAND_MAX + 3);
    this_thread::sleep_for(chrono::seconds(seconds));

    return id;
}

int main()
{
    blocking_queue<shared_future<int>> futures(2);

    thread t([&](){
        for (int i = 0; i < 20; i++)
        {
            shared_future<int> f = async(launch::async, work, i);
            futures.push(f);
        }
    });
    

    for (int i = 0; i < 20; i++)
    {
        shared_future<int> f = futures.front();
        int value = f.get();
        futures.pop();
        cout << "Returned: " << value << endl;
    }

    t.join();

    return 0;
}