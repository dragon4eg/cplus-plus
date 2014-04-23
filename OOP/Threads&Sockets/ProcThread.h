#pragma once
#include "Segment.h"
#include "PCqueue.h"
#include "WorkItem.h"
#include <thread>
using std::thread;

class ProcThread
{
private:
    PCqueue<WorkItem*>& queue_;
    SegmentSet& db_;
    void run();
public:
    ProcThread(PCqueue< WorkItem * > &, SegmentSet &);
    void operator()(){ run(); }
};
