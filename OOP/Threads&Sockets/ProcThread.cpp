#include "ProcThread.h"
#include "AnswerItem.h"
#include "Operations.h"
#include "ListenManager.h"
using std::cout;

ProcThread::ProcThread(PCqueue< WorkItem * > & queue, SegmentSet & db) : 
    queue_(queue), 
    db_(db)
{ }

void ProcThread::run() 
{
    //Removes 1 item at a time and process it. 
    //Waits if no items are available to process.
    for (int i = 0;; i++) 
    {
        cout<<"Processor "<<std::this_thread::get_id()<<" is waiting for item in loop "<< i <<'\n';
        const WorkItem * item = queue_.remove();
        Segment tmps;
        PCqueue< AnswerItem * > & ans_queue = item->getAnsQueue();
        const string msg = item->getMessage();
        const auto listener_id = item->getId();
        switch (msg[0])
        {
            case 'f': Operations::find(msg, tmps, db_, ans_queue, listener_id);
                break;
            case 'm': Operations::make(msg, tmps, db_, ans_queue, listener_id);
                break;
            case 'r': Operations::remove(msg, tmps, db_, ans_queue, listener_id);
                break;
            case 'q': { cout<<"Proc: Connection: "<< listener_id <<" requested closing.\n"; ListenManager::instance().deleteListener(listener_id); }
                break;
            default: //handle other inputs
                break;
        }
        cout<<msg<<'\n';
        cout<<"Processor: "<<std::this_thread::get_id()<<" on loop "<<i<<", got one item:\nmessage: "<<msg<<"from connection: "<<listener_id<<'\n';
    
    delete item;
    }
}
