#include "Operations.h"
#include <stdexcept>      // std::invalid_argument
#include <sstream>
#include <string>
using std::stoi;
using std::min; 
using std::max;
using std::cout;
using std::string;
using std::make_pair;
using std::stringstream;
using std::to_string;

const bool Operations::isDbEmpty(const SegmentSet & db, AnswerItem & item, PCqueue< AnswerItem > & ans_que)
{
    if (db.empty())
    {
        const string answer = "Empty set!\n";
        item.putMessage(answer);
        ans_que.add(item);
        return true;
    }
    return false;
}
SegmentSet::const_iterator Operations::findSameSegment(const SegmentSet & db, Segment & s)
{
    const auto iterpair = db.equal_range(s.length());
    const int a = min(s.start(), s.end());
    const int b = max(s.start(), s.end());
    int x, y;
    for (auto i = iterpair.first; i != iterpair.second; ++i)
    {
        x = (*i).second.start();//already ordered less - start of segment in database
        y = (*i).second.end();//already ordered more - end
        if (a == x and b == y)
            return i;
    }  
    return db.end();
}    
const bool Operations::parseAndInit(const string & message, Segment & s, AnswerItem & item, const std::thread::id id)
{
    //incoming format <"fn"/"mk"/"rm">[3;4] or "quit" mk[3;5]  
    item.putId(id);//security chek
    string answer = "", tmp1, tmp2;
    int a, b;
    item.putMessage(answer);
    const size_t lbrace = message.find('[');
    const size_t rbrace = message.find(']');
    const size_t delimiter = message.find(';');
    if (lbrace == std::string::npos or rbrace == std::string::npos or delimiter == std::string::npos or rbrace != (message.length() - 1))
    {                                                    
        answer = "Parser: Bad input! Try h/help.\n";
        item.putMessage(answer);
        return false;
    }
    try
    {
        tmp1 = message.substr(lbrace + 1, delimiter - lbrace - 1);
        tmp2 = message.substr(delimiter + 1, rbrace - delimiter - 1);
        a = stoi (tmp1);
        b = stoi (tmp2);
        if ((to_string(a).length() < tmp1.length()) or (to_string(b).length() < tmp2.length()))
        {
            throw string("bad numers entered!");
        }
        s.start() = min (a, b); //Lets hold points of Segment ordered
        s.end() = max (a, b);
        return true;
    }
    catch ( const std::invalid_argument & except )
    {
        cout<<"Exception (invalid_argument) catched! "<<except.what()<<'\n';
        answer = "Not a proper numeric parameter! Try rm[a;b], fn[a;b], mk[a;b] or q/quit.\n";
        item.putMessage(answer);
    }
    catch ( const string & badnum )
    {
        cout<<"Exception catched! "<< badnum <<'\n';
        answer = "Bad numbers in parameter! Try rm[a;b], fn[a;b], mk[a;b] or q/quit.\n";
        item.putMessage(answer);
    }
    catch ( const std::out_of_range& oor )
    {
        cout<<"Exception (out_of_range) catched! "<<oor.what()<<'\n';
        answer = "Out of range numeric parameter! Try rm[a;b], fn[a;b], mk[a;b] or q/quit.\n";
        item.putMessage(answer);
    }
    return false;
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
    answer << "No found any segment containing " << s << '\n';
    item.putMessage(answer.str());
    return;
}
void Operations::make(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem > & ans_que, const std::thread::id id)
{
    stringstream answer;
    AnswerItem item;
    if ( Operations::parseAndInit(message, s, item, id) != true )
    {
        ans_que.add(item);
        return;
    }
    else
    {
        auto it = findSameSegment(db, s);
        if ( it != db.end())
        {
            answer << "There is the same already!\n";
            item.putMessage(answer.str());
            ans_que.add(item);
            return;
        }
        db.insert(make_pair(s.length(), s));
        answer << "Segment "<<s<<" added!\n";
        item.putMessage(answer.str());
        ans_que.add(item);
        return;
    }    
}
void Operations::remove(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem > & ans_que, const std::thread::id id)
{
    AnswerItem item;
    if ( Operations::parseAndInit(message, s, item, id) != true )
    {
        ans_que.add(item);
        return;
    }
    else
    {
        const int key = s.length();
        SegmentSet::iterator iter;
        string answer;
        if (isDbEmpty(db, item, ans_que))
            return;
        iter = db.find(key);
        if (iter == db.end())//search if there are with the same length
        {
            answer = "There is no such segment!\n";
            item.putMessage(answer);
            ans_que.add(item);
            return;
        }
        auto it = findSameSegment(db, s);
        if (it != db.end())
        {
            db.erase(it);
            answer = "Removed successfully!\n";
            item.putMessage(answer);
            ans_que.add(item);
            return;
        }
        answer = "Unfortunately, there is no such segment to erase. \n";
        item.putMessage(answer);
        ans_que.add(item);
        return;
    }
}
void Operations::find(const string & message, Segment & s, SegmentSet & db, PCqueue< AnswerItem > & ans_que, const std::thread::id id)
{
    AnswerItem item;
    if ( Operations::parseAndInit(message, s, item, id) != true )
    {
        ans_que.add(item);
        return;
    }
    int key = s.length();
    SegmentSet::iterator iter;
    stringstream answer;
    if (isDbEmpty(db, item, ans_que))
        return;
    if (db.find(key) != db.end())//search if there are with the same length
    {
        iter = db.find(key);
    }
    else// if (db.find(key) == db.end())//search if there are longer
    {
        if (db.upper_bound(key) != db.end())
        {
            iter = db.upper_bound(key);
        }
        else
        {
            answer << "There is no segment containing " << s << '\n';
            item.putMessage(answer.str());
            ans_que.add(item);
            return;
        }
    }
    Operations::solve(s, db, iter, item);
    ans_que.add(item);
    return;
}

