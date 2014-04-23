#include "ListenManager.h"
#include <cassert>
#include <iostream>
using std::mutex;
using std::thread;
using std::lock_guard;
using std::move;
using std::cout;

ListenManager & ListenManager::instance() //no static here, only in header
{
    static ListenManager inst;
    return inst;
}
void ListenManager::startNewListener (PCqueue<WorkItem*> & wq, int sock) 
{
    lock_guard< mutex > lock(mutex_);
    thread listen_thread(ListenThread(wq, sock)); 
    //cout<<"id of temp thread: "<<listen_thread.get_id()<<'\n';
    listeners_pool_.emplace(listen_thread.get_id(), move(listen_thread));
    //all good with new keys, thread move constructor rules
    //TESTs
    //for (auto it = listeners_pool_.begin(); it != listeners_pool_.end(); ++it)
    //    cout<<"Id of mapped element: "<<it->second.get_id()<<'\n';
}
void ListenManager::deleteListener(const thread::id keyid)
{
    lock_guard< mutex > lock(mutex_);
    listeners_pool_.find(keyid)->second.join();
    //needed to be joined, otherwise destructor will call terminate();
    
    const size_t erased = listeners_pool_.erase(keyid);
    assert(erased == 1);
}
ListenManager::~ListenManager()
{
    for (auto & elem : listeners_pool_)
    elem.second.join();
}

