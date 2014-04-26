#pragma once
#include "WorkItem.h"
#include "PCqueue.h"
#include <mutex>
#include <map>
#include <thread>
using std::thread;
using std::map;

typedef map< const thread::id, thread > Pool;

class ListenManager
{
public:
    static ListenManager & instance();
    void startNewListener (PCqueue< WorkItem > & wq, const int & sock);
    void deleteListener(const thread::id keyid);
    void closeAll();
    ~ListenManager();
private:
    ListenManager() { }
    ListenManager(const ListenManager &);
    ListenManager & operator=(const ListenManager &);
    Pool pool_;
    mutex mutex_;
};

