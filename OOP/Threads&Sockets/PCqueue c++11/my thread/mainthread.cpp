#include <thread>
#include <mutex>
#include <iostream>
using namespace std;


class Functor
{
public:
    Functor (int i): k_(i){}
    void operator()(){ run(); }
private:
    void run() {  cout<<"I've started!\nk1 = "<<k_<<" k2 = "<<++k_<<'\n'; }
    int k_;
    //mutex mtx;
};

const size_t number = 10;
thread pool[number];

int main()
{
    for (size_t i = 0; i<number; ++i)
    {
        pool[i] = thread(Functor(i));
    }
    for (size_t i = 0; i<number; ++i)
    {
        pool[i].join();
    }
    return 0;
}

//ListenManager::instance().startNewListener(&queue, new_sock);

