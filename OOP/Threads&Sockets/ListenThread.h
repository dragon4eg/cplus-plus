#pragma once
#include "WorkItem.h"
#include <unistd.h>    //write, close
#include <thread>
#include <iostream>
using std::cout;
using std::thread;
using std::move;

class ListenThread
{
public:
    ListenThread(PCqueue<WorkItem*> &, int );
    ListenThread(ListenThread && other): 
        queue_(other.queue_), 
        socket_(other.socket_),
        workable_(true)
    {
        other.socket_ = -1;//a socket alwasy is non negative value
        other.workable_ = false;//make other lisnener stop working
    }
    void operator()(){ run(); }
    ~ListenThread() 
    { 
        if (socket_ != -1 ) 
        {
            close(socket_);
            cout<<"Socket: "<<socket_<<" closed!\n"; 
        }
    }
    //Binding a reference to an object has no effect on the object lifetime 
    //(other than possibly extending it if the reference is const and the object 
    //is a temporary, but it surely won't reduce the lifetime)
    //so the above destructor will not affect the PCqueue<WorkItem*> itself
private:
    void killMeSoftly(PCqueue<AnswerItem*> &);
    PCqueue<WorkItem*> & queue_;
    int     socket_;
    bool    workable_;
    void    run();
};



