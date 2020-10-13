////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: main.c
// This File Description: This is the reader that also sets up the queue struct
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "queue.h"
#include "thread.h"

const int QUEUE_SIZE = 10;

// This is the main method which creates the queues for each 
// function, creates the pthreads for each function, joins 
// them at the end, and prints queue statistics
// 
// @returns N/A
int main(){

	// create each queue
	Queue *readerToMunch1Queue = CreateStringQueue(QUEUE_SIZE);
	if (readerToMunch1Queue == NULL){
		fprintf(stderr, "Error unable to create queue\n");
		return -1;
	}
	Queue *munch1ToMunch2Queue = CreateStringQueue(QUEUE_SIZE);
	if (munch1ToMunch2Queue == NULL){
		fprintf(stderr, "Error unable to create queue\n");
		return -1;
	}
	Queue *munch2ToWriterQueue = CreateStringQueue(QUEUE_SIZE);
	if (munch2ToWriterQueue == NULL){
		fprintf(stderr, "Error unable to create queue\n");
		return -1;
	}
	
	void *queues[3];
	queues[0] = readerToMunch1Queue;
	queues[1] = munch1ToMunch2Queue;
	queues[2] = munch2ToWriterQueue;
	
	// create the threads
	pthread_t readThread;
	pthread_t munch1Thread;
	pthread_t munch2Thread;
	pthread_t writeThread;
	
	// creates the thread for each module
	int createRead = pthread_create(&readThread, NULL, Read, queues);
	if(createRead != 0){
		fprintf(stderr, "Error creating Reader thread.\n");
		return -1;
	}
	
	int createMunch1 = pthread_create(&munch1Thread, NULL, Munch1, queues);
	if(createMunch1 != 0){
		fprintf(stderr, "Error creating Munch1 thread.\n");
		return -1;
	}
	
	int createMunch2 = pthread_create(&munch2Thread, NULL, Munch2, queues);
	if(createMunch2 != 0){
		fprintf(stderr, "Error creating Munch2 thread.\n");
		return -1;
	}
	
	int createWrite = pthread_create(&writeThread, NULL, Write, queues);
	if(createWrite != 0){
		fprintf(stderr, "Error creating Writer thread.\n");
		return -1;
	}
	
	// joins all of the threads
	int joinRead = pthread_join(readThread, NULL);
	if(joinRead != 0) {
		fprintf(stderr, "Error: couldn't join readThread.");
		return -1;	
		}
	
	int joinMunch1 = pthread_join(munch1Thread, NULL);
	if(joinMunch1 != 0) {
		fprintf(stderr, "Error: couldn't join munch1Thread.");
		return -1;	
		}
	
	int joinMunch2 = pthread_join(munch2Thread, NULL);
	if(joinMunch2 != 0) {
		fprintf(stderr, "Error: couldn't join munch2Thread.");
		return -1;	
		}
	
	int joinWrite = pthread_join(writeThread, NULL);
	if(joinWrite != 0) {
		fprintf(stderr, "Error: couldn't join writeThread.");
		return -1;
	}

	// stats of each queue
	fprintf(stderr, "\nQUEUE 1 STATS\n");
	PrintQueueStats(queues[0]);
	fprintf(stderr, "\nQUEUE 2 STATS\n");
	PrintQueueStats(queues[1]);
	fprintf(stderr, "\nQUEUE 3 STATS\n");
	PrintQueueStats(queues[2]);

	// destroy the semaphores
	for(int i = 0; i < QUEUE_SIZE; i++){
		Queue **queue = (Queue **) queues;
		sem_destroy(&queue[i]->eqReady);
		sem_destroy(&queue[i]->dqReady);
		sem_destroy(&queue[i]->mutex);
	}
}
