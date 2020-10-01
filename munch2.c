////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: munch2.c
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
#include <ctype.h>
#include <pthread.h>
#include "queue.h"

// This method receives the queue from munch1, iterates through the queue
// to find lower case characters, and if that's the case it changes them
// to uppercase. Then it returns the updated queue
//
// @param queues
// @return queues
//
void *munch2Strings(void *queues){
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
