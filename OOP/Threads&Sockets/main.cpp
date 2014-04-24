//#include <netdb.h> //hostent
#include <arpa/inet.h> //inet_addr
#include "ProcThread.h"
#include "Operations.h"
#include "ListenManager.h"
using std::cout;
using std::cerr;
using std::to_string;
//static const size_t MACHINE_CORES = 4;

//TODO THE ONLY thing left is to make proper killing of listen manager and consequently all listenerThreads from inside...
//maybe we can fake send them "q_KILLME_"? not really from clients
//TODO change getting a commant just by first letter of input, make some better way of distinguishing them like an enumeration
//add moveConstructor to Segment and items
//remove pointers everywhere even in Work and Ans Items
// FIND A straaange BUG
void showClientsInfo(const int & socket, sockaddr_in & client, const string & message)    //find out clients ip and port
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

int main ()
{
    SegmentSet db;
    PCqueue<WorkItem>  pcqueue;
    thread processor_thread(ProcThread(pcqueue, db));
/*******************************Prepare Socket******************************************/    
    int socket_desc, new_socket, c;
    sockaddr_in server, client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        cout<<"Could not create socket\n";
    }
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;    
    server.sin_port = htons( 8100 );//listening port for clients
    if( bind(socket_desc,(sockaddr *)&server, sizeof(server)) < 0)
    {
        cout<<"Bind failed!!!\n"; // Why I always go here, what bind() really returns
    }
    else
    {
        cout<<"Bind done\n";
    }
    listen(socket_desc , 3); //3 is The backlog argument, defines the maximum length 
    //to which the queue of pending connections for sockfd may grow
    cout<<"Waiting for incoming connections...\n";   
    c = sizeof(sockaddr_in);
    const string message = "Hello Client, I have received your connection.\n";
//HERE WE GO...
    bool some_condition = true;//make a new thread-daemon that will work until condition
    
    while( some_condition )
    {
        new_socket = accept(socket_desc, (sockaddr *)&client, (socklen_t*)&c);
        if (new_socket<0)
        {
            cerr<<"Server internal error: accept failed! Terminating...\n";
            break;
        }
        showClientsInfo(new_socket, client, message);
        ListenManager::instance().startNewListener(pcqueue, new_socket);//new threads
    }
/*************************************************************************************/
    processor_thread.join();
    close(socket_desc);
    db.clear();
    return 0;
}
