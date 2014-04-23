#include "Operations.h"
#include <string>
using std::stoi;
using std::min; 
using std::max;
using std::string;
using std::pair;
/*********************************Operations auxiliary****************************************/
void Operations::parseAndInit(const string & message, Segment & s)//we should construct a dflt segment before
{
    //incoming format <"fn"/"mk"/"rm">[3;4] or "quit" mk[3;5]
    const size_t delimiter = message.find(';');
    string tmp1 = message.substr(3, delimiter-3);
    string tmp2 = message.substr(delimiter+1, message.length()-1 - delimiter - 1);
    int a = stoi (tmp1);
    int b = stoi (tmp2);
    s.start() = min (a, b);
    s.end() = max (a, b);
}
/********************************/
void Operations::solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter, AnswerItem * const item)
{
    string answer;
    int x = (*iter).second.start();//already ordered less
    int y = (*iter).second.end();//already ordered more
    int st =  s.start();
    int nd = s.end();
    int m = max(st, nd);//more
    int l = min(st, nd);//less
    do
    {
        if ( l >= x and m <= y )
        {
            answer = "Solved! Segment " + static_cast< string >((*iter).second) + " includes " + static_cast< string >(s) + '\n';
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
void Operations::make(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem* > & ans_que, const std::thread::id id)
{
    AnswerItem*const item = new AnswerItem();
    item->putId(id);
    string answer;
    Operations::parseAndInit(message, s);
    db.insert(pair< int, Segment >(s.length(), s));
    answer = "Segment added!\n";
    item->putMessage(answer);
    ans_que.add(item);
}
/********************************/
void Operations::remove(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem* > & ans_que, const std::thread::id id)
{
    Operations::parseAndInit(message, s);
    const int key = s.length();
    SegmentSet::iterator iter;
    AnswerItem*const item = new AnswerItem();
    item->putId(id);
    string answer;
    if (db.empty())
    {
        answer = "Empty set!\n";
        item->putMessage(answer);
        ans_que.add(item);
        return;
    }
    if (db.find(key) != db.end())
    {
        iter = db.find(key);
    }else
    {
        answer = "There is no such segment!\n";
        item->putMessage(answer);
        ans_que.add(item);
        return;
    }
    const pair<SegmentSet::iterator, SegmentSet::iterator> p = db.equal_range(key);
    const int x = s.start();
    const int y = s.end();
    bool xeqls = false;
    bool yeqls = false;
    for (auto i = p.first; i != p.second; ++i)
    {
        xeqls = ((*i).second.start() == s.start()) or ((*i).second.start() == s.end());
        yeqls = ((*i).second.end()) == s.start() or ((*i).second.end() == s.end());
        if (xeqls and yeqls)
        {
            db.erase(i);
            answer = "Removed successfully!\n";
            item->putMessage(answer);
            ans_que.add(item);
            return;
        }
    }
    answer = "Unfortunately, there is no such segment to erase. \n";
    item->putMessage(answer);
    ans_que.add(item);
}
/********************************/
void Operations::find(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem* > & ans_que, const std::thread::id id)
{
    Operations::parseAndInit(message, s);
    int key = s.length();
    SegmentSet::iterator iter;
    AnswerItem*const item = new AnswerItem();
    item->putId(id);
    string answer;
    if (db.empty())
    {
        answer = "Empty set!\n";
        item->putMessage(answer);
        ans_que.add(item);
        return;
    }
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
            answer = "There is no segment containing " + static_cast< string >(s) + '\n';
            item->putMessage(answer);
            ans_que.add(item);
            return;
        }
    }
    //cout<<"Candidates are: \n";
    //for (auto i = iter; i != db.end(); ++i)
    //    cout<<'['<<(*i).first<<'_'<<(*i).second<<"]\n";
    Operations::solve(s, db, iter, item);
    ans_que.add(item);
}
