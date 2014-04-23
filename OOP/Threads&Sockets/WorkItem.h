#pragma once
#include "AnswerItem.h"
#include "PCqueue.h"
#include <thread>
class WorkItem : public AnswerItem
{
public:
    WorkItem(const std::string message, const std::thread::id number, PCqueue< AnswerItem * > & ans_queue): 
        AnswerItem(message, number), 
        ans_queue_(ans_queue) 
    { }
    PCqueue< AnswerItem* > & getAnsQueue() const {return ans_queue_ ;}
private:
    PCqueue< AnswerItem* > & ans_queue_;
};
