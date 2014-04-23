#include "WorkItem.h"
/*******************************WorkItem************************************************/
WorkItem::WorkItem(const std::string message, const std::thread::id number, wqueue<AnswerItem*>* ans_queue) : 
    AnswerItem(message, number), 
    ans_queue_(ans_queue) 
{ }
