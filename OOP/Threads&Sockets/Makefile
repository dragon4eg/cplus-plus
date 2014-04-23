# TODO add compiler flags and (EXECUTABLE) and (OBJECT) vars
CC=g++
CFLAGS=-std=c++11
LIB=-lpthread
all: server
	
server: main.o Segment.o ListenThread.o Operations.o ProcThread.o ListenManager.o
	$(CC) $(CFLAGS) main.o Segment.o ListenThread.o Operations.o ProcThread.o ListenManager.o $(LIB) -g3 -O0 -o server
	
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

#WorkItem.o: WorkItem.cpp 
#	$(CC) $(CFLAGS) -c WorkItem.cpp
	
#Point.o: Point.cpp 
# 	$(CC) $(CFLAGS) -c Point.cpp 

#Thread.o: Thread.cpp
#	$(CC) $(CFLAGS) -c Thread.cpp

clean:
	rm -rf *.o server

#7 cpp files (without main.cpp)
#_________
#9 headers
#__________
#SegmentGeneric.h 
#PCqueue.h 
#__________
#Point.h 
#Segment.h 
#Operations.h 
#WorkItem.h 
#AnswerItem.h 
#ProcThread.h 
#ListenThread.h 
