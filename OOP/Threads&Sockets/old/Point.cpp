#include "Point.h"
Point & Point::operator= (Point & p)
{
    x_ = p.x();
    return *this;
}
const bool operator==(const Point & a, const Point & b)
{
    return a.x()==b.x();
}
std::ostream & operator<<(std::ostream & os, const Point & p)
{
    os<<'('<<p.x()<<')';
    return os;
}
const std::string to_string (Point p)
{
    std::string result;
    return result = '(' + to_string(p.x()) + ')';
}
