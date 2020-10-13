////////////////////////////////////////////////////////////////////////////////
// Main File: thread.c
// This File: thread.c
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
#include "thread.h"

extern const int MAX_LINE_LEN;
extern const int QUEUE_SIZE;

//This function returns a line if it is lesser than buffer size
//It takes care of what is returned when the EOF is encountered
parseLine *read_line(int buff_size)
{
    parseLine *lineVal = (parseLine *) malloc(sizeof(parseLine));
    char *line = (char *) malloc(sizeof(char) * buff_size);
    if (errno == ENOMEM) {
        fprintf(stderr, "No enough memory for malloc\n");
        line[0] = '\0';
        lineVal->readString = line;
        lineVal->foundEof = 1;
        lineVal->bufferExceed = 0;
        return lineVal;
    }
    char c = '\0';
    for (int i = 0; i < buff_size; i++) {
        c = getchar();
        if (c != EOF && c != '\n') {
            line[i] = c;
        } else if (c == '\n') {
            line[i] = '\0';
            lineVal->readString = line;
            lineVal->foundEof = 0;
            lineVal->bufferExceed = 0;
            return lineVal;
        } else {
            line[i] = '\0';
            lineVal->readString = line;
            lineVal->foundEof = 1;
            lineVal->bufferExceed = 0;
            return lineVal;
        }
    }
    lineVal->bufferExceed = 1;
    line[buff_size - 1] = '\0';
    lineVal->readString = line;
    lineVal->foundEof = 0;

    // Flush the rest of the line
    while ((c = getchar()) != '\n' && c != EOF);
    if (c == EOF)
        lineVal->foundEof = 1;

    return lineVal;
}

//Reader thread reads a line from stdio and enqueues it into Q1 for Munch1 to access
void *read(void *out_queue)
{
    size_t max_line_len = MAX_LINE_LEN;

    char *store = NULL;
    do {
        parseLine *line_struct = read_line(max_line_len);
        if (line_struct->bufferExceed == 1) {
	    //If line size exceeds buffer size, then line is discarded and message is printed to stderr
            fprintf(stderr, "Line size exceeds the buffer size %zu\n", max_line_len);
            continue;
        }
        if (line_struct->foundEof == 1) {
            if (line_struct->readString[0] != '\0') {
                store = line_struct->readString;
            }
            else {
                break;
            }
        }
        else {
            store = line_struct->readString;
        }
        // printf("Reading at %x: %s\n", line, line);
        EnqueueString((Queue *) out_queue, line);
    } while (line != NULL);

    // EnqueueString NULL as a terminating condition for next queue
    EnqueueString((Queue *) out_queue, NULL);
    pthread_exit(NULL);
    return NULL;
}

//Munch1 DequeueStrings string from Q1, processes it and EnqueueStrings the string to Q2
void *munch1(void *queues)
{
    char *string;
    do {
        string = DequeueString(((threadDto *)queues)->input);
        
        if (string == NULL)
            break;

	//Replace every space with a *
        for (int i=0; string[i] != '\0'; i++) {
            if (string[i] == ' ')
                string[i] = '*';
        }

        EnqueueString(((threadDto *)queues)->output, string);
    } while (string != "EOFNULLchar");

    // EnqueueString NULL as a terminating condition for next queue
    EnqueueString(((threadDto *)queues)->output, NULL);
    pthread_exit(NULL);

    return NULL;
}

//Munch2 DequeueStrings string from Q2, processes it and EnqueueStrings the string to Q3
void *munch2(void *queues)
{
    char *string;
    do {
        string = DequeueString(((threadDto *)queues)->input);
        
        if (string == NULL)
            break;

        // printf("Before Munch2: %s\n", string);

        for (int i=0; string[i] != '\0'; i++) {
            if (islower(string[i]))
                string[i] = toupper(string[i]);
        }

        // printf("After Munch2: %s\n", string);

        EnqueueString(((threadDto *)queues)->output, string);
    } while (string != "EOFNULLchar");

    // EnqueueString NULL as a terminating condition for next queue
    EnqueueString(((threadDto *)queues)->output, NULL);
    pthread_exit(NULL);

    return NULL;
}

void *write(void *in_queue)
{
    char *string = DequeueString((Queue *)in_queue);
    while (string != NULL && strcmp(string, "EOFNULLchar") != 0) {
        fprintf(stdout, "%s\n", string);
        free(string);
        string = DequeueString((Queue *)in_queue);
    }
    fprintf(stdout, "\nTotal number of processed strings: %d\n", ((Queue *)in_queue)->enqueueCount - 1);
    pthread_exit(NULL);
    return NULL;
}
