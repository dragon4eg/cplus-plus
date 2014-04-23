#pragma once
#include <pthread.h>
#include <list>
//TODO exclude pthread and Thread with std::Thread and std::cond_variable
//add moveConstructor to Segment
//remove pointers everywhere even in Work and ans Items
template <typename T> class wqueue
{
    std::list<T>     queue_;
    pthread_mutex_t  mutex_;
    pthread_cond_t   condv_; 

  public:
    wqueue() 
    {
        pthread_mutex_init(&mutex_, NULL);
        pthread_cond_init(&condv_, NULL);
    }
    ~wqueue() 
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&condv_);
    }
    void add(T item) 
    {
        pthread_mutex_lock(&mutex_);
        queue_.push_back(item);
        pthread_cond_signal(&condv_);
        pthread_mutex_unlock(&mutex_);
    }
    T remove() 
    {
        pthread_mutex_lock(&mutex_);
        while (queue_.size() == 0) 
        {
            pthread_cond_wait(&condv_, &mutex_);
        }
        T item = queue_.front();
        queue_.pop_front();
        pthread_mutex_unlock(&mutex_);
        return item;
    }
    int size() 
    {
        pthread_mutex_lock(&mutex_);
        int size = queue_.size();
        pthread_mutex_unlock(&mutex_);
        return size;
    }
};


