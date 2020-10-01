////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: main.c
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
	pthread_t readerThread;
	pthread_t munch1Thread;
	pthread_t munch2Thread;
	pthread_t writerThread;
	
	// creates the thread for each module
	if(pthread_create(&readerThread, NULL, readInStrings, queues)){
		fprintf(stderr, "Error creating Reader thread.\n");
		return -1;
	}
	
	if(pthread_create(&munch1Thread, NULL, munch1Strings, queues)){
		fprintf(stderr, "Error creating Munch1 thread.\n");
		return -1;
	}
	
	if(pthread_create(&munch2Thread, NULL, munch2Strings, queues)){
		fprintf(stderr, "Error creating Munch2 thread.\n");
		return -1;
	}
	
	if(pthread_create(&writerThread, NULL, writeOutStrings, queues)){
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
	PrintQueueStats(queues[0]);
	fprintf(stderr, "\nQUEUE 2 STATS\n");
	PrintQueueStats(queues[1]);
	fprintf(stderr, "\nQUEUE 3 STATS\n");
	PrintQueueStats(queues[2]);

	// destroy the semaphores
	for(int i = 0; i < QUEUE_SIZE; i++){
		Queue **queue = (Queue **) queues;
		sem_destroy(&queue[i]->enqueueReady);
		sem_destroy(&queue[i]->dequeueReady);
		sem_destroy(&queue[i]->mutex);
	}
}
