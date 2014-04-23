#pragma once
//#include <pthread.h>
#include <list>
#include <mutex>
#include <condition_variable>
#include <sys/wait.h>
//TODO exclude pthread and Thread with std::Thread and std::cond_variable
//add moveConstructor to Segment
//remove pointers everywhere even in Work and ans Items

using std::unique_lock;
using std::mutex;
using std::list;
using std::condition_variable;


template <typename T> class wqueue
{
private:
    list<T>              queue_;
    mutex                mutex_;
    condition_variable   condv_; 

public:
    wqueue() 
    {
        mutex mutex_;
        condition_variable condv_;
    }
    ~wqueue() { }
    void add(T item)
    {
        mutex_.lock();
        queue_.push_back(item);
        condv_.notify_one();
        mutex_.unlock();
    }
    T remove() 
    {
        //mutex_.lock();
        unique_lock<std::mutex> lck (mutex_);
        //If the object currently owns a lock on the managed mutex object, 
        //its unlock member is called before destroying the object.
        while (queue_.size() == 0) 
        {
            condv_.wait (lck);
        }
        T item = queue_.front();
        queue_.pop_front();
        //mutex_.unlock();
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


