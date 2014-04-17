#ifndef __segment_h__
#define __segment_h__

#include <string>
#include <memory>
#include <iostream>
#include <map>
//#include <stdlib.h>
#include <algorithm>
#include "wqueue.h"
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
//#include <netdb.h> //hostent
#include <unistd.h>	//write

//#include <pthread.h>

using namespace std;
class Segment;
typedef multimap<int, Segment> SegmentSet; 
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
ostream & operator<<(ostream & os, const Point & p);
const string to_string (Point p);
/********************************************************************************************/
class Segment
{
public:
	Segment(): start_ (Point()), end_(Point()) {}
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
ostream & operator<<(ostream & os, const Segment & s);
const string to_string (Segment s);

class AnswerItem
{
public:
	AnswerItem(): message_(""), number_(0) { }
	AnswerItem(const string message, const int number);
	virtual ~AnswerItem() { }
	const string getMessage() const;
	const int getNumber() const;
	void putMessage(const string msg) { message_ = msg; }
	void putNumber(const int num) { number_ = num; }
private:
	string message_;
	int	number_;
};
/********************************************************************************************/
class WorkItem : public AnswerItem
{
public:
	WorkItem(const string message, const int number, wqueue<AnswerItem*>* ans_queue);
	wqueue<AnswerItem*>* getAnsQueue() const {return ans_queue_ ;}
private:
	wqueue<AnswerItem*>* ans_queue_;
};
/********************************************************************************************/
namespace Operations
{	//auxilary
	void parseAndInit(const string & message, Segment & s);
	const bool dbEmpty (const SegmentSet &, wqueue<AnswerItem*>*const, const int);
	void solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter, wqueue<AnswerItem*>*const, const int);

	//main
	void make(const string &, Segment &, SegmentSet &, wqueue<AnswerItem*>*const, const int);
	void remove(const string &, Segment &, SegmentSet &, wqueue<AnswerItem*>*const, const int);
	void find(const string &, Segment &, SegmentSet &, wqueue<AnswerItem*>*const, const int);
}
/********************************************************************************************/
class Thread
{
public:
	Thread();
	virtual ~Thread();
	int start();
	int join();
	int detach();
	pthread_t self(); 
	virtual void* run() = 0;  
private:
	pthread_t  m_tid;
	int 	   m_running;
	int 	   m_detached;
};

class ProcThread : public Thread
{
private:
 	wqueue<WorkItem*>& queue_;
 	SegmentSet& db_;
public:
	ProcThread(wqueue<WorkItem*>&, SegmentSet&);
	void* run();
};

class ListenThread : public Thread
{
private:
 	wqueue<WorkItem*>/*&*/ * queue_;
 	int* socket_;
public:
	ListenThread(wqueue<WorkItem*>/*&*/ *, int*);
	void* run();
};


#endif  



