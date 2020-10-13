////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: main.c
// This File Description: This is the reader that also sets up the queue struct
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"
#include "reader.h"
#include "munch1.h"
#include "munch2.h"
#include "writer.h"

const int QUEUE_SIZE = 10;

// This is the main method which creates the queues for each 
// function, creates the pthreads for each function, joins 
// them at the end, and prints queue statistics
// 
// @returns N/A
int main(){

	// creating the threads
	pthread_t readerThread;
	pthread_t munch1Thread;
	pthread_t munch2Thread;
	pthread_t writerThread;
	

	// creating the queues
	Queue *readerQueue = CreateStringQueue(QUEUE_SIZE);
	if (readerQueue == NULL){
		fprintf(stderr, "Error unable to create queue\n");
		return -1;
	}
	Queue *munch1Queue = CreateStringQueue(QUEUE_SIZE);
	if (munch1Queue == NULL){
		fprintf(stderr, "Error unable to create queue\n");
		return -1;
	}
	Queue *munch2Queue = CreateStringQueue(QUEUE_SIZE);
	if (munch2Queue == NULL){
		fprintf(stderr, "Error unable to create queue\n");
		return -1;
	}
	
	void *queueHolder[3];
	queueHolder[0] = readerQueue;
	queueHolder[1] = munch1Queue;
	queueHolder[2] = munch2Queue;

	// creates the thread for each module
	if(pthread_create(&readerThread, NULL, reader, queueHolder)){
		fprintf(stderr, "Error creating Reader thread.\n");
		return -1;
	}
	
	if(pthread_create(&munch1Thread, NULL, munch1, queueHolder)){
		fprintf(stderr, "Error creating Munch1 thread.\n");
		return -1;
	}
	
	if(pthread_create(&munch2Thread, NULL, munch2, queueHolder)){
		fprintf(stderr, "Error creating Munch2 thread.\n");
		return -1;
	}
	
	if(pthread_create(&writerThread, NULL, writer, queueHolder)){
		fprintf(stderr, "Error creating Writer thread.\n");
		return -1;
	}
	
	// joins all of the threads
	if(pthread_join(readerThread, NULL)) {
		fprintf(stderr, "Error: couldn't join readerThread.");
		return -1;	
		}
	
	if(pthread_join(munch1Thread, NULL)) {
		fprintf(stderr, "Error: couldn't join munch1Thread.");
		return -1;	
		}
	
	if(pthread_join(munch2Thread, NULL)) {
		fprintf(stderr, "Error: couldn't join munch2Thread.");
		return -1;	
		}
	
	if(pthread_join(writerThread, NULL)) {
		fprintf(stderr, "Error: couldn't join writerThread.");
		return -1;
	}

	// TEST QUEUE
	fprintf(stderr, "\nQUEUE 1 STATS\n");
	PrintQueueStats(queueHolder[0]);
	fprintf(stderr, "\nQUEUE 2 STATS\n");
	PrintQueueStats(queueHolder[1]);
	fprintf(stderr, "\nQUEUE 3 STATS\n");
	PrintQueueStats(queueHolder[2]);

	// destroy the semaphores
	for(int i = 0; i < QUEUE_SIZE; i++){
		Queue **queue = (Queue **) queues;
		sem_destroy(&queue[i]->eqReady);
		sem_destroy(&queue[i]->dqReady);
		sem_destroy(&queue[i]->mutex);
	}
}
