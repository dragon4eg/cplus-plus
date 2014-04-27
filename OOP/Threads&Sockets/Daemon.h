#pragma once
#include <memory>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write, close
#include <vector>
#include "PCqueue.h"
#include "WorkItem.h"
using std::vector;

//functor-daemon that will work while stop_ == false
//it accepts new connections and starts threads to 
//handle them, collects new opened sockets, and kills all
class Daemon
{
public:
    Daemon(const int & socket, const size_t & port): 
        main_socket_(socket),
        port_(port)
    { }
    void shutSocketsDown();
    void stopProcessing(PCqueue< WorkItem > &);
    Daemon (Daemon && other) = default;
    Daemon & operator=(const Daemon &) = delete;
    void operator()();        
private:
    size_t                    port_;
    bool                      stop_;
    int                       main_socket_;
    vector<int>               socket_pool_;   
    void showClientsInfo(const int & socket, sockaddr_in & client);
};     

