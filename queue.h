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
typedef struct Queue
{
    int head, tail, size, numElem, eqCount, dqCount, eqBlockNum, dqBlockNum;
    char **string;
    sem_t mutex, full, empty, stat_block_mutex;
} Queue;

Queue *Create(int size);

void Enqueue(Queue *q, char *string);

char * Dequeue(Queue *q);

void Print(Queue *q);

#endif