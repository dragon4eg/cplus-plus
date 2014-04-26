#TODO add compiler flags and (EXECUTABLE) and (OBJECT) vars
CC=g++
CFLAGS=-std=c++11
LIB=-lpthread
all: server
	
server: main.o Segment.o Daemon.o ListenThread.o Operations.o ProcThread.o ListenManager.o
	$(CC) $(CFLAGS) main.o Segment.o Daemon.o ListenThread.o Operations.o ProcThread.o ListenManager.o $(LIB) -g3 -O0 -o server
	
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Segment.o: Segment.cpp 
	$(CC) $(CFLAGS) -c Segment.cpp 

ListenThread.o: ListenThread.cpp 
	$(CC) $(CFLAGS) -c ListenThread.cpp 

Operations.o: Operations.cpp 
	$(CC) $(CFLAGS) -c Operations.cpp 

ProcThread.o: ProcThread.cpp 
	$(CC) $(CFLAGS) -c ProcThread.cpp 

ListenManager.o: ListenManager.cpp 
	$(CC) $(CFLAGS) -c ListenManager.cpp 

Daemon.o: Daemon.cpp 
	$(CC) $(CFLAGS) -c Daemon.cpp
	
clean:
	rm -rf *.o server
#Filelist:
#7 .cpp files (with main.cpp)
#10 headers:
#Daemon.h
#SegmentGeneric.h 
#PCqueue.h 
#Segment.h 
#Operations.h 
#WorkItem.h 
#AnswerItem.h 
#ProcThread.h 
#ListenThread.h 
#ListenManager.h 
