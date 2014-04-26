#include "Operations.h"
#include <sstream>
#include <string>
using std::stoi;
using std::min; 
using std::max;
using std::string;
using std::make_pair;
using std::stringstream;

void Operations::parseAndInit(const string & message, Segment & s)
{
    //incoming format <"fn"/"mk"/"rm">[3;4] or "quit" mk[3;5]
    const size_t delimiter = message.find(';');
    string tmp1 = message.substr(3, delimiter-3);
    string tmp2 = message.substr(delimiter+1, message.length()-1 - delimiter - 1);
    int a = stoi (tmp1);
    int b = stoi (tmp2);
    s.start() = min (a, b); //Lets hold points of Segment ordered
    s.end() = max (a, b);
}
void Operations::solve(const Segment & s, const SegmentSet & db, SegmentSet::iterator iter, AnswerItem & item)
{
    stringstream answer;
    const int a = min(s.start(), s.end());//less - start of current segment
    const int b = max(s.start(), s.end());//more - end
    do
    {
        const int x = (*iter).second.start();//already ordered less - start of segment in database
        const int y = (*iter).second.end();//already ordered more - end
        if ( a >= x and b <= y )
        {
            answer<< "Solved! Segment "<< ((*iter).second) << " includes " << s << '\n';
            item.putMessage(answer.str());
            return;
        }
        ++iter;
    }
    while(iter != db.end());
    answer <<"There is no segment...\n";
    item.putMessage(answer.str());
}
void Operations::make(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem > & ans_que, const std::thread::id id)
{
    AnswerItem item;
    item.putId(id);//security chek
    stringstream answer;
    Operations::parseAndInit(message, s);
    db.insert(make_pair(s.length(), s));
    answer << "Segment added!\n";
    item.putMessage(answer.str());
    ans_que.add(item);
}
void Operations::remove(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem > & ans_que, const std::thread::id id)
{
    Operations::parseAndInit(message, s);
    const int key = s.length();
    SegmentSet::iterator iter;
    AnswerItem item;
    item.putId(id);
    string answer;
    if (db.empty())
    {
        answer = "Empty set!\n";
        item.putMessage(answer);
        ans_que.add(item);
        return;
    }
    if (db.find(key) != db.end())//search if there are with the same length
    {
        iter = db.find(key);
    }else
    {
        answer = "There is no such segment!\n";
        item.putMessage(answer);
        ans_que.add(item);
        return;
    }
    const auto iterpair = db.equal_range(key);
    const int a = min(s.start(), s.end());
    const int b = max(s.start(), s.end());
    for (auto i = iterpair.first; i != iterpair.second; ++i)
    {
        const int x = (*i).second.start();//already ordered less - start of segment in database
        const int y = (*i).second.end();//already ordered more - end
        if (a == x and b == y)
        {
            db.erase(i);
            answer = "Removed successfully!\n";
            item.putMessage(answer);
            ans_que.add(item);
            return;
        }
    }
    answer = "Unfortunately, there is no such segment to erase. \n";
    item.putMessage(answer);
    ans_que.add(item);
}
void Operations::find(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem > & ans_que, const std::thread::id id)
{
    Operations::parseAndInit(message, s);
    int key = s.length();
    SegmentSet::iterator iter;
    AnswerItem item;
    item.putId(id);
    stringstream answer;
    if (db.empty())
    {
        answer << "Empty set!\n";
        item.putMessage(answer.str());
        ans_que.add(item);
        return;
    }
    if (db.find(key) != db.end())//search if there are with the same length
    {
        iter = db.find(key);
    }
    else// if (db.find(key) == db.end())//search if there are longer
    {
        if (db.upper_bound(key) != db.end())
        {
            iter = db.upper_bound(key);
        }else
        {
            answer << "There is no segment containing " << s << '\n';
            item.putMessage(answer.str());
            ans_que.add(item);
            return;
        }
    }
    Operations::solve(s, db, iter, item);
    ans_que.add(item);
}

