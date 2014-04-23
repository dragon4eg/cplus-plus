#pragma once
#include "SegmentGeneric.h"
#include <map>
#include <memory>
#include <cstdlib>
namespace SegmentLength
{
    const double segmentLength(const int & start, const int & end);
}

typedef SegmentGeneric< int, &SegmentLength::segmentLength > Segment;
typedef std::multimap< int, Segment > SegmentSet;

