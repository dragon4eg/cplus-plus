//#include <netdb.h> //hostent
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write, close
#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	//write, close socket
#include <sys/socket.h>
using namespace std;

//void showClientsInfo(const int & socket, sockaddr_in & client, const string & message)     

void dostuff (const int & socket_)
{
    int bytes_read {};
    stringstream message;
    const size_t maxMsgLen = 25;
    char chk, client_message[maxMsgLen];
    const string talk = "Enter q: \n";
    const thread::id myid = std::this_thread::get_id();
    message << "Your handler id is " << myid << '\n' << talk;
    write(socket_, message.str().c_str(), message.str().length());
    bool cond = true;
    while( cond )
    {
        bytes_read = recv(socket_, client_message, maxMsgLen, 0);//recv returns -1 or 0 if not OK
        if( bytes_read > 0) 
        {
            chk = client_message[0];
            if (chk == 'q')
            cout<<"message read\n";
        }
        else
        {
            const string down = "Server is being shutting down...\n";
            write(socket_, down.c_str(), down.length());
            cout<<down;
            cond = false;
        }
    }
}

void startListen(const int & socket_desc, const sockaddr_in & client, const int & c)
{
    int new_socket;
    new_socket = accept(socket_desc, (sockaddr *)&client, (socklen_t*)&c);
    thread listener(dostuff, new_socket);
    cout<<"Close socket now? yes\n";
    string ans = "";
    cin>>ans;
    if (ans == "y") 
    {   
        shutdown (new_socket, SHUT_RD);
    }
    listener.join();
    sleep(3);
    cout<<"Done!\n";
}

int main ()
{
    string testmessage("mk[234;554]");
    const size_t rbrace = testmessage.find(']');
    cout<<"length " << testmessage.length() << "rbrace pos "<<rbrace<<'\n';
    string strint = to_string(-12341);
    cout<<strint<<", length "<<strint.length();
    int socket_desc, c;
    sockaddr_in server, client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        cout<<"Could not create socket\n";
    }
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;    
    server.sin_port = htons( 8659 );//listening port for clients
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
//HERE WE GO.............................................................................
    thread serv(startListen, socket_desc, client, c);
    
/*************************************************************************************/
    serv.join();
    close(socket_desc);
    return 0;
}
