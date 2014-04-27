#include "Segment.h"
const double SegmentLength::segmentLength(const int & start, const int & end)
{
    return std::abs(end - start);
}

