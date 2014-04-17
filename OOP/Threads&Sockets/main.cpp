//#include <stdio.h>
//#include <stdlib.h>
#include "Segment.h"
//using namespace std;


int main ()
{
	//Segment tmps;
	SegmentSet db;
	wqueue<WorkItem*>  queue;
	ProcThread* processor = new ProcThread(queue, db);
	processor->start();
/*******************************Socket************************************************/	
	int socket_desc, new_socket, c, *new_sock;
	sockaddr_in server, client;
	string message;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		cout<<"Could not create socket\n";
	}
	
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons( 9898 );//port
	if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cout<<"Bind failed\n";
	}
	cout<<"Bind done\n";
	listen(socket_desc , 3); //3 is The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow
	cout<<"Waiting for incoming connections...\n";
	c = sizeof(struct sockaddr_in);
	message = "Hello Client, I have received your connection.\n";	
	while( (new_socket = accept(socket_desc, (sockaddr *)&client, (socklen_t*)&c)) )
	{
		string client_ip, info;
		int client_port;
		cout<<"connection accepted\n";
		//find out clients ip and port
		client_ip = inet_ntoa(client.sin_addr);
		client_port = ntohs(client.sin_port);
		info = message+ "Your ip is: " + client_ip + " port: " + to_string(client_port) + '\n';
		write(new_socket, info.c_str(), info.length());
		cout<<"Clients ip is: "<<client_ip<<" port: "<<client_port<<'\n';
		new_sock = new int(new_socket);
		//new_sock = malloc(1);
		//*new_sock = new_socket;
		ListenThread* listener = new ListenThread(&queue, new_sock);//here in constructor I 
		//have a question as I give wqueue<WorkItem*>& both to ProcessorThread and 
		//Listener thread, can both of rhem own one object or should I pass a pointer to 
		//a Listener thread (I've tested this, it probebly doesnt matter)
		if (listener->start()< 0 /*returns 0 if OK*/)
		{
			cout<<"could not create listener thread!!!\n";
			return 1;
		}
		
	}
	if (new_socket<0)
	{
		cerr<<"accept failed\n";
	}
/*************************************************************************************/
	db.clear();
	return 0;
}
