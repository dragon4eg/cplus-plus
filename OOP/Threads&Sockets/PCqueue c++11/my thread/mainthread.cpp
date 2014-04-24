#include <thread>
#include <mutex>
#include <iostream>
#include "PCqueue.h"
using namespace std;


class Foo
{
    
public:
    int i_;
    Foo(int i = 5): i_(i){}
    Foo(Foo&& other): i_(other.i_) { other.i_ = 0; }
};

class Functor
{
public:
    Functor (PCqueue<Foo>&pcq, Foo f): pcq_(pcq), f_(f.i_){}
    void operator()(){ run(); }
private:
    void run() {  pcq_.add(move(f_)); cout<<"I've started!"<<f_<<'\n'; }
    int f_;
    PCqueue<Foo>& pcq_;
};


class Functor2
{
public:
    Functor2 (PCqueue<Foo>&pcq): pcq_(pcq){}
    void operator()(){ run(); }
private:
    void run() { cout<<"Functor2: "<<(pcq_.remove()).i_<<'\n'; }
    PCqueue<Foo>& pcq_;
};

const size_t number = 10;
thread prodpool[number];
thread conspool[number];

int main()
{
    Foo f1;
    PCqueue<Foo> pcq;
    cout<<f1.i_<<'\n';
        thread trd1(Functor(pcq, move(f1)));
    cout<<f1.i_<<'\n';
        thread trd2(Functor2(pcq));
        trd1.join();
        //trd2.join();
    return 0;
}

//ListenManager::instance().startNewListener(&queue, new_sock);

