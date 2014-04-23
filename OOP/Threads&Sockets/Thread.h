#pragma once
#include <pthread.h>
class Thread
{
public:
    Thread();
    virtual ~Thread();
    int start();
    int join();
    int detach();
    const pthread_t self() const; 
    virtual void* run() = 0;  
private:
    pthread_t  m_tid;
    int        m_running;
    int        m_detached;
};

