#pragma once
#include "Segment.h"
#include <map>
#include <memory>
typedef Segment <int, [] (const int& start, const int& end) { return abs(end - start); } > Segment;
typedef std::multimap<int, Segment> SegmentSet;
