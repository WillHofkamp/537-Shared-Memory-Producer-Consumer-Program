////////////////////////////////////////////////////////////////////////////////
// Main File: thread.c
// This File: thread.c
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
#include "thread.h"

extern const int MAX_LINE_LEN;
extern const int QUEUE_SIZE;

const int BUFFER_SIZE = 1024;

//Reader thread reads a line from stdio and enqueues it into Q1 for Munch1 to access
void *Read(void *out_queue)
{
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

//Munch1 DequeueStrings string from Q1, processes it and EnqueueStrings the string to Q2
void *Munch1(void *queues)
{
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

//Munch2 DequeueStrings string from Q2, processes it and EnqueueStrings the string to Q3
void *Munch2(void *queues)
{
    Queue **queue = (Queue **) queues;
	char *string = NULL;
	int charIndex = 0;
	char* TERMINATE = "EOFNULLchar";
	
	// run the thread until we get the the termination key
	while(1){
		// prevents the thread from busy waiting
		string = DequeueString(queue[1]);
		
		// test if we've reached the end of the queue
		if((strcmp(string, TERMINATE)) == 0){
			break;
		}

		charIndex = 0;
		// loop through the entire string and change the lower case chars to UPPER
		while(string[charIndex] != '\0'){
			if(islower(string[charIndex])) {
				string[charIndex] = toupper(string[charIndex]);
			}
			charIndex++;
		}
		EnqueueString(queue[2], string);
	}
	// termination string used to communicate to writer thread that munch2 is done
	EnqueueString(queue[2], TERMINATE);
	pthread_exit(NULL);
}

void *Write(void *in_queue)
{
    Queue **queue = (Queue **) queues;
	char *line = NULL;
	char* TERMINATE = "EOFNULLchar";
	
	// run the thread until we get the the termination key
	while(1){
		// prevents the thread from busy waiting
		line = DequeueString(queue[2]);
		
		// test if we've reached the end of the queue
		if((strcmp(line, TERMINATE)) == 0){
			break;
		}
		// print the line to standard output
		printf("%s\n", line);
	}
	pthread_exit(NULL);
}
