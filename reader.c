////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: reader.c
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
#include <pthread.h>
#include "queue.h"

const int BUFFER_SIZE = 1024;

// This method reads in strings from stdin into a queue
// and dynamically allocates the queue necessary for this
// 
// @param queues 
// @return queues 
void *readInStrings(void *queues){
	Queue **queue = (Queue **) queues;
	char c = '\0';
	int mallocNewLineFlag = 1;
	int ignoreLine = 0;
	int charIndex = 0;
	char *inputString = NULL;
	char* TERMINATE = "EOFNULLchar";
	
	// iterates through stdin until file end is reached
	while( (c = getc(stdin)) != EOF){

		// allocates space for new char* for each new line
		if(mallocNewLineFlag){
			inputString = (char *) calloc(BUFFER_SIZE, sizeof(char));
			if(inputString == NULL){
				printf("Error: Unable malloc space for input string.\n");
				return NULL;
			}
			// reset both flags
			mallocNewLineFlag = 0;
			ignoreLine = 0;
		}

		// as long as the char isn't a return (new line), continue
		if(c != '\n'){
			if(!ignoreLine){
				// test that the length so far isn't longer than buffer.
				if(charIndex < BUFFER_SIZE){
					inputString[charIndex] = c;
					charIndex++;
				}else{
					fprintf(stderr, "Error: Input string is larger than buffer.\n");
					// set flag telling program to ignore the rest of this line
					ignoreLine = 1;
				}
			}
		// adds string to queue
		}else{
			charIndex = 0;
			mallocNewLineFlag = 1;
			if(inputString != NULL){
				if(!ignoreLine){
					EnqueueString(queue[0], inputString);
					inputString = NULL;
				}else{
					// this line was ignored to free the space
					free(inputString);
					inputString = NULL;
				}
			}
		}
	}
	// if there was a string that didn't end with a new line but instead an EOF it wasn't added to the queue
	if(inputString != NULL && (!ignoreLine)){
		EnqueueString(queue[0], inputString);
	}

	// termination string used to communicate to munch1 thread that reader is done
	EnqueueString(queue[0], TERMINATE);
	pthread_exit(NULL);
}
