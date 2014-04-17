#include "Segment.h"

/**************************************Point******************************************/
Point & Point::operator= (Point & p)
{
       	x_ = p.x();
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
ostream & operator<<(ostream & os, const Segment & s)
{
	os<<s.start()<<'-'<<s.end();
	return os;
}
/*********************************Operations***********************************************/
void Operations::parseAndInit(const string & message, Segment & s)//we should construct a dflt segment before
{
	//incoming format <"fn"/"mk"/"rm">[3;4] or "quit" mk[3;5]
	const size_t delimiter = message.find(';');
	string tmp1 = message.substr(3, delimiter-3);
	string tmp2 = message.substr(delimiter+1, message.length()-1 - delimiter - 1);
	int a = stoi (tmp1);
	int b = stoi (tmp2);
	s.start().x() = min (a, b);
	s.end().x() = max (a, b);
}
void Operations::solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter)
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
const bool Operations::dbEmpty (const SegmentSet & db)
{
	if (db.empty())
	{
		cout<<"Empty Set!\n";
		return true;
	}
	return false;
}
void Operations::make(const string & message, Segment & s, SegmentSet & db)
{
	Operations::parseAndInit(message, s);
	db.insert(pair<int, Segment>(s.length(), s));
}
void Operations::remove(const string & message, Segment & s, SegmentSet & db)
{
	if (dbEmpty(db))
		return;
	Operations::parseAndInit(message, s);
	const int key = s.length();
	SegmentSet::iterator iter;
	if (db.find(key) != db.end())
	{
		iter = db.find(key);
	}else
	{
		cout<<"There is no such segment!\n";
		return;
	}
	const pair<SegmentSet::iterator, SegmentSet::iterator> p = db.equal_range(key);
	const int x = s.start().x();
	const int y = s.end().x();
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
void Operations::find(const string & message, Segment & s, SegmentSet & db)
{
	if (dbEmpty(db))
		return;
	Operations::parseAndInit(message, s);
	int key = s.length();
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
	Operations::solve(s, db, iter);
}
/*******************************WorkItem************************************************/
WorkItem::WorkItem(string message, int number) : message_(message), number_(number) {}
const string WorkItem::getMessage() { return message_; }
int WorkItem::getNumber() { return number_; }
/*******************************ProcThread***********************************************/
ProcThread::ProcThread(wqueue<WorkItem*>& queue, SegmentSet& db) : queue_(queue), db_(db) {}
void* ProcThread::run() 
{
//Remove 1 item at a time and process it. Blocks if no items are available to process.
	for (int i = 0;; i++) 
        {
		cout<<"Processor is waiting for item...\n"<<(long unsigned int)self()<<' '<< i<<'\n';
		WorkItem* item = queue_.remove();
		Segment tmps;
		switch ((item->getMessage())[0])
		{
			case 'f': Operations::find(item->getMessage(), tmps, db_);
				break;
			case 'm': Operations::make(item->getMessage(), tmps, db_);
				break;
			case 'r': Operations::remove(item->getMessage(), tmps, db_);
				break;
		}
	cout<<"Processor: "<<(long unsigned int)self()<<" loop "<<i<<" got one item:\n";
	cout<<"message: "<<item->getMessage()<<" from Listener: "<<(long unsigned int)item->getNumber()<<'\n';
	delete item;
        }
        return NULL;
}
/*******************************ListenThread***********************************************/
ListenThread::ListenThread(wqueue<WorkItem*>/*&*/ * queue, int* socket) : queue_(queue), socket_(socket) {}
void* ListenThread::run()
{
	int read_size;
	int sock = *(int*)socket_;
	string message, talk;
	char client_message[25];
	talk = "Enter command and segment or type quit: \n";
	message = "Your handler id is " + to_string(self()) + '\n' + talk;
	write(sock, message.c_str(), message.length());
	while( (read_size = recv(sock, client_message, 25, 0)) > 0 )
	{
		write(sock, talk.c_str(), talk.length());
		if (client_message[0] == 'q')
		{
			cout<<"Done!\n";
			break;
		}
		else
		{
			message = client_message;//this is OK
			WorkItem* item = new WorkItem(message, self());//procThread deletes used item
			(*queue_).add(item);//send the copy of a pointer to the WorkQueue
			delete item;
		}
	}
	delete socket_;
	return NULL;
}
  
