////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: munch1.c
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
#include <pthread.h>
#include "queue.h"

// This method iterates through the queue passed in from reader,
// indexes the string for spaces,replaces those spaces with 
// asterisks and then returns the updated queue
//
// @param queues
// @returns queues
//
void *munch1Strings(void *queues){
	Queue **queue = (Queue **) queues;
	char *string = NULL;
	char asterisk = '*';
	char space = ' ';
	char *ptr = NULL;
	char* TERMINATE = "EOFNULLchar";
	
	// run the thread until we get the the termination key
	while(1){
		// prevents the thread from busy waiting
		string = DequeueString(queue[0]);
		
		// test if we've reached the end of the queue
		if((strcmp(string, TERMINATE)) == 0){
			break;
		}
		
		// run until no more spaces in string, then break out of this loop
		while (1){
			ptr = strchr(string, space);
			// if ptr is NULL than there are no more spaces in string
			if(ptr == NULL){
				break;
			}else{
				*(ptr) = asterisk;
			}
		}
		// add the munipulated string onto next queue
		EnqueueString(queue[1], string);
	}
	// termination string used to communicate to munch2 thread that munch1 is done
	EnqueueString(queue[1], TERMINATE);
	pthread_exit(NULL);
}
