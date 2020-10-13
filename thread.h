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

void *Read(void *queues);

void *Munch1(void *queues);

void *Munch2(void *queues);

void *Write(void *queues);

#endif