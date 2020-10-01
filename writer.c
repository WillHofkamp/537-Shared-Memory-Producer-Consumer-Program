////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: writer.c
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
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

void *writeOutStrings(void *queues){
	Queue **queue = (Queue **) queues;
	char *string = NULL;
	char* TERMINATE = "EOFNULLchar";
	
	// run the thread until we get the the termination key
	while(1){
		// prevents the thread from busy waiting
		string = DequeueString(queue[2]);
		
		// test if we've reached the end of the queue
		if((strcmp(string, TERMINATE)) == 0){
			break;
		}
		// print the string to standard output
		printf("%s\n", string);
	}
	pthread_exit(NULL);
}
