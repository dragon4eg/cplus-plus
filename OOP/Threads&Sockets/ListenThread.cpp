#include "ListenThread.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>	//write, close socket
#include <sys/socket.h>
using std::cout;
using std::stringstream;
using std::move;
ListenThread::ListenThread(PCqueue< WorkItem > & queue, int socket) : queue_(queue), socket_(socket), workable_(true) {}

void ListenThread::killMeSoftly(PCqueue< AnswerItem > & answer_queue)
{
    //pass 'q' = "KILLME" to the PCqueue so that the ProcThread asked the ListenManager
    //to close this thread because we can't do this from inside it
    cout<<"Connection: "<< std::this_thread::get_id() <<" requested closing.\n";
    stringstream message;
    message << "q_KILLME_\n";
    WorkItem item(message.str(), std::this_thread::get_id(), answer_queue);
    queue_.add(move(item));//WE DON'T NEED std::MOVE since we'll call the only constructor we have and it's not a copy one
    workable_ = false;
}
/*******************************ListenThrea::run()***********************************************/
void ListenThread::run()
{
    PCqueue< AnswerItem > answer_queue;// create my own anser queue
    int bytes_read {};
    stringstream message;
    const size_t maxMsgLen = 25;
    char chk, client_message[maxMsgLen];
    const string talk = "Enter command and segment or type quit: \n";
    message << "Your handler id is " << std::this_thread::get_id() << '\n' << talk;
    write(socket_, message.str().c_str(), message.str().length());
    while( workable_ )
    {
        bytes_read = recv(socket_, client_message, maxMsgLen, 0);//recv returns -1 or 0 if not OK
        if( bytes_read > 0) 
        {
            chk = client_message[0];
            if (chk == 'q')//if quit asked
            {
                killMeSoftly(answer_queue);
            }
            else
            if (chk != 'q' and chk != 'r' and chk != 'm' and chk != 'f')
            {
                const string error = "Bad input! Try rm[a,b], fn[a,b], mk[a;b] or q/quit.\n";
                write(socket_, error.c_str(), error.length());
                cout<<"Bad input cacthed!\n";
                continue;           
            }
            else
            {
                const string string_message(client_message, client_message + bytes_read);      
                WorkItem item(string_message, std::this_thread::get_id(), answer_queue);
                queue_.add(move(item));
                //wait for answer while PCqueue::remove() makes this thread wait
                AnswerItem answer(answer_queue.remove());
                if( answer.getId() != std::this_thread::get_id() )
                {
                    const string error = "Error: not this listener's answer queue read! Closing connection!\n";
                    write(socket_, error.c_str(), error.length());
                    cout<<"Connection: "<< std::this_thread::get_id() <<" read wrong answer queue! Closing...\n";
                    killMeSoftly(answer_queue);
                }
                else
                {
                    const string answ = answer.getMessage();//answer to the connetion:
                    write(socket_, answ.c_str(), answ.length());
                }
                write(socket_, talk.c_str(), talk.length());
            }
        memset(client_message, '\0', maxMsgLen);//clear char *
        }
        else
        {
            const string error = "Error: server failed to recieve data. Closing...\n";
            write(socket_, error.c_str(), error.length());
            cout<<"Connection: "<< std::this_thread::get_id() <<" failed to recieve data. Closing...\n";
            killMeSoftly(answer_queue);
        }
    }
}
