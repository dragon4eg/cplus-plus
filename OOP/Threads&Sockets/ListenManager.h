#include "ListenThread.h"
#include <mutex>
#include <map>
using std::map;

class ListenManager //Meyers' singleton
{
public:
    static ListenManager & instance();
    void startNewListener (PCqueue< WorkItem > & wq, int sock);
    void deleteListener(const thread::id keyid);
    ~ListenManager();
private:
    ListenManager() { }
    ListenManager(const ListenManager &);
    ListenManager & operator=(const ListenManager &);
    map< thread::id, thread > listeners_pool_;
    mutex mutex_;
};
