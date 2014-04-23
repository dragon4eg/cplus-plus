#pragma once
#include "SegmentGeneric.h"
#include <map>
#include <memory>
#include <cstdlib>
namespace
{
    const int segmentLength(const int& start, const int& end)
    {
        return std::abs(end - start);
    }
}

typedef SegmentGeneric<int,  segmentLength> Segment;
typedef std::multimap<int, Segment> SegmentSet;

