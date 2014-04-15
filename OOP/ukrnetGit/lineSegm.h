#include <memory>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <algorithm>
 
using namespace std;
class Segment;
typedef multimap<int, Segment> SegmentSet; 
class Point
{
public:
	Point(const int x = 0): m_x(x) { }
	Point(const Point & p): m_x(p.x()) { }
	~Point() { }
	const int & x() const { return m_x;}
	int & x() { return m_x;}
	Point & operator= (Point & p);
private:
	int m_x;
};

const bool operator==(const Point & a, const Point & b);
ostream & operator<<(ostream & os, const Point & p);
/********************************************************************************************/
class Segment
{
public:
	Segment(const Point a = Point(), const Point b = Point());
	Segment(const Segment & s);
	~Segment() { }
	const Point & start() const { return m_start; }
	const Point & end() const { return m_end; }
	Point & start() { return m_start; }
	Point & end() { return m_end; }
private:
	Point m_start, m_end;
};
ostream & operator<<(ostream & os, const Segment & s);
/********************************************************************************************/
void parseAndInit(const string & message, Segment & s);
const int calcLen(const Segment & s);
void solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter);
/**************************MENU****************************************/
void make(const string & message, Segment & s, SegmentSet & db);
void remove(const string & message, Segment & s, SegmentSet & db);
void find(const string & message, Segment & s, SegmentSet & db);
