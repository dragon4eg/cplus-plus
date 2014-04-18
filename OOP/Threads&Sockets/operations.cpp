#include "Segment.h"

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

void Operations::solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter, AnswerItem*const item)
{
	string answer;
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
			answer = "Solved! Segment " + to_string((*iter).second) + " includes " + to_string(s) + '\n';
			item->putMessage(answer);
			return;
		}
		++iter;
	}
	while(iter != db.end());
	answer = "There is no segment...\n";
	item->putMessage(answer);
}
/*********************************Operations main***********************************************/
void Operations::make(const string & message, Segment & s, SegmentSet & db, wqueue<AnswerItem*>* ans_que, const int id)
{
	AnswerItem*const item = new AnswerItem();
	item->putNumber(id);
	string answer;
	Operations::parseAndInit(message, s);
	db.insert(pair<int, Segment>(s.length(), s));
	answer = "Segment added!\n";
	item->putMessage(answer);
	(*ans_que).add(item);
}

void Operations::remove(const string & message, Segment & s, SegmentSet & db, wqueue<AnswerItem*>* ans_que, const int id)
{
	AnswerItem*const item = new AnswerItem();
	item->putNumber(id);
	string answer;
	if (db.empty())
	{
		answer = "Empty set!\n";
		item->putMessage(answer);
		(*ans_que).add(item);
		return;
	}
	Operations::parseAndInit(message, s);
	const int key = s.length();
	SegmentSet::iterator iter;
	if (db.find(key) != db.end())
	{
		iter = db.find(key);
	}else
	{
		answer = "There is no such segment!\n";
		item->putMessage(answer);
		(*ans_que).add(item);
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
			answer = "Removed successfully!\n";
			item->putMessage(answer);
			(*ans_que).add(item);
			return;
		}
	}
	answer = "Unfortunately, there is no such segment to erase. \n";
	item->putMessage(answer);
	(*ans_que).add(item);
}

void Operations::find(const string & message, Segment & s, SegmentSet & db, wqueue<AnswerItem*>*const ans_que, const int id)
{
	AnswerItem*const item = new AnswerItem();
	item->putNumber(id);
	string answer;
	if (db.empty())
	{
		answer = "Empty set!\n";
		item->putMessage(answer);
		(*ans_que).add(item);
		return;
	}
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
			//cout<<"upper key taken\n";
		}else
		{
			answer = "There is no segment containing " + to_string(s) + '\n';
			item->putMessage(answer);
			(*ans_que).add(item);
			return;
		}
	}
	//cout<<"Candidates are: \n";
	//for (auto i = iter; i != db.end(); ++i)
	//	cout<<'['<<(*i).first<<'_'<<(*i).second<<"]\n";
	Operations::solve(s, db, iter, item);
	(*ans_que).add(item);
}
