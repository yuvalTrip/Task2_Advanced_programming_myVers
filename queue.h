//
// Created by yuvalbs on 2/15/24.
//

#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include <pthread.h>
#define MAX_QUEUE_SIZE 10000000

typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
    int index;
    pthread_mutex_t lock; // Mutex lock for thread safety
} Queue;

// Function declarations
void initializeQueue(Queue *q);
int isFull(Queue *q);
int isEmpty(Queue *q);
bool enqueue(Queue *q, int value);
int enqueueMany(Queue *q, int *values, int count);
int dequeue(Queue *q);
int dequeueMany(Queue *q, int *values, int count);
void printQueue(Queue *q);

#endif /* QUEUE_H */

