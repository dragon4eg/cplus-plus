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
const string to_string (Point p)
{
	string result;
	return result = '(' + to_string(p.x()) + ')';
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
const string to_string (Segment s)
{
	string result;
	return result = to_string(s.start()) + '-' + to_string(s.end());
}

/*******************************AnswerItem************************************************/
AnswerItem::AnswerItem(const string message, const int number) : 
	message_(message), 
	number_(number) 
{ }
const string AnswerItem::getMessage() const { return message_; }
const int AnswerItem::getNumber() const { return number_; }

/*******************************WorkItem************************************************/
WorkItem::WorkItem(const string message, const int number, wqueue<AnswerItem*>* ans_queue) : 
	AnswerItem(message, number), 
	ans_queue_(ans_queue) 
{ }
/*******************************ProcThread***********************************************/
ProcThread::ProcThread(wqueue<WorkItem*>& queue, SegmentSet& db) : queue_(queue), db_(db) {}
void* ProcThread::run() 
{
	//Remove 1 item at a time and process it. 
	//Blocks if no items are available to process.
	for (int i = 0;; i++) 
        {
		cout<<"Processor is waiting for item...\n"<<(long unsigned int)self()<<' '<< i<<'\n';
		const WorkItem* item = queue_.remove();
		Segment tmps;
		wqueue<AnswerItem*>* ans_queue = item->getAnsQueue();
		const string msg = item->getMessage();
		const int listener_id = item->getNumber();
		switch (msg[0])
		{
			case 'f': Operations::find(msg, tmps, db_, ans_queue, listener_id);
				break;
			case 'm': Operations::make(msg, tmps, db_, ans_queue, listener_id);
				break;
			case 'r': Operations::remove(msg, tmps, db_, ans_queue, listener_id);
				break;
			default: //handle other inputs
				break;
		}
	cout<<"Processor: "<<(long unsigned int)self()<<" on loop "<<i<<", got one item:\n";
	cout<<"message: "<<item->getMessage()<<"from connection: "<<(long unsigned int)item->getNumber()<<'\n';
	delete item;
        }
        return NULL;
}
/*******************************ListenThread***********************************************/
ListenThread::ListenThread(wqueue<WorkItem*>/*&*/ * queue, int* socket) : queue_(queue), socket_(socket) {}
void* ListenThread::run()
{
	wqueue<AnswerItem*> answer_queue;
	int read_size;
	int sock = *(int*)socket_;
	string message, talk, answ;
	char client_message[25];
	talk = "Enter command and segment or type quit: \n";
	message = "Your handler id is " + to_string(self()) + '\n' + talk;
	write(sock, message.c_str(), message.length());
	while( (read_size = recv(sock, client_message, 25, 0)) > 0 )
	{
		if (client_message[0] == 'q')
		{
			cout<<"Connection: "<< self() <<" requested closing. Done!\n";
			break;
		}
		else
		{
			message = client_message;//this is OK
			//procThread deletes used item
			WorkItem* item = new WorkItem(message, self(), &answer_queue);
			(*queue_).add(item);//send the copy of a pointer to the WorkQueue
			const AnswerItem* answer = answer_queue.remove();
			if( answer->getNumber() != self() )
			{
				const string fatal_error = "Not my answer queue read! Fata Error. Closing connection!\n";
				write(sock, fatal_error.c_str(), fatal_error.length());
				break;
			}
			else
			{
				answ = answer->getMessage();
				write(sock, answ.c_str(), answ.length());
			}
			delete answer;
			write(sock, talk.c_str(), talk.length());
		}
	}
	close(*socket_);
	delete socket_;
	return NULL;
}
  
