////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: queue.c
// This File Description: This is the queue
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include "queue.h"

// Class to create a Queue of the given size
// @param size
// @stringQueue
Queue *CreateStringQueue(int qsize){

	Queue *queue = (Queue *) malloc(sizeof(Queue));
	if(queue == NULL){
		fprintf(stderr, "Error unable to malloc queue\n");
		return NULL;
	}

	queue->strings = (char**) malloc(sizeof(char*) * size);
	if(queue->strings == NULL){
		fprintf(stderr, "Error unable to malloc queue\n");
		free(queue);
		return NULL;
	}
	queue->capacity = qsize;
	queue->numElems = 0;
	queue->head = 0;
	queue->tail = 0;
	queue->enqueueCount = 0;
	queue->dequeueCount = 0;
	queue->enqueueTime = 0;
	queue->dequeueTime = 0;
	sem_init(&queue->eqReady, 0, 0);
	sem_init(&queue->dqReady, 0, 10);
	sem_init(&queue->mutex, 0, 1);
	
	return queue;
}

// Dequeue a string into the respective queue
void EnqueueString(Queue *q, char *string){
	clock_t timeStart;
	clock_t timeEnd;
	clock_t timeTaken;
	timeStart = clock();
    
    if(q->numElems == q->capacity){
		q->enqueueBlockCount++;
	}
	sem_wait(&q->dqReady);
	sem_wait(&q->mutex);
	if(q->tail == q->capacity){
		q->tail = 0;
	}
	q->strings[q->tail] = string;
	q->tail++;
	q->numElems++;
	q->enqueueCount++;
	sem_post(&q->mutex);
	//unlock the blocked dequeue
	sem_post(&q->eqReady);

    timeEnd = clock();
	timeTaken = timeEnd-timeStart;
	q->dequeueTime = (double)(timeTaken)/CLOCKS_PER_SEC
}

// Dequeue a string into the respective queue
char * DequeueString(Queue *q){
	clock_t timeStart;
	clock_t timeEnd;
	clock_t timeTaken;
	timeStart = clock();

    if(q->numElems == 0){
		q->dequeueTime++;
		// BLOCK until enqueue is made!
	}
	sem_wait(&q->eqReady);
	sem_wait(&q->mutex);
	
    // wrap the head around
	if(q->head == q->capacity){
		q->head = 0;
	}
	char *string = q->strings[q->head];
	q->head++;
	q->size--;
	q->dequeueCount++;
	sem_post(&q->mutex);
	//unlock the blocked enqueue
	sem_post(&q->dqReady);
    
    timeEnd = clock();
	timeTaken = timeEnd-timeStart;
	q->dequeueTime = (double)(timeTaken)/CLOCKS_PER_SEC
	return string;
}

// Printing queue stats
void PrintQueueStats(Queue *q){
	fprintf(stderr, "enqueueCount = %d\n", q->enqueueCount);
	fprintf(stderr, "dequeueCount = %d\n", q->dequeueCount);
	fprintf(stderr, "enqueueTime = %d\n", q->enqueueTime);
	fprintf(stderr, "dequeueTime = %d\n", q->dequeueTime);
}
