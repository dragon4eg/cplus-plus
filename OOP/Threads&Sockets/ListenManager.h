#pragma once
#include "WorkItem.h"
#include "PCqueue.h"
#include <mutex>
#include <map>
#include <thread>
using std::thread;
using std::map;
using std::pair;

class PoolItem
{
public:
    PoolItem(thread && tr):
        thread_(tr),
        stop_(false)
    { }
    PoolItem(PoolItem && other):
        thread_(move(other.thread_)),
        stop_(other.stop_)    
    { }
    thread thread_;
    bool stop_;
private:
};


typedef map< const thread::id, PoolItem > Pool;

class ListenManager //Meyers' singleton
{
public:
    static ListenManager & instance();
    void startNewListener (PCqueue< WorkItem > & wq, const int sock);
    void deleteListener(const thread::id keyid);
    ~ListenManager();
private:
    ListenManager() { }
    ListenManager(const ListenManager &);
    ListenManager & operator=(const ListenManager &);
    Pool pool_;
    mutex mutex_;
};
