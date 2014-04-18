#include <memory>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <cmath>
using namespace std;
/********************************************************************************************/
class Point
{
public:
	Point(const int x = 0, const int y = 0): m_x(x), m_y(y) { }
	Point(const Point & p): m_x(p.x()), m_y(p.y()) { }
	~Point() { }
	const int & x() const { return m_x;}
	const int & y() const { return m_y;}
	int & x() { return m_x;}
	int & y() { return m_y;}
	Point & operator= (Point & p){ m_x = p.x(); m_y = p.y(); return *this;}
private:
	int m_x, m_y;
};

const bool operator==(const Point & a, const Point & b)
{
	return a.x()==b.x() && a.y()==b.y();
}
ostream& operator<<(ostream & os, const Point & p)
{
	os<<'('<<p.x()<<", "<<p.y()<<')';
	return os;
}
/********************************************************************************************/
class Segment
{
public:
	Segment(const Point a = Point(), const Point b = Point()): m_start(a), m_end(b) { }
	Segment(const Segment & s): m_start(s.start()), m_end(s.end()) { }
	~Segment() { }
	const Point & start() const { return m_start; }
	const Point & end() const { return m_end; }
	Point & start() { return m_start; }
	Point & end() { return m_end; }
private:
	Point m_start, m_end;
};
ostream & operator<<(ostream & os, const Segment & s)
{
	os<<s.start()<<'-'<<s.end();
	return os;
}
const bool checkIfParallel(const Segment & s)
{
	return s.start().x() == s.end().x();
}
/********************************************************************************************/
struct SegmParams
{
	int k, b; 
	float length;
	void calculateSegmParams(const Segment & s);
};
//we might need also operator < for multimap to order!!!!!!!!yep it was true
bool operator==(const SegmParams & l1, const SegmParams & l2)
{
	return l1.k == l2.k && l1.b == l2.b;
}
bool operator<(const SegmParams & l1, const SegmParams & l2)
{	//check k (tangent)
	if (l1.k < l2.k)
	{
		return true;
	}
	//check b (displacement)	
	if (l1.k == l2.k)
	{
		if (l1.b < l2.b)
			return true;
		//check length  
		if (l1.b == l2.b)
		{
			if(l1.length < l2.length)
				return true;
		}
	}
	return false;
}
void SegmParams::calculateSegmParams(const Segment & s)
{
	//k = (y2-y1)/(x2-x1)
	k = (s.end().y() - s.start().y() )/(s.end().x() - s.start().x());
	//b = y1-k*x1
	b = s.start().y() - k * s.start().x();
	length = sqrt(abs(abs(s.start().x()) - abs(s.end().x())) + abs(abs(s.start().y()) - abs(s.end().y())))
}
ostream & operator<<(ostream & os, const SegmParams & l)
{
	os<<"SegmParams: "<<l.k<<','<<l.b;
	return os;
}
/********************************************************************************************/
struct SegmentSet
{
	 unique_ptr<multimap<SegmParams, Segment>> oKsegments;
	 unique_ptr<multimap<int, Segment>> oYparallel;

};
ostream & operator<<(ostream & os, const multimap<int, Segment> m)
{
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		os<<(*it).first<<" _ "<<(*it).second<<'\n';
	}
	return os;
}
//ostream & operator<<(ostream & os, pair<int, Segment> paral)
//{
//	os<<"oYparalEntry: [ k="<<paral.first<<" | "<<paral.second<<" ]";
//	return os;
//}
//ostream & operator<<(ostream & os, const multimap<SegmParams, Segment>::iterator normal)
//{
//	os<<"oKsegmEntry: [ k="<<normal->first<<" | "<<normal->second<<" ]";
//	return os;
//}
void parseAndInit(const string & message, Segment & s)//we should construct a dflt segment before
{
	//incoming format <"fn"/"mk"/"rm">[234,54223;23443,2342] or "quit" mk[3,4;4,5]
	size_t found_cm = message.find(',');
	size_t found_sc = message.find(';');
	string x_tmp = message.substr(3, found_cm-3);
	string y_tmp = message.substr(found_cm+1, found_sc - found_cm-1);
	int x = stoi(x_tmp);
	int y = stoi(y_tmp);
	Point st(x, y);
	s.start() = st;
	found_cm = message.find(',', found_sc+1);
	x_tmp = message.substr(found_sc+1, found_cm - found_sc-1);
	y_tmp = message.substr(found_cm+1, message.length()-1 - found_sc-1);
	x = stoi(x_tmp);
	y = stoi(y_tmp);
	Point en(x, y);
	s.end() = en;
}
/********************************************************************************************/
void make(const string & message, Segment & s, SegmentSet & db)
{
	parseAndInit(message, s);
	if (checkIfParallel(s))
	{
		(*db.oYparallel).insert(pair<int, Segment>(s.start().x(), s));
		cout<<"inserted to parallel:\nb: _ Segment:\n"<<*db.oYparallel;
		return;
	}
	SegmParams l;
	l.calculateSegmParams(s);
	(*db.oKsegments).insert(pair<SegmParams, Segment>(l, s));
	cout<<"Added!"<<'\n';
}	
void find(const string & message, Segment & s, SegmentSet & db)
{
	parseAndInit(message, s);
	if (checkIfParallel(s))
	{
		int b = s.start().x();
		cout<<"We have "<<(*db.oYparallel).count(b)<<" candidates to search...\n";
		pair<multimap<int, Segment>::iterator, multimap<int, Segment>::iterator> pmrange;
		//equal_range(b) returns pair<iterator,iterator> representing the range of element with key b
		pmrange = (*db.oYparallel).equal_range(b);
		for (auto it = pmrange.first; it != pmrange.second; ++it)
		{
			cout<<(*it).first<< " "<<(*it).second<<'\n';
		}
		//***TODO*TODO*TODO***SOLVE ON ONE LINE!!!!!!!!!!!*********TODO*TODO******
	}
}

int main()
{
	//multimap< multimap<Line, int length>, Segment >
	SegmentSet db {
	unique_ptr<multimap<SegmParams, Segment>>(new multimap<SegmParams, Segment>()),
	unique_ptr<multimap<int, Segment>>(new multimap<int, Segment>())};
	string message;//read and passed here by the server
	do
	{
		cout<<"Enter command and segment or type quit: \n";
		cin>>message;
		if (message[0] == 'q')
		{
			cout<<"Done!\n";
			break;
		}
		Segment tmps;
		switch (message.at(0))
		{
			case 'f': find(message, tmps, db);
				break;
			case 'm': make(message, tmps, db);
				break;
			//case 'r': remove();
			//	break;
		}
	}
	while(true);

	return 0;
}
