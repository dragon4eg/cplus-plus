//#include <netdb.h> //hostent
#include <arpa/inet.h> //inet_addr
#include "ProcThread.h"
#include "Operations.h"
#include "ListenManager.h"
using std::cout;
using std::cerr;
using std::to_string;

//static const size_t MACHINE_CORES = 4;

//TODO
//add moveConstructor to Segment and items
//remove pointers everywhere even in Work and Ans Items
// FIND A straaange BUG

int main ()
{
    SegmentSet db;
    PCqueue<WorkItem*>  pcqueue;
    thread processor_thread(ProcThread(pcqueue, db));
/*******************************Socket************************************************/    
    int socket_desc, new_socket, c; //*new_sock;
    sockaddr_in server, client;
    string message, quit;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        cout<<"Could not create socket\n";
    }
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons( 8087 );//listening port for clients
    
    if( bind(socket_desc,(sockaddr *)&server, sizeof(server)) < 0)
    {
        cout<<"Bind failed!!!\n"; // Why I always go here, what bind() really returns
    }
    else{
    cout<<"Bind done\n";
    }
    listen(socket_desc , 3); //3 is The backlog argument defines the maximum length 
    //to which the queue of pending connections for sockfd may grow
    cout<<"Waiting for incoming connections...\n";   
    c = sizeof(struct sockaddr_in);
    message = "Hello Client, I have received your connection.\n";
        
    while( (new_socket = accept(socket_desc, (sockaddr *)&client, (socklen_t*)&c)) )
    {
        string client_ip, info;
        int client_port;
        cout<<"Connection accepted!\n";
        //find out clients ip and port
        client_ip = inet_ntoa(client.sin_addr);
        client_port = ntohs(client.sin_port);
        info = message+ "Your ip is: " + client_ip + " port: " + to_string(client_port) + '\n';
        write(new_socket, info.c_str(), info.length());
        cout<<"Clients ip is: "<<client_ip<<" port: "<<client_port<<'\n';
        //new_sock = new int(new_socket); //Listener delets this new_sock
        ListenManager::instance().startNewListener(pcqueue, new_socket);
    }
    if (new_socket<0)
    {
        cerr<<"accept failed\n";
    }
/*************************************************************************************/
    processor_thread.join();
    close(socket_desc);
    db.clear();
    return 0;
}
