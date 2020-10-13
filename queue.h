////////////////////////////////////////////////////////////////////////////////
// Main File: queue.h
// This File: queue.h
// This File Description: This is the reader that also sets up the queue struct
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////
#include <semaphore.h>

#ifndef QUEUE_SERVICE
#define QUEUE_SERVICE

typedef struct {
    int size;
    int numElem;
    int head; 
    int tail;
    int enqueueCount;
    int dequeueCount;
    int enqueueTime;
    int dequeueTime;
    int eqBlockNum;
    int dqBlockNum;
    char **string;
    sem_t mutex; 
    sem_t full;
    sem_t empty; 
    sem_t stat_block_mutex;
} Queue;

Queue *CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char * DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif