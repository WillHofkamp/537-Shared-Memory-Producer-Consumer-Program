////////////////////////////////////////////////////////////////////////////////
// Main File: queue.c
// This File: queue.c
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
#include "queue.h"

extern const int MAX_LINE_LEN;
extern const int QUEUE_SIZE;

//This method creates a Queue with no of elements = size
Queue *CreateStringQueue(int size) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\n");
        free(queue);
        return NULL;
    }

    queue->size = size; 
    queue->numElem = 0;
    queue->head = 0;  
    queue->tail = 0;
    queue->eqNum = 0;
    queue->dqNum = 0;
    queue->eqBlockNum = 0;
    queue->dqBlockNum = 0;
    queue->string = (char **) malloc(sizeof(char *) * size);
    if (errno == ENOMEM) {
        fprintf(stderr, "Not enough memory for malloc\n");
        free(queue);
        return NULL;
    }

    for (int i = 0; i < size; i++)    
	    queue->string[i] = NULL;

    // if sem_init() returns -1, then there was an error in initializing sem
    if (sem_init(&(queue->stat_block_mutex), 0, 1) == -1 || sem_init(&(queue->mutex), 0, 1) == -1 || 
        sem_init(&(queue->full), 0, 0) == -1 || sem_init(&(queue->empty), 0, size) == -1) {
        fprintf(stderr, "Error in initializing the semaphore\n");
        if (errno == EINVAL) {
            fprintf(stderr, "Semaphore value exceeds SEM_VALUE_MAX\n");
        }
        else if (errno == ENOSYS) {
            fprintf(stderr, "pshared is nonzero, but the system does not support process-shared semaphores\n");
        }
        free(queue);
        return NULL;
    }

    return queue;
}

//This method enqueues a string into the respective Queue
//It waits until there is space for a string to be enqueued
//Once enqueued, it informs that there is a string that can be dequeued
//Enqueue count and enqueue block count are incremented here
void EnqueueString(Queue *q, char *string) 
{
    sem_wait(&(q->stat_block_mutex));
    if (q->size == q->no_of_elements) {
	    q->enqueueBlockCount++;
    }
    sem_post(&(q->stat_block_mutex));

    sem_wait(&(q->empty));
    sem_wait(&(q->mutex));

    q->string[q->tail] = string; 
    q->tail = (q->tail + 1) % q->size;
    q->no_of_elements++;
    q->enqueueCount++;

    sem_post(&q->mutex);
    sem_post(&q->full);
}

//This method dequeues a string into the respective Queue
//It waits until there is a string that can be dequeued
//Once dequeued, it informs that there is space for an enqueue to happen
//Dequeue count and dequeue block count are incremented here
char * DequeueString(Queue *q) 
{
    sem_wait(&(q->stat_block_mutex));
    if (q->no_of_elements==0){
	    q->dequeueBlockCount++;
    }
    sem_post(&(q->stat_block_mutex));

    sem_wait(&q->full);
    sem_wait(&q->mutex);

    char* queue_string = q->string[q->head];
    q->head = (q->head + 1) % q->size; 
    q->no_of_elements--;
    q->dequeueCount++;
     
    sem_post(&q->mutex);
    sem_post(&q->empty);
    return queue_string;
}

//This method prints the queue statistics to stderr
void PrintQueueStats(Queue *q) {
	fprintf(stderr, "1. Enqueue Count: %d\n2. Dequeue Count: %d\n3. Enqueue Block Count: %d\n4. Dequeue Block Count: %d\n", q->enqueueCount - 1, q->dequeueCount - 1, q->enqueueBlockCount, q->dequeueBlockCount);
}
