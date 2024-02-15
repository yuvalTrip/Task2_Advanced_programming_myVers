//
// Created by yuvalbs on 2/15/24.
//
// Define your queue data structure
#include "queue.h"
#include <stdbool.h>
#include <stdio.h>


// Function to initialize the queue
void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->index = 0;

}

// Function to check if queue is full
int isFull(Queue *q) {
    return (q->size == MAX_QUEUE_SIZE);
}

// Function to check if queue is empty
int isEmpty(Queue *q) {
    //printf("q->size: %d\n",q->size);
    return (q->size == 0);
}

// Function to enqueue an element
bool enqueue(Queue *q, int value,int index) {


    if (!isFull(q)) {
        //printf("im in enque\n");

        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        q->data[q->rear] = value;
        q->size++;
        q->index=index;

        //printf("second q->size: %d",q->size);

        return true;//if we succeed to push
    }
    return false;//if we do not succeed to push i.e the queue is full
}

// Function to dequeue an element
int dequeue(Queue *q) {
    //printf("before dequeue!!");

    if (!isEmpty(q)) {
        //printf("dequeue!!");

        int value = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        q->size--;
        return value;
    } else {
        return -1; // Assuming -1 represents an empty queue or invalid data
    }
}
