#pragma once
#include <thread>
#include <string>
using std::string;
using std::thread;

class AnswerItem
{
public:
    AnswerItem(): message_(""), id_(thread::id()) { }// get rid of this one
    AnswerItem(const string & message, const thread::id id) : 
        message_(message), 
        id_(id) 
    { }
    AnswerItem (const AnswerItem & other):
        message_(other.message_), 
        id_(other.id_) 
    { }
    // move constructor
    //AnswerItem (AnswerItem && other):
    //    message_(other.message_), 
    //    id_(other.id_) 
    //{
    //    message_ = "";
    //    thread noexec; //make a default constructed thread to get non numeric id
    //    id_ = noexec.get_id();
    //}
    const string & getMessage() const { return message_; }
    const thread::id getId() const { return id_; }
    void putMessage(const string & msg) { message_ = msg; }
    void putId(const thread::id num) { id_ = num; }
private:
    string        message_;
    thread::id    id_;
};

