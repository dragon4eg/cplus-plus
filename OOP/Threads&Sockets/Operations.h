#pragma once
#include "Segment.h"
#include "PCqueue.h"
#include "AnswerItem.h"

namespace Operations
{
    //auxiliary
    const bool isDbEmpty(const SegmentSet &, AnswerItem &, PCqueue< AnswerItem > &);
    SegmentSet::const_iterator findSameSegment(const SegmentSet &, Segment &);
    const bool parseAndInit(const string &, Segment &, AnswerItem & item, const std::thread::id);
    void solve(const Segment &, const SegmentSet &, SegmentSet::iterator, AnswerItem &);
    //main
    void make(const string &, Segment &, SegmentSet &, PCqueue< AnswerItem > &, const std::thread::id);
    void remove(const string &, Segment &, SegmentSet &, PCqueue< AnswerItem > &, const std::thread::id);
    void find(const string &, Segment &, SegmentSet &, PCqueue< AnswerItem > &, const std::thread::id);
}

