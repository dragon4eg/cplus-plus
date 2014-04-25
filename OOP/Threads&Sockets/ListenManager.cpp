#include "ListenManager.h"
#include "ListenThread.h"
#include <cassert>
#include <iostream>
#include <tuple>
#include <utility>
using std::swap;
using std::mutex;
using std::lock_guard;
using std::piecewise_construct;


ListenManager & ListenManager::instance() //no static here, only in header
{
    static ListenManager inst;
    return inst;
}
void ListenManager::startNewListener (PCqueue< WorkItem > & wq, const int sock) 
{
    lock_guard< mutex > lock(mutex_);
    thread listener (ListenThread(wq, sock, pool_));
    PoolItem item(move(listener), false);
    thread::id id = item.thread_.get_id();
    pool_.emplace(id, move(item) );
    //all good with new keys in a map, thread move constructor rules
    //TEST
    //for (auto it = pool_.begin(); it != pool_.end(); ++it)
    //    cout<<"Id of mapped element: "<<it->second.get_id()<<'\n';
}
void ListenManager::deleteListener(const thread::id keyid)
{
    lock_guard< mutex > lock(mutex_);
    pool_.find(keyid)->second.first.join();
    //needed to be joined, otherwise destructor will call terminate();    
    const size_t erased = pool_.erase(keyid);
    assert(erased == 1);
}
ListenManager::~ListenManager()
{
    for (auto & elem : pool_)
    elem.second.first.join();
}

/*
    lock_guard< mutex > lock(mutex_);
    thread listener(ListenThread(wq, sock, pool_));
    thread::id id = listener.get_id();
    pool_.emplace(piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(move(listener), false) );
*/

