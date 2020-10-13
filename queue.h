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
	int maxSize;
	int size;
	int head;
	int tail;
	int enqueues;
	int dequeues;
	int eqBlocks;
	int dqBlocks;
	sem_t eqReady;
	sem_t dqReady;
	sem_t mutex;
}Queue;	

Queue *MakeQueue(int qsize);

void Enqueue(Queue *q, char *string);

char * Dequeue(Queue *q);

void PrintStats(Queue *q);

#endif
