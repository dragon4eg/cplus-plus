#pragma once
#include "WorkItem.h"
#include "AnswerItem.h"
#include "ListenManager.h" //Pool
#include <unistd.h>    //write, close
#include <iostream>
#include <thread>
#include <set>
using std::set;
using std::thread;
using std::cout;
using std::move;
using std::pair;

class ListenThread
{
public:
    ListenThread(PCqueue< WorkItem > & queue, int socket, Pool pool):
        queue_(queue), 
        socket_(socket), 
        pool_(pool),
        running_(true)
    { }
    // move constructor
    ListenThread(ListenThread && other): 
        queue_(other.queue_), 
        socket_(other.socket_),
        pool_(other.pool_),
        running_(other.running_)
    {
        other.socket_ = -1;//a socket alwasy is non negative value, so spoil it
        other.running_ = false;//stop, he's not valid anymore and deserve to die
    }
    //move assignment operator
    //ListenThread & operator=(ListenThread && other)
    //{
    //    //queue_ (other.queue_); 
    //    socket_ = other.socket_;
    //    position_ = other.position_;
    //    running_ = other.running_;
    //    other.socket_ = -1;
    //    other.running_ = false;
    //    other.position_ = (set< pair< thread, bool > >::iterator)nullptr;
    //}
    
    //operator () to be run by a thread
    void operator()(){ run(); }
    ~ListenThread()
    {
        if (socket_ != -1 )//if not spoiled by move constructor
        {
            close(socket_);
            cout<<"Socket: "<<socket_<<" closed!\n"; 
        }
    }
    //Binding a reference to an object has no effect on the object lifetime 
    //(other than possibly extending it if the reference is const and the object 
    //is a temporary, but it surely won't reduce the lifetime)
    //so the above destructor will not affect the PCqueue<WorkItem> itself
private:
    void run();
    void killMeSoftly(PCqueue< AnswerItem > &, const std::thread::id);
    PCqueue< WorkItem > &     queue_;
    int                       socket_;
    Pool &                    pool_;//we see with this pointer a false STOPer in the pool, true - used for interruption
    bool                      running_;//default true, false used used for self-interruption after being moved by = or (T&&)
};



