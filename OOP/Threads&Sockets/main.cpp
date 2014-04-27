#include "Daemon.h"
#include <iostream>

using std::cin;
using std::cout;
//TODO change getting a commant just by first letter of input, 
//make some better way of distinguishing them like an enumeration

int main ( int argc, char *argv[] )
{
    string command, prt; 
    int main_socket;
    if ( argc != 2 ) // argc should be 2 for correct execution
    {
        cout<<"usage: "<< argv[0] <<" <port_number>\n";
        return 0;
    }
    else 
    {
        prt = argv[1]; 
        const size_t port = stoi(prt); 
        if  (port < 0 or port > 65535)
        { 
            cout <<"Bad port [1...65535]\n";
            return 0;
        }
        else
        {
            main_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (main_socket == -1)
            {
                cout<<"Could not create socket\n";
                return 0;
            }
            else
            {
                thread daemon(Daemon(main_socket, port));
                do
                {
                    cout<<"Type 'kill', when to shut down!!!\n";
                    cin >> command;
                }
                while (command != "kill");
                shutdown(main_socket, SHUT_RDWR);//SHUT_RDWR Disables further send and receive operations
                daemon.join();// me in death))
            }
        }   
    }     
    return 0;
}



