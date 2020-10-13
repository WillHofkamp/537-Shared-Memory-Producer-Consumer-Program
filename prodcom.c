////////////////////////////////////////////////////////////////////////////////
// Main File: prodcom.c
// This File: prodcom.c
// This File Description: This is the main file that creates the queues and the 
//                        pthreads and sends them through the queues
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

//This is the main method that creates the queues for each thread
//and then joins them all together. Once the process is done with the write
//thread, it is 
int main(){

    // create threads
	pthread_t readThread;
	pthread_t munch1Thread;
	pthread_t munch2Thread;
	pthread_t writeThread;

	// create queues and create an array of them all
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

	//create threads
	int createRead = pthread_create(&readThread, NULL, Read, queues); 
	if(createRead != 0){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	int createMunch1 = pthread_create(&munch1Thread, NULL, Munch1, queues); 
	if(createMunch1 != 0){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	int createMunch2 = pthread_create(&munch2Thread, NULL, Munch2, queues); 
	if(createMunch2 != 0){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	int createWrite = pthread_create(&writeThread, NULL, Write, queues); 
	if(createWrite != 0){
		fprintf(stderr, "Error: Couldn't create thread\n");
		return -1;
	}
	
	// joining the threads
	int joinRead = pthread_join(readThread, NULL); 
	if(joinRead != 0) {
		fprintf(stderr, "Error: Couldn't join readThread.");
		return -1;	
	}
	
	int joinMunch1 = pthread_join(munch1Thread, NULL); 
	if(joinMunch1 != 0) {
		fprintf(stderr, "Error: Couldn't join munch1Thread.");
		return -1;	
	}
	
	int joinMunch2 = pthread_join(munch2Thread, NULL); 
	if(joinMunch2 != 0) {
		fprintf(stderr, "Error: Couldn't join munch2Thread.");
		return -1;	
	}
	
	int joinWrite = pthread_join(writeThread, NULL); 
	if(joinWrite != 0) {
		fprintf(stderr, "Error: Couldn't join writerThread.");
		return -1;
	}

	// Printing stats
	fprintf(stderr, "\nQueue 1(Reader to Munch1) Stats\n");
	PrintQueueStats(queues[0]);
	fprintf(stderr, "\nQueue 2(Munch1 to Munch2) Stats\n");
	PrintQueueStats(queues[1]);
	fprintf(stderr, "\nQueue 3(Munch2 to Writer) Stats\n");
	PrintQueueStats(queues[2]);
}
