#include "lineSegm.h"
Point & Point::operator= (Point & p)
{
       	m_x = p.x();
       	return *this;
}
const bool operator==(const Point & a, const Point & b)
{
	return a.x()==b.x();
}
ostream & operator<<(ostream & os, const Point & p)
{
	os<<'('<<p.x()<<')';
	return os;
}

/********************************************************************************************/
Segment::Segment(const Point a /* = Point() */, const Point b /* = Point() */):
       	m_start(a), 
	m_end(b) 
{ }
Segment::Segment(const Segment & s):
       	m_start(s.start()), 
	m_end(s.end()) 
{ }
ostream & operator<<(ostream & os, const Segment & s)
{
	os<<s.start()<<'-'<<s.end();
	return os;
}
void parseAndInit(const string & message, Segment & s)//we should construct a dflt segment before
{
	//incoming format <"fn"/"mk"/"rm">[3;4] or "quit" mk[3;5]
	size_t found_cm = message.find(';');
	string tmp1 = message.substr(3, found_cm-3);
	string tmp2 = message.substr(found_cm+1, message.length()-1 - found_cm - 1);
	int a, b;
	a = stoi (tmp1);
	b = stoi (tmp2);
	s.start().x() = min (a, b);
	s.end().x() = max (a, b);
}
const int calcLen(const Segment & s)
{
	return abs(s.end().x() - s.start().x());
} 

void solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter)
{
	do
	{
		int x = (*iter).second.start().x();//already ordered less
		int y = (*iter).second.end().x();//already ordered more
		int st =  s.start().x();
		int nd = s.end().x();
		int m = max(st, nd);//more
		int l = min(st, nd);//less
		if ( l >= x and m <= y )
		{
			cout<<"Solved! Segment "<<(*iter).second<<" includes "<<s<<'\n';
			return;
		}
		++iter;
	}
	while(iter != db.end());
	cout<<"There is no segment...\n";
}
/**************************MENU****************************************/
void make(const string & message, Segment & s, SegmentSet & db)
{
	parseAndInit(message, s);
	db.insert(pair<int, Segment>(calcLen(s), s));
}
void remove(const string & message, Segment & s, SegmentSet & db)
{
	if (db.empty())
	{
		cout<<"Nothing to remove, Vasya!\n";
		return;
	}
	parseAndInit(message, s);
	int key = calcLen(s);
	SegmentSet::iterator iter;
	if (db.find(key) != db.end())
	{
		iter = db.find(key);
	}else
	{
		cout<<"There is no such segment!\n";
		return;
	}
	pair<SegmentSet::iterator, SegmentSet::iterator> p;
	p = db.equal_range(key);
	int x = s.start().x();
	int y = s.end().x();
	bool xeqls = false;
	bool yeqls = false;
	for (auto i = p.first; i != p.second; ++i)
	{
		xeqls = ((*i).second.start().x() == s.start().x()) or ((*i).second.start().x() == s.end().x());
		yeqls = ((*i).second.end().x()) == s.start().x() or ((*i).second.end().x() == s.end().x());
		if (xeqls and yeqls)
		{
		       db.erase(i);
		       cout<<"Removed successfully!\n";
		       return;
		}
	}
	cout<<"Unfortunately, there is no such segment to erase. \n";
}
void find(const string & message, Segment & s, SegmentSet & db)
{
	if (db.empty())
	{
		cout<<"No answer! Empty Set!\n";
		return;
	}
	parseAndInit(message, s);
	int key = calcLen(s);
	SegmentSet::iterator iter;
	if (db.find(key) != db.end())
	{
		iter = db.find(key);
	}
	else if (db.find(key) == db.end())
	{
		if (db.upper_bound(key) != db.end())
		{
			iter = db.upper_bound(key);
			cout<<"upper key taken\n";
		}else
		{
			cout<<"There is no segment containing "<<s<<'\n';
			return;
		}
	}
	cout<<"Candidates are: \n";
	for (auto i = iter; i != db.end(); ++i)
		cout<<'['<<(*i).first<<'_'<<(*i).second<<"]\n";
	solve(s, db, iter);
}

