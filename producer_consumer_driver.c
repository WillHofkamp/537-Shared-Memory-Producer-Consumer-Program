  
////////////////////////////////////////////////////////////////////////////////
// Main File: producer_consumer_driver.c
// This File: producer_consumer_driver.c
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
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "thread.h"

//The MAX_LINE_LEN stores the buffer size (string length is buffer size - 1)
//QUEUE_SIZE specifies the size of the queue to be created
const int MAX_LINE_LEN = 1024;
const int QUEUE_SIZE = 10;

int main()
{
    pthread_t readerThread, munch1Thread, munch2Thread, writerThread;
    Queue *queue1, *queue2, *queue3;
    //queue1 is the queue between reader and munch1
    queue1 = Create(QUEUE_SIZE);
    //queue2 is the queue between munch1 and munch2
    queue2 = Create(QUEUE_SIZE);
    //queue3 is the queue between munch2 and writer
    queue3 = Create(QUEUE_SIZE);

    int error_num = 0;

    threadDto *munch1Dto = (threadDto *) malloc(sizeof(threadDto)), 
        *munch2Dto = (threadDto *) malloc(sizeof(threadDto));
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\nExiting...\n");
        free(munch1Dto);
        free(munch2Dto);
        return -1;
    }

    munch1Dto->input = queue1;
    munch1Dto->output = queue2;
    munch2Dto->input = queue2;
    munch2Dto->output = queue3;
	
    //Creating the 4 threads here and taking care of any system call errors
    if ((error_num = pthread_create(&readerThread, NULL, reader, (void *) queue1)) != 0 || 
      (error_num = pthread_create(&munch1Thread, NULL, munch1, (void *) munch1Dto)) != 0 ||
      (error_num = pthread_create(&munch2Thread, NULL, munch2, (void *) munch2Dto)) != 0 ||
      (error_num = pthread_create(&writerThread, NULL, writer, (void *) queue3)) != 0) {
        fprintf(stderr, "One or more thread(s) couldn't be created successfully.\n");
        if (error_num == EAGAIN)
            fprintf(stderr, "Insufficient resources to create a thread.\nExiting...\n");
        else if (error_num == EINVAL)
            fprintf(stderr, "Invalid settings in attr.\nExiting...\n");
        else if (error_num == EPERM)
            fprintf(stderr, "No permission to set the scheduling policy and parameters specified in attr.\nExiting...\n");
        free(munch1Dto);
        free(munch2Dto);
        return -1;
    }

    if ((error_num = pthread_join(readerThread, NULL)) != 0 || 
      (error_num = pthread_join(munch1Thread, NULL)) != 0 || 
      (error_num = pthread_join(munch2Thread, NULL)) != 0 || 
      (error_num = pthread_join(writerThread, NULL)) != 0) {
        fprintf(stderr, "One or more thread(s) couldn't be joined successfully.\n");
        if (error_num == EDEADLK)
            fprintf(stderr, "A deadlock was detected.\nExiting...\n");
        else if (error_num == EINVAL)
            fprintf(stderr, "The thread is not a joinable thread.\nExiting...\n");
        else if (error_num == ESRCH)
            fprintf(stderr, "No thread with thread id could be found.\nExiting...\n");
        return -1;
    }

    //Printing the queue statistics to stderr
    fprintf(stderr, "\n\nQueue Statistics:\n");
    fprintf(stderr, "\nQueue 1 (Reader to Munch1):\n");
    Print(queue1);
    fprintf(stderr, "\nQueue 2 (Munch1 to Munch2):\n");
    Print(queue2);
    fprintf(stderr, "\nQueue 3 (Munch2 to Writer):\n");
    Print(queue3);

    return 0;
}
