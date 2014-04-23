#pragma once
#include <iostream>
#include "Point.h"

class Segment
{
public:
	Segment(): 
		start_ (Point()), 
		end_(Point()) 
	{}
	Segment(const Point a, const Point b = Point());
	Segment(const Segment & s);
	const Point & start() const { return start_; }
	const Point & end() const { return end_; }
	Point & start() { return start_; }
	Point & end() { return end_; }
	const int length();
private:
	Point start_, end_;
};
std::ostream & operator<<(std::ostream & os, const Segment & s);
const std::string to_string (Segment s);
