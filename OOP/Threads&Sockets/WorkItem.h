#pragma once
#include "AnswerItem.h"
#include "PCqueue.h"
#include <thread>
using std::string;
using std::thread;
class WorkItem
{
public:
    WorkItem(const std::string message, const std::thread::id id, PCqueue< AnswerItem > & ans_queue) : 
        message_(message), 
        id_(id), 
        ans_queue_(ans_queue) 
    { }
    // move constructor
    WorkItem (WorkItem && other):
        message_(other.message_), 
        id_(other.id_), 
        ans_queue_(other.ans_queue_) 
    {
        message_ = "";
        thread noexec; //make a default constructed thread to get non numeric id
        id_ = noexec.get_id();
    }
    const string getMessage() const { return message_; }
    const thread::id getId() const { return id_; }
    void putMessage(const string msg) { message_ = msg; }
    void putId(const thread::id num) { id_ = num; }
    PCqueue< AnswerItem > & getAnsQueue() const { return ans_queue_; }
private:
    string                   message_;
    thread::id               id_;
    PCqueue< AnswerItem > & ans_queue_;
};
