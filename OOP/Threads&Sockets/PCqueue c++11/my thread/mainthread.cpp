#include <thread>
#include <mutex>
#include <map>
#include <iostream>
#include "PCqueue.h"
using namespace std;

int main()
{
    map<int, string> mp;
    string  o0 = "", a1 = "a", b2 = "bb", c3 = "ccc", d4 = "dddd", e5 = "eeeee", f6 = "ffffff";
    mp.insert(make_pair(a1.length(),a1));
    mp.insert(make_pair(b2.length(),b2));
    mp.insert(make_pair(c3.length(),c3));
    auto iter = mp.insert(make_pair(e5.length(),e5));
    mp.insert(make_pair(f6.length(),f6));
    cout<<"Element of lenght 5 has length " <<iter.first->first <<' '<<iter.first->second<<'\n';
    mp.insert(make_pair(d4.length(),d4));
    cout<<"Element of lenght 5 has length " <<iter.first->first <<' '<<iter.first->second<<'\n';
    mp.insert(make_pair(o0.length(),o0));
    cout<<"Element of lenght 5 has length " <<iter.first->first <<' '<<iter.first->second<<'\n';
    return 0;
}

//ListenManager::instance().startNewListener(&queue, new_sock);

