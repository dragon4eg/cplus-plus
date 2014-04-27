#include "ListenManager.h"
#include "ListenThread.h"
#include <cassert>
#include <iostream>
using std::mutex;
using std::lock_guard;

ListenManager & ListenManager::instance()
{
    static ListenManager inst;
    return inst;
}
void ListenManager::startNewListener (PCqueue< WorkItem > & wq, const int & sock) 
{
    lock_guard< mutex > lock(mutex_);
    thread listener(ListenThread(wq, sock));
    thread::id id = listener.get_id();
    pool_.emplace(id, move(listener));
}
void ListenManager::deleteListener(const thread::id keyid)
{
    lock_guard< mutex > lock(mutex_);
    pool_.find(keyid)->second.join();    
    const size_t erased = pool_.erase(keyid);
    assert(erased == 1);
}
void ListenManager::closeAll()
{
    for (auto & elem : pool_)
    elem.second.join();
}
ListenManager::~ListenManager() { }


