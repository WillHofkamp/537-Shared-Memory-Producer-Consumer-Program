////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: queue.c
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
Queue *CreateStringQueue(int size){

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
	stringQueue->capacity = size;
	stringQueue->numElements = 0;
	stringQueue->headIndex = 0;
	stringQueue->tailIndex = 0;
	stringQueue->enqueueCount = 0;
	stringQueue->dequeueCount = 0;
	stringQueue->enqueueBlockCount = 0;
	stringQueue->dequeueBlockCount = 0;
	sem_init(&stringQueue->enqueueReady, 0, 0);
	sem_init(&stringQueue->dequeueReady, 0, 10);
	sem_init(&stringQueue->mutex, 0, 1);
	
	return stringQueue;
}

// enqueue strings onto the string as well as update the statistics
void EnqueueString(Queue *q, char *string){
	if(q->numElements == q->capacity){
		q->enqueueBlockCount++;
		// BLOCK until dequeue is made!
	}
	sem_wait(&q->dequeueReady);
	sem_wait(&q->mutex);
	if(q->tailIndex == q->capacity){
		q->tailIndex = 0;
	}
	q->strings[q->tailIndex] = string;
	q->tailIndex++;
	q->numElements++;
	q->enqueueCount++;
	sem_post(&q->mutex);
	//unlock the blocked dequeue
	sem_post(&q->enqueueReady);
}

// dequeue strings onto the string as well as update the statistics
char * DequeueString(Queue *q){
	if(q->numElements == 0){
		q->dequeueBlockCount++;
		// BLOCK until enqueue is made!
	}
	sem_wait(&q->enqueueReady);
	sem_wait(&q->mutex);
	// wrap the head around
	if(q->headIndex == q->capacity){
		q->headIndex = 0;
	}
	char *string = q->strings[q->headIndex];
	q->headIndex++;
	q->numElements--;
	q->dequeueCount++;
	sem_post(&q->mutex);
	//unlock the blocked enqueue
	sem_post(&q->dequeueReady);
	return string;
}

// print all stats related to the queue
void PrintQueueStats(Queue *q){
	fprintf(stderr, "enqueueCount = %d\n", q->enqueueCount);
	fprintf(stderr, "dequeueCount = %d\n", q->dequeueCount);
	fprintf(stderr, "enqueueBlockCount = %d\n", q->enqueueBlockCount);
	fprintf(stderr, "dequeueBlockCount = %d\n", q->dequeueBlockCount);
}
