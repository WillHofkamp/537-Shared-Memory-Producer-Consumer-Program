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
Queue *MakeQueue(int qsize){

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
	stringQueue->maxSize = qsize;
	stringQueue->size = 0;
	stringQueue->head = 0;
	stringQueue->tail = 0;
	stringQueue->enqueues = 0;
	stringQueue->dequeues = 0;
	stringQueue->eqBlocks = 0;
	stringQueue->dqBlocks = 0;
	sem_init(&stringQueue->eqReady, 0, 0);
	sem_init(&stringQueue->dqReady, 0, 10);
	sem_init(&stringQueue->mutex, 0, 1);
	
	return stringQueue;
}

// enqueue strings onto the string as well as update the statistics
void Enqueue(Queue *queue, char *string){
	if(queue->size == queue->maxSize){
		queue->eqBlocks++;
		// BLOCK until dequeue is made!
	}
	sem_wait(&queue->dqReady);
	sem_wait(&queue->mutex);
	if(queue->tail == queue->maxSize){
		queue->tail = 0;
	}
	queue->strings[queue->tail] = string;
	queue->tail++;
	queue->size++;
	queue->enqueues++;
	sem_post(&queue->mutex);
	//unlock the blocked dequeue
	sem_post(&queue->eqReady);
}

// dequeue strings onto the string as well as update the statistics
char * Dequeue(Queue *queue){
	if(queue->size == 0){
		queue->dqBlocks++;
		// BLOCK until enqueue is made!
	}
	sem_wait(&queue->eqReady);
	sem_wait(&queue->mutex);
	// wrap the head around
	if(queue->head == queue->maxSize){
		queue->head = 0;
	}
	char *string = queue->strings[queue->head];
	queue->head++;
	queue->size--;
	queue->dequeues++;
	sem_post(&queue->mutex);
	//unlock the blocked enqueue
	sem_post(&queue->dqReady);
	return string;
}

// print all stats related to the queue
void PrintStats(Queue *queue){
	fprintf(stderr, "enqueues = %d\n", queue->enqueues);
	fprintf(stderr, "dequeues = %d\n", queue->dequeues);
	fprintf(stderr, "eqBlocks = %d\n", queue->eqBlocks);
	fprintf(stderr, "dqBlocks = %d\n", queue->dqBlocks);
}
