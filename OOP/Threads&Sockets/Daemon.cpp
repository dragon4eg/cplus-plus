#include "Daemon.h"
#include "ProcThread.h"
#include "ListenManager.h"
#include <thread>
#include <iostream>
using std::to_string;
using std::cout;
void Daemon::operator()()
{
    int  new_socket, c; 
    size_t max_errors = 5;//MAX NUMBERS OF ERRORS INDICATING THAT EITHER WE'VE GOT 
    //PROBLEMS WITH CONNECTION (so we can't work) OR WE'RE FORCEFULLY SHUTING DOWN
    //TODO find out errors returned by accept() to distinguish the shutDown and problems
    sockaddr_in server, client;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;    
    server.sin_port = htons( port_ );//listening port taken from main()
    if( bind(main_socket_,(sockaddr *)&server, sizeof(server)) < 0)
    {
        cout<<"Bind failed!!!Run again and enter another port [1...65535].\n";
        return;
    }
    else
        cout<<"Bind done\n";
    listen(main_socket_, 3); //3 defines the maximum number of pending connections
    SegmentSet db;
    PCqueue<WorkItem> pcqueue;
    thread processor_thread(ProcThread(pcqueue, db));
    cout<<"Waiting for incoming connections...\n";
    c = sizeof(sockaddr_in);
    const string message = "Hello Client, I have received your connection.\n";
    size_t error_counter = 0;
    while( !stop_ )
    {
        new_socket = accept(main_socket_, (sockaddr *)&client, (socklen_t*)&c);
        if (new_socket<=0)
        {
            ++error_counter;
            cout<<"Server shutting down in: "<<max_errors - error_counter<<'\n';
            sleep(1);
            if (error_counter >= max_errors)
            {
                shutSocketsDown();
            }
            else
                continue;
        }
        else
        {
            socket_pool_.push_back(new_socket);//put socket into a pool, so we could shut id down in Manager
            showClientsInfo(new_socket, client, message);
            ListenManager::instance().startNewListener(pcqueue, new_socket);//starting thread per connection
        }
    }
    stopProcessing(pcqueue);
    processor_thread.join();
    db.clear();
}
//find out clients ip and port
void Daemon::showClientsInfo(const int & socket, sockaddr_in & client, const string & message)
{
    string client_ip, info;
    int client_port;
    cout<<"Connection accepted!\n";
    client_ip = inet_ntoa(client.sin_addr);
    client_port = ntohs(client.sin_port);
    info = message + "Your ip is: " + client_ip + " port: " + to_string(client_port) + '\n';
    write(socket, info.c_str(), info.length());
    cout<<"Clients ip is: "<<client_ip<<" port: "<<client_port<<'\n';
}
//close all sockets and set condition for Daemon's loop to stop
void Daemon::shutSocketsDown() 
{
    stop_ = true;
    for (auto & i : socket_pool_) 
        shutdown(i, SHUT_RD); //SHUT_RD - disable reading, so we could last time notify users, we're closing
}
//stop Processing Thread
void Daemon::stopProcessing(PCqueue< WorkItem > & pcqueue)
{
    PCqueue< AnswerItem > answer_queue;// create my own anser queue
    const string lastmsg = "__KILL_SERVER__INSTRUCTION__\n";
    WorkItem item(lastmsg, std::this_thread::get_id(), answer_queue);
    pcqueue.add(item);
    AnswerItem answer = answer_queue.remove();
    cout<<answer.getMessage();
    ListenManager::instance().closeAll();
}

