#pragma once
#include <sstream>
#include <string>
#include <iostream>


template< typename TPoint, const double (*DistanceFunction)(const TPoint&, const TPoint&) >
class SegmentGeneric
{
public:
    SegmentGeneric():
        start_(TPoint{}),
        end_(TPoint{})
    { }
    SegmentGeneric(const TPoint & start, const TPoint & end):
        start_(start),
        end_(end)
    { }
    SegmentGeneric(const SegmentGeneric & other):
        start_(other.start()),
        end_(other.end())
    { }

    const TPoint & start() const { return start_; }
    const TPoint & end() const { return end_; }
    TPoint & start() { return start_; }
    TPoint & end() { return end_; }
    
    const double length() { return DistanceFunction(start(), end()); }

    explicit operator std::string() const
    {
    	std::stringstream ss;
        ss << '[' << start() << ';' << end() << ']';
        return ss.str();
    }

private:
    TPoint start_, end_;
};

template< typename TPoint, const double (*DistanceFunction)(const TPoint&, const TPoint&) >
std::ostream & operator<<(std::ostream & os, const SegmentGeneric< TPoint, DistanceFunction > & s)
{
    os << static_cast< std::string >(s);
    return os;
}


