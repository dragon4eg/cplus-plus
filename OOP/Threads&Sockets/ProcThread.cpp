#include "ProcThread.h"
#include "AnswerItem.h"
#include "Operations.h"
#include "ListenManager.h"
#include <iostream>
using std::cout;
using std::mutex;

ProcThread::ProcThread(PCqueue< WorkItem > & queue, SegmentSet & db ) : 
    queue_(queue), 
    db_(db),
    stop_(false)
{ }
//TODO check if that was the Daemon who asked us to stop (with adding to ProcThread the daemon_id& as member)
//this is not needed untill we don't have proper user commands starting with '_'
const bool ProcThread::mayIDie(const string & msg, PCqueue< AnswerItem > & ans_queue, const std::thread::id id)
{
    if (msg == "__KILL_SERVER__INSTRUCTION__\n")
    {
        stop_ = true;
        AnswerItem item;
        item.putId(id);//security chek
        const string answer = "Processor is stopped!\n";
        item.putMessage(answer);
        ans_queue.add(item);
        return true;
    }
    return false;
}

void ProcThread::run() 
{
    //Removes 1 item at a time and process it. 
    //Waits if no items are available to process.
    for (size_t i = 0; !stop_; i++) 
    {
        cout<<"Processor "<<std::this_thread::get_id()<<" is waiting for item in loop "<< i <<'\n';
        WorkItem item(queue_.remove());
        Segment tmps;
        PCqueue< AnswerItem > & ans_queue = item.getAnsQueue();
        const string msg = item.getMessage();
        const auto listener_id = item.getId();
        mutex mtx; mtx.lock();
        cout<<"Processor: "<<std::this_thread::get_id()<<" on loop "<<i<<", got one item:\n";
        cout<<"message: "<<msg<<"from connection: "<<listener_id<<'\n';
        mtx.unlock();
        switch (msg[0])
        {
            case 'f': Operations::find(msg, tmps, db_, ans_queue, listener_id);
                break;
            case 'm': Operations::make(msg, tmps, db_, ans_queue, listener_id);
                break;
            case 'r': Operations::remove(msg, tmps, db_, ans_queue, listener_id);
                break;
            case 'q': ListenManager::instance().deleteListener(listener_id);
                break;
            case '_': { if (mayIDie(msg, ans_queue, listener_id)) continue; } 
                break;
            default: //handle other inputs
                break;
        }    
    }
}


