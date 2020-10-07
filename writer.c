////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: writer.c
// This File Description: This is the writer that also sets up the queue struct
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

void *writeLines(void *queues){
	Queue **queue = (Queue **) queues;
	char *line = NULL;
	char* TERMINATE = "EOFNULLchar";
	
	// run the thread until we get the the termination key
	while(1){
		// prevents the thread from busy waiting
		line = Dequeue(queue[2]);
		
		// test if we've reached the end of the queue
		if((strcmp(line, TERMINATE)) == 0){
			break;
		}
		// print the line to standard output
		printf("%s\n", line);
	}
	pthread_exit(NULL);
}
