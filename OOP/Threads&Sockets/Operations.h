#pragma once
#include "Segment.h"
#include "PCqueue.h"
#include "AnswerItem.h"

namespace Operations
{
    //auxiliary
    void parseAndInit(const string & message, Segment & s);
    void solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter, AnswerItem &);
    //main
    void make(const string &, Segment &, SegmentSet &, PCqueue< AnswerItem > &, const std::thread::id);
    void remove(const string &, Segment &, SegmentSet &, PCqueue< AnswerItem > &, const std::thread::id);
    void find(const string &, Segment &, SegmentSet &, PCqueue< AnswerItem > &, const std::thread::id);
}

