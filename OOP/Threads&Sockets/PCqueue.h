#pragma once
#include <list>
#include <mutex>
#include <condition_variable>
#include <sys/wait.h>
using std::unique_lock;
using std::mutex;
using std::list;
using std::condition_variable;

template <typename T> class PCqueue
{
private:
    list<T>              queue_;
    mutex                mutex_;
    condition_variable   condv_; 

public:
    PCqueue() 
    {
        mutex mutex_;
        condition_variable condv_;
    }
    ~PCqueue() { }
    void add(T item)
    {
        mutex_.lock();
        queue_.push_back(item);
        condv_.notify_one();
        mutex_.unlock();
    }
    T remove() 
    {
        unique_lock<std::mutex> lck (mutex_);
        while (queue_.size() == 0) 
        {
            condv_.wait (lck);
        }
        T item(queue_.front());
        queue_.pop_front();
        return item;
    }
    int size() 
    {
        mutex_.lock();
        int size = queue_.size();
        mutex_.unlock();
        return size;
    }
};

//about wait():
//At the moment of blocking the thread, the function automatically calls lck.unlock(),
//allowing other locked threads to continue.
//Once notified (explicitly, by some other thread), the function unblocks 
//and calls lck.lock(), leaving lck in the same state as when the function was called.

