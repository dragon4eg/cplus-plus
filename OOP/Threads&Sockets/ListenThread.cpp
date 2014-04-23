#include "ListenThread.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>	//write, close socket
#include <sys/socket.h>
using std::cout;
using std::stringstream;
/*******************************ListenThread***********************************************/
ListenThread::ListenThread(PCqueue<WorkItem*> & queue, int socket) : queue_(queue), socket_(socket), workable_(true) {}

void ListenThread::killMeSoftly(PCqueue<AnswerItem*> & answer_queue)
{
    //pass 'q' = "KILLME" to the PCqueue so that the ProcThread
    //asked the ListenManager to close this thread
    cout<<"Connection: "<< std::this_thread::get_id() <<" is about to close.\n";
    stringstream message;
    message << "q_KILLME_\n";
    WorkItem* item = new WorkItem(message.str(), std::this_thread::get_id(), answer_queue);
    queue_.add(item);                
    workable_ = false;
}

void ListenThread::run()
{
    PCqueue<AnswerItem*> answer_queue;
    int bytes_read, sock = socket_;//*(int*)socket_;
    string error, talk, answ;//, string_message;
    stringstream message;
    const size_t maxMsgLen = 25;
    char chk, client_message[maxMsgLen];
    talk = "Enter command and segment or type quit: \n";
    message << "Your handler id is " << std::this_thread::get_id() << '\n' << talk;
    write(sock, message.str().c_str(), message.str().length());
    while( workable_ )
    {
        if( (bytes_read = recv(sock, client_message, maxMsgLen, 0) > 0) )//recv returns size > 0 if OK
        {
            chk = client_message[0];
            if (chk == 'q')
            {
                killMeSoftly(answer_queue);
            }
            else
            if (chk != 'q' and chk != 'r' and chk != 'm' and chk != 'f')//TODO change to enumeration
            {
                error = "Bad input! Try rm[a,b], fn[a,b], mk[a;b] or q/quit.\n";
                write(sock, error.c_str(), error.length());
                cout<<"Bad input cacthed!\n";
                continue;           
            }
            else
            {
                string string_message(client_message, client_message + bytes_read);
                //HERE I NEED TO KNOW THE LENGHT OF CLIENT MESSAGE
                cout<<"Message was: "<<string_message<<'\n';
                
                //ProcThread deletes used item created here://TODO get rid of pointers at all move items
                WorkItem* item = new WorkItem(string_message, std::this_thread::get_id(), answer_queue);
                queue_.add(item); //send the copy of a pointer to the WorkQueue
                
                const AnswerItem * answer = answer_queue.remove();//wait for answer
                if( answer->getId() != std::this_thread::get_id() )
                {
                    error = "Error: not this listener's answer queue read! Closing connection!\n";
                    write(sock, error.c_str(), error.length());
                    cout<<"Connection: "<< std::this_thread::get_id() <<" read wrong answer queue! Closing...\n";
                    killMeSoftly(answer_queue);
                }
                else
                {
                    answ = answer->getMessage();//answer to the connetion:
                    write(sock, answ.c_str(), answ.length());
                }
                delete answer;
                write(sock, talk.c_str(), talk.length());
            }
        memset(client_message, '\0', maxMsgLen);//clear char *
        }
        else
        {
            error = "Error: server failed to recieve data. Closing...\n";
            write(sock, error.c_str(), error.length());
            cout<<"Connection: "<< std::this_thread::get_id() <<" failed to recieve data. Closing...\n";
            killMeSoftly(answer_queue);
        }
    }
}
