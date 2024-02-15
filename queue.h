//
// Created by yuvalbs on 2/15/24.
//
#include <stdbool.h>

#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_SIZE 10000

typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
    int index;
} Queue;

// Function declarations
void initializeQueue(Queue *q);
int isFull(Queue *q);
int isEmpty(Queue *q);
bool enqueue(Queue *q, int value, int index);
int dequeue(Queue *q);

#endif /* QUEUE_H */

