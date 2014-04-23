#pragma once
#include <iostream>
class Point
{
public:
	Point(const int x = 0): x_(x) { }
	Point(const Point & p): x_(p.x()) { }
	const int & x() const { return x_;}
	int & x() { return x_;}
	Point & operator= (Point & p);
private:
	int x_;
};

const bool operator==(const Point & a, const Point & b);
std::ostream & operator<<(std::ostream & os, const Point & p);
const std::string to_string (Point p);

