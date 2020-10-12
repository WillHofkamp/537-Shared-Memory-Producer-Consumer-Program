////////////////////////////////////////////////////////////////////////////////
// Main File: thread.h
// This File: thread.h
// This File Description: This is the reader that also sets up the queue struct
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////
#include "queue.h"

//This structure is used to pass the Queues as arguments to the threads
#ifndef THREAD_SERVICE
#define THREAD_SERVICE 
typedef struct threadDto
{
    Queue *input;
    Queue *output;
} threadDto;

//This structure is used for returning a line from stdio
//It also checks whether eof has been encountered or
//if the line has exceeded the buffer size.
typedef struct {
    char *read_str;
    int has_eof;
    int buff_size_exceeding;
} read_line_val;

void *Read(void *out_queue);

void *Munch1(void *queues);

void *Munch2(void *queues);

void *Write(void *in_queue);

#endif