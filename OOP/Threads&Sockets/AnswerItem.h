#pragma once
#include <string>
#include <thread>
using std::string;
using std::thread;

class AnswerItem
{
public:
    AnswerItem(): message_(""), id_(0) { }
    AnswerItem(const string message, const thread::id number) : 
            message_(message), 
            id_(number) 
        { }
    virtual ~AnswerItem() { }
    const string getMessage() const{ return message_; }
    const thread::id getId() const { return id_; }
    void putMessage(const string msg) { message_ = msg; }
    void putId(const thread::id num) { id_ = num; }
private:
    string        message_;
    thread::id    id_;
};
