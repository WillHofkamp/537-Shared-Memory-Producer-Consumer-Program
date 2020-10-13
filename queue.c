////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: queue.c
// This File Description: This is the queue implementation which can create, as
//						  well as enqueue and dequeue to, a queue whose stats 
//						  can be printed out
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include "queue.h"


//creates a queue with the max capacity based off of the passed in qsize
Queue *CreateStringQueue(int qsize){

	//malloc a new queue
	Queue *queue = (Queue *) malloc(sizeof(Queue));
	if(queue == NULL){
		fprintf(stderr, "Error unable to malloc queue\n");
		return NULL;
	}

	//malloc enough space for enough strings to be added to queue based
	//on the qsize parameter
	queue->strings = (char**) malloc(sizeof(char*) * qsize);
	if(queue->strings == NULL){
		fprintf(stderr, "Error unable to malloc queue\n");
		free(queue);
		return NULL;
	}
	
	//initialize the rest of the queue to 0
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

// Enqueues the passed in string to the passed in queue
void EnqueueString(Queue *q, char *string){
	clock_t t;
	t = clock();

	//block the dequeue
	sem_wait(&q->dqReady);
	sem_wait(&q->mutex);
	// wrap the queue around if the queue max size is reached
	if(q->tail == q->capacity){
		q->tail = 0;
	}
	//add element to queue and update tail, update stats
	q->strings[q->tail] = string;
	q->tail++;
	q->numElems++;
	q->enqueueCount++;

	sem_post(&q->mutex);
	//unlock the blocked dequeue
	sem_post(&q->eqReady);

	//calculate time spent on enqueue
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	q->enqueueTime += time_taken;
}

// Dequeues the top string in the passed in queue
char * DequeueString(Queue *q){
	clock_t t;
	t = clock();

	//blocked until eq is made
	sem_wait(&q->eqReady);
	sem_wait(&q->mutex);
	
    // wrap the queue around if the queue max size is reached
	if(q->head == q->capacity){
		q->head = 0;
	}
	//remove the element from the queue and change the tail, update stats
	char *string = q->strings[q->head];
	q->head++;
	q->numElems--;
	q->dequeueCount++;

	sem_post(&q->mutex);
	//unblock the enqueue
	sem_post(&q->dqReady);
    
	//calculate time spent on dequeue
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	q->dequeueTime += time_taken;
	return string;
}

// Print the passed in queue stats
void PrintQueueStats(Queue *q){
	fprintf(stderr, "enqueueCount = %d\n", q->enqueueCount);
	fprintf(stderr, "dequeueCount = %d\n", q->dequeueCount);
	fprintf(stderr, "enqueueTime = %f\n", q->enqueueTime);
	fprintf(stderr, "dequeueTime = %f\n", q->dequeueTime);
}
