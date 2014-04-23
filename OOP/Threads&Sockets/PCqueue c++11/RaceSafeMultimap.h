#pragma once
//#include <pthread.h>
#include <list>
#include <mutex>
#include <condition_variable>
#include <sys/wait.h>
//TODO exclude pthread and Thread with std::Thread and std::cond_variable
//add moveConstructor to Segment
//remove pointers everywhere even in Work and ans Items

using std::unique_lock;
using std::mutex;
using std::list;
using std::condition_variable;

typedef multimap< TKey, Tvalue >::const_iterator mapIterator
//carefully can be a map
template <typename TKey, typename TValue> class RaceSafeMultimap
{
private:
    multimap< TKey, Tvalue >   mmap_;
    mutex                      mutex_;
    condition_variable         condv_; 

public:
    RaceSafeMultimap() 
    {
        mutex mutex_;
        condition_variable condv_;
    }
    ~RaceSafeMultimap() { }
     begin() const
    {
        mutex_.lock();
        mapIterator iter =  mmap_.begin();
        mutex_.unlock();
        return iter;
    }
    size_t count (const Tkey& k) const;
    {
       
    }
    
    T remove() 
    {
        //mutex_.lock();
        unique_lock<std::mutex> lck (mutex_);
        //If the object currently owns a lock on the managed mutex object, 
        //its unlock member is called before destroying the object.
        while (queue_.size() == 0) 
        {
            condv_.wait (lck);
        }
        T item = queue_.front();
        queue_.pop_front();
        //mutex_.unlock();
        return item;
    }
    int size() 
    {
        mutex_.lock();
        int size = queue_.size();
        mutex_.unlock();
        return size;
    }
};
// --------------- - needed
//member functions:
//multimap::begin---------------
//multimap::cbegin
//multimap::cend
//multimap::clear
//multimap::count---------------
//multimap::crbegin
//multimap::crend
//multimap::emplace
//multimap::emplace_hint
//multimap::empty
//multimap::end------------------
//multimap::equal_range
//multimap::erase----------------
//multimap::find-----------------
//multimap::get_allocator
//multimap::insert---------------
//multimap::key_comp
//multimap::lower_bound
//multimap::max_size
//multimap::operator=
//multimap::rbegin
//multimap::rend
//multimap::size
//multimap::swap
//multimap::upper_bound----------
//multimap::value_comp

//member functions that has map, but not multimap:
//map::at
//map::operator[]


