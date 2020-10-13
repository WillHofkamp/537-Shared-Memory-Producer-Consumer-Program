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
#include "queue.h"

// This method creates a queue to the size
// specification passed in, and initializes
// the statistics that will be tracked 
// thoughout the queue's use.
// 
// @param size
// @stringQueue
Queue *CreateStringQueue(int qsize){

	Queue *stringQueue = (Queue *) malloc(sizeof(Queue));
	if(stringQueue == NULL){
		fprintf(stderr, "Error unable to malloc queue\n");
		return NULL;
	}
	stringQueue->strings = (char**) malloc(sizeof(char*) * size);
	if(stringQueue->strings == NULL){
		fprintf(stderr, "Error unable to malloc queue\n");
		free(stringQueue);
		return NULL;
	}
	stringQueue->capacity = qsize;
	stringQueue->numElems = 0;
	stringQueue->head = 0;
	stringQueue->tail = 0;
	stringQueue->enqueueCount = 0;
	stringQueue->dequeueCount = 0;
	stringQueue->enqueueTime = 0;
	stringQueue->dequeueTime = 0;
	sem_init(&stringQueue->eqReady, 0, 0);
	sem_init(&stringQueue->dqReady, 0, 10);
	sem_init(&stringQueue->mutex, 0, 1);
	
	return stringQueue;
}

// enqueue strings onto the string as well as update the statistics
void EnqueueString(Queue *queue, char *string){
	if(queue->numElems == queue->capacity){
		queue->enqueueTime++;
		// BLOCK until dequeue is made!
	}
	sem_wait(&queue->dqReady);
	sem_wait(&queue->mutex);
	if(queue->tail == queue->capacity){
		queue->tail = 0;
	}
	queue->strings[queue->tail] = string;
	queue->tail++;
	queue->numElems++;
	queue->enqueueCount++;
	sem_post(&queue->mutex);
	//unlock the blocked dequeue
	sem_post(&queue->eqReady);
}

// dequeue strings onto the string as well as update the statistics
char * DequeueString(Queue *queue){
	if(queue->numElems == 0){
		queue->dequeueTime++;
		// BLOCK until enqueue is made!
	}
	sem_wait(&queue->eqReady);
	sem_wait(&queue->mutex);
	// wrap the head around
	if(queue->head == queue->capacity){
		queue->head = 0;
	}
	char *string = queue->strings[queue->head];
	queue->head++;
	queue->size--;
	queue->dequeueCount++;
	sem_post(&queue->mutex);
	//unlock the blocked enqueue
	sem_post(&queue->dqReady);
	return string;
}

// print all stats related to the queue
void PrintQueueStats(Queue *queue){
	fprintf(stderr, "enqueueCount = %d\n", queue->enqueueCount);
	fprintf(stderr, "dequeueCount = %d\n", queue->dequeueCount);
	fprintf(stderr, "enqueueTime = %d\n", queue->enqueueTime);
	fprintf(stderr, "dequeueTime = %d\n", queue->dequeueTime);
}
