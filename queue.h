////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: queue.h
// Other Files: main.c / queue.c / reader.c / munch1.c / munch2.c / writer.c / queue.h / reader.h / munch1.h / munch2.h / writer.h / makefile
// Semester:         CS 537 Fall 2018
//
// Author:           Ethan Lengfeld
// Email:            elengfeld@wisc.edu
// CS Login:         lengfeld
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//
//
// Online sources:   None
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef QUEUE
#define QUEUE
#include <semaphore.h>

typedef struct{
	char **strings;
	int capacity;
	int numElements;
	int headIndex;
	int tailIndex;
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
	sem_t enqueueReady;
	sem_t dequeueReady;
	sem_t mutex;
}Queue;	

Queue *CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char * DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif
