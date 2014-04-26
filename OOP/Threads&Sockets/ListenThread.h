#pragma once
#include "WorkItem.h"
#include <unistd.h>    //write, close
#include <iostream>
using std::cout;

class ListenThread
{
public:
    ListenThread(PCqueue< WorkItem > & queue, int socket):
        queue_(queue), 
        socket_(socket), 
        running_(true)
    { }
    ListenThread(ListenThread && other): 
        queue_(other.queue_), 
        socket_(other.socket_),
        running_(other.running_)//this is used to be stoped by USER
    {
        other.socket_ = -1;//a socket alwasy is non negative value
        other.running_ = false;//stop, he's not valid anymore 
    }
    void operator()(){ run(); }
    ~ListenThread()
    {
        if (socket_ != -1 )//if not spoiled by move constructor
        {
            close(socket_);
            cout<<"Connection on socket "<<socket_<<" destroyed!\n";
        }
    }
private:
    void run();
    void killMeSoftly(PCqueue< AnswerItem > &, const std::thread::id);
    PCqueue< WorkItem > & queue_;
    int                   socket_;
    bool                  running_;//default true, false used used for 
                          //self-interruption after being moved by (T&&)
};



