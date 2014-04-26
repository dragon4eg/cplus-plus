#pragma once
#include "Segment.h"
#include "PCqueue.h"
#include "WorkItem.h"
#include "AnswerItem.h"
#include <thread>
using std::thread;

class ProcThread
{
private:
    PCqueue< WorkItem > &  queue_;
    SegmentSet&            db_;
    bool                   stop_;
    void run();
    const bool mayIDie(const string &, PCqueue< AnswerItem > &, const std::thread::id);
public:
    ProcThread(PCqueue< WorkItem > &, SegmentSet & );
    void operator()(){ run(); }
};
