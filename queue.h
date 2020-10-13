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

typedef struct{
	char **strings;
	int numElems;
	int capacity;
	int head;
	int tail;
	int enqueueCount;
	int dequeueCount;
	int enqueueTime;
	int dequeueTime;
	sem_t eqReady;
	sem_t dqReady;
	sem_t mutex;
}Queue;	

Queue *CreateStringQueue(int qsize);

void EnqueueString(Queue *q, char *string);

char * DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif
