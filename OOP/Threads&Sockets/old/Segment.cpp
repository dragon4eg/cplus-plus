#include "Segment.h"
#include <algorithm>
/**************************************Segment******************************************/
Segment::Segment(const Point a, const Point b /* = Point() */):
    start_(a), 
    end_(b) 
{ }
Segment::Segment(const Segment & s):
    start_(s.start()), 
    end_(s.end()) 
{ }
const int Segment::length()
{
    return abs(end().x() - start().x());
} 
std::ostream & operator<<(std::ostream & os, const Segment & s)
{
    os<<s.start()<<'-'<<s.end();
    return os;
}
const std::string to_string (Segment s)
{
    std::string result;
    return result = to_string(s.start()) + '-' + to_string(s.end());
}

  
