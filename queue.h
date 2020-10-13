////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: queue.h
// This File Description: This is the queue interface that also sets up the queue struct
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////
#ifndef QUEUE
#define QUEUE
#include <semaphore.h>

//Defining the queue elements
typedef struct{
	char **strings;
	int numElems, capacity, head, tail;
	int enqueueCount, dequeueCount;
	int enqueueTime, dequeueTime;
	sem_t eqReady, dqReady, mutex;
}Queue;	

// Defining the functions for creating a queue and adding and removing data from it.
Queue *CreateStringQueue(int qsize);

void EnqueueString(Queue *q, char *string);

char * DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif
