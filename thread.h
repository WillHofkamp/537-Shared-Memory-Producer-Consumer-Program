////////////////////////////////////////////////////////////////////////////////
// Main File: thread.h
// This File: thread.h
// This File Description: This is the thread interface which defines the methods
//                        for each type of thread that is used
// Author:           William Hofkamp, Pranet Gowni
// Email:            hofkamp@wisc.edu, gowni@wisc.edu
// CS Login:         hofkamp, pranet
////////////////////////////////////////////////////////////////////////////////
#include "queue.h"

#ifndef THREAD_SERVICE
#define THREAD_SERVICE 

void *read(void *out_queue);

void *munch1(void *queues);

void *munch2(void *queues);

void *write(void *in_queue);

#endif
