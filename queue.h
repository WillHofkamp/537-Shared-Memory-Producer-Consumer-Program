#include <semaphore.h>


//Queue definition
#ifndef STRUCT_STAT_STATM_CMDLINE
#define STRUCT_STAT_STATM_CMDLINE
typedef struct Queue
{
    int head;
    int tail;
    int capacity;
    int numElements;
    int enqueueCount;
    int dequeueCount;
    int enqueueBlockCount;
    int dequeueBlockCount;
    char **string;
    sem_t mutex;
    sem_t full;
    sem_t empty;
    sem_t stat_block_mutex;
} Queue;
#endif

//Function definitions for the Queue methods
Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);


//Function definitions for the Threads
void *reader(void *string);
void *munch1(void *queues);
void *munch2(void *queues);
void *writer(void *queue_data);
