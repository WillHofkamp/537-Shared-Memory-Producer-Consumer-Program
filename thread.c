////////////////////////////////////////////////////////////////////////////////
// Main File: thread.c
// This File: thread.c
// This File Description: This is thread implementation that has all the methods
//						  for the different types of threads in this project.
//						  The read method reads in lines, munch1 converts spaces
//						  to stars, munch2 converts all letters to uppercase, and
//						  write writes all the lines to the output file
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

const int BUFFER = 1024;

//reads a line from the file input and enqueues it into Q1
void *Read(void *queues)
{
    Queue **queue = (Queue **) queues;
	char c = '\0';
	int nextLine = 1;
	int skip = 0;
	int charIndex = 0;
	char *inputString = NULL;
	
	//while the file end is not reached yet
	while( (c = getc(stdin)) != EOF){

		//allocate space for each line
		if(nextLine){
			stringLine = (char *) malloc(BUFFER, sizeof(char));
			if(stringLine == NULL){
				printf("Error: Unable to malloc space for input string.\n");
				return NULL;
			}
			//reset flags after line so each line is checked
			nextLine = 0;
			skip = 0;
		}

		if(c != '\n'){
			if(!skip){
				//if the current length is smaller than buffer
				if(charIndex < BUFFER){
					stringLine[charIndex] = c;
					charIndex++;
				}else{
					fprintf(stderr, "Error: Line size exceeds size of the buffer.\n");
					//since the line is too big, ignore the rest of it
					skip = 1;
				}
			}
		}else{
			//add string to queue if char is newline
			charIndex = 0;
			nextLine = 1;
			if(stringLine != NULL){
				if(!skip){
					EnqueueString(queue[0], stringLine);
					stringLine = NULL;
				}else{
					//if the line was ignored, free it instead of enqueuing it
					free(stringLine);
					stringLine = NULL;
				}
			}
		}
	}
	//check if string wasn't added to queue that has EOF instead of \n for the last char
	if(stringLine != NULL && (!skip)){
		EnqueueString(queue[0], stringLine);
	}

	//termination string signals that Read is done
	EnqueueString(queue[0], NULL);
	pthread_exit(NULL);
}

//dequeues the strings from Q1, processes them, and then enqueues the strings to be used in Q2
void *Munch1(void *queues)
{
    Queue **queue = (Queue **) queues;
	char *inputString = NULL;

	// run the thread until the end of the file is reached
	while(1){
		//prevent busy waiting
		inputString = DequeueString(queue[0]);
		
		if((strcmp(inputString, NULL)) == 0){
			break;
		}

		//iterate over each char of string
		for(int i = 0; inputString[i] != '\0'; i++){
			if(inputString[i] == ' '){
				inputString[i] = '*';
			}
		}
		//enqueue to the next queue
		EnqueueString(queue[1], inputString);
	}
	//termination string signals that Munch1 is done
	EnqueueString(queue[1], NULL);
	pthread_exit(NULL);
}

//dequeues strings from Q2, processes them and enqueues them to be used in Q3
void *Munch2(void *queues)
{
    Queue **queue = (Queue **) queues;
	char *inputString = NULL;
	
	// run the thread until the end of the file is reached
	while(1){
		//prevent waiting
		inputString = DequeueString(queue[1]);
		
		if((strcmp(inputString, NULL)) == 0){
			break;
		}

		for(int i = 0; inputString[i] != '\0'; i++){
			if(islower(inputString[i])){
				string[i] = toupper(string[i]);
			}
		}
		//enqueue to the next queue
		EnqueueString(queue[2], inputString);
	}
	//termination string signals that Munch2 is done
	EnqueueString(queue[2], NULL);
	pthread_exit(NULL);
}

//dequeues the strings from Munch2 and then outputs them to the output file
void *Write(void *queues)
{
    Queue **queue = (Queue **) queues;
	char *line = NULL;
	
	// run the thread until the entire Munch2 queue is dequeued
	while(1){
		line = DequeueString(queue[2]);
		
		//test if we've reached the end of the queue
		if((strcmp(line, NULL)) == 0){
			break;
		}
		// print line in output file
		printf("%s\n", line);
	}
	pthread_exit(NULL);
}
