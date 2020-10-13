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
#include "thread.h"

const int QUEUE_SIZE = 10;

// Method to create the queues and pthreads to be used
// by the other functions
// @returns N/A
int main(){

    // create the threads
	pthread_t readerThread;
	pthread_t munch1Thread;
	pthread_t munch2Thread;
	pthread_t writerThread;

	// create each queue
	Queue *readerQueue = CreateStringQueue(QUEUE_SIZE);
	if (readerQueue == NULL){
		fprintf(stderr, "Error: Couldn't create queue\n");
		return -1;
	}
	Queue *munch1Queue = CreateStringQueue(QUEUE_SIZE);
	if (munch1Queue == NULL){
		fprintf(stderr, "Error: Couldn't create queue\n");
		return -1;
	}
	Queue *munch2Queue = CreateStringQueue(QUEUE_SIZE);
	if (munch2Queue == NULL){
		fprintf(stderr, "Error: Couldn't create queue\n");
		return -1;
	}
	
	void *queues[3];
	queues[0] = readerQueue;
	queues[1] = munch1Queue;
	queues[2] = munch2Queue;

	if(pthread_create(&readerThread, NULL, read, queues)){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	if(pthread_create(&munch1Thread, NULL, munch1, queues)){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	if(pthread_create(&munch2Thread, NULL, munch2, queues)){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	if(pthread_create(&writerThread, NULL, write, queues)){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	// joining the threads
	if(pthread_join(readerThread, NULL)) {
		fprintf(stderr, "Error: Couldn't join readerThread.");
		return -1;	
		}
	
	if(pthread_join(munch1Thread, NULL)) {
		fprintf(stderr, "Error: Couldn't join munch1Thread.");
		return -1;	
		}
	
	if(pthread_join(munch2Thread, NULL)) {
		fprintf(stderr, "Error: Couldn't join munch2Thread.");
		return -1;	
		}
	
	if(pthread_join(writerThread, NULL)) {
		fprintf(stderr, "Error: Couldn't join writerThread.");
		return -1;
	}

	// Printing queue statistics
	fprintf(stderr, "\nQueue 1(Reader to Munch1) Stats\n");
	PrintQueueStats(queues[0]);
	fprintf(stderr, "\nQueue 2(Munch1 to Munch2) Stats\n");
	PrintQueueStats(queues[1]);
	fprintf(stderr, "\nQueue 3(Munch2 to Writer) Stats\n");
	PrintQueueStats(queues[2]);
}
