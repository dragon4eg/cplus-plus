#include "ListenThread.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>	//write, close socket
#include <sys/socket.h>
using std::cout;
using std::stringstream;

void ListenThread::killMeSoftly(PCqueue< AnswerItem > & answer_queue, const std::thread::id myid)
{
    //pass 'q' = "KILLME" to the PCqueue so that the ProcThread asked the ListenManager
    //to close this thread because we can't do this from inside it
    running_ = false;
    cout<<"Connection: "<< myid <<" is closing.\n";
    stringstream message;
    message << "quit\n";
    WorkItem item(message.str(), myid, answer_queue);
    queue_.add(item);
}

void ListenThread::run()
{
    PCqueue< AnswerItem > answer_queue;// create my own anser queue
    int bytes_read {};
    stringstream message;
    const size_t maxMsgLen = 25;
    char chk, client_message[maxMsgLen];
    const string talk = "Enter command and segment or type quit: \n";
    const thread::id myid = std::this_thread::get_id();
    message << "Your handler id is " << myid << '\n' << talk;
    write(socket_, message.str().c_str(), message.str().length());
    //auto stop_ptr = pool_.find(myid);
    while( running_ /*and !(stop_ptr->second.stop_)*/ )//stopptr = false, if true stop!
    {
        bytes_read = recv(socket_, client_message, maxMsgLen, 0);//recv returns -1 or 0 if not OK
        if( bytes_read > 0) 
        {
            chk = client_message[0];
            if (chk == 'q')//if quit asked
            {
                killMeSoftly(answer_queue, myid);
            }
            else
            {
                if (chk != 'q' and chk != 'r' and chk != 'm' and chk != 'f')
                {
                    const string error = "Bad input! Try rm[a;b], fn[a;b], mk[a;b] or q/quit.\n";
                    write(socket_, error.c_str(), error.length());
                    cout<<"Bad input cacthed!\n";
                    continue;           
                }
                else
                {
                    const string string_message(client_message, client_message + bytes_read);      
                    WorkItem item(string_message, myid, answer_queue);
                    queue_.add(item);
                    //wait for answer while PCqueue::remove() makes this thread wait
                    AnswerItem answer(answer_queue.remove());
                    if( answer.getId() != myid )
                    {
                        const string error = "Error: not this listener's answer queue read! Closing connection!\n";
                        write(socket_, error.c_str(), error.length());
                        cout<<"Connection: "<< myid <<" read wrong answer queue! Closing...\n";
                        killMeSoftly(answer_queue, myid);
                    }
                    else
                    {
                        const string answ = answer.getMessage();//answer to the connetion:
                        write(socket_, answ.c_str(), answ.length());
                    }
                    write(socket_, talk.c_str(), talk.length());
                }
            }
            memset(client_message, '\0', maxMsgLen);//clear char *
        }
        else//we are here when the whole server is shutting down and ask Manager to kill us
        {
            const string down = "Server is being shutting down...\n";
            write(socket_, down.c_str(), down.length());
            cout<<"Socket: "<<socket_<<" was shut down!";
            running_ = false;
        }
        //TODO find out which error i'm forcing, and handle other real ERRNOs of 
        //{
        //    const string error = "Error: server failed to recieve data. Closing...\n";
        //    write(socket_, error.c_str(), error.length());
        //    cout<<"Connection: "<< myid <<" failed to recieve data. Closing...\n";
        //    killMeSoftly(answer_queue, myid);
        //}
    }
}
