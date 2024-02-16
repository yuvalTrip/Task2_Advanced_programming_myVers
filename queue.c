#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h> // Include pthread library for mutex

// Function to print the contents of the queue
void printQueue(Queue *q) {
    pthread_mutex_lock(&(q->lock)); // Acquire lock before accessing the queue
    //printf("Queue contents: ");
    int i = q->front;
    int count = 0;
    while (count < q->size) {
        //printf("i val: %d\n",i);
        //printf("%d ", q->data[i]);
        i = (i + 1) % MAX_QUEUE_SIZE;
        count++;
    }
    //printf("\n");
    //pthread_mutex_unlock(&(q->lock)); // Release lock after accessing the queue
}

// Function to initialize the queue
void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    pthread_mutex_init(&(q->lock), NULL); // Initialize mutex lock
}

// Function to check if queue is full
int isFull(Queue *q) {
    return (q->size == MAX_QUEUE_SIZE);
}

// Function to check if queue is empty
int isEmpty(Queue *q) {
    return (q->size == 0);
}

// Function to enqueue an element
bool enqueue(Queue *q, int value ) {
    pthread_mutex_lock(&(q->lock)); // Acquire lock before modifying the queue
    if (!isFull(q)) {
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        q->data[q->rear] = value;
        // printf("q->data[q->rear]: %d\n",q->data[q->rear]);

        q->size++;
        pthread_mutex_unlock(&(q->lock)); // Release lock after modification
        return true;
    }
    pthread_mutex_unlock(&(q->lock)); // Release lock if queue is full
    return false;
}

// Function to dequeue an element
int dequeue(Queue *q) {
    pthread_mutex_lock(&(q->lock)); // Acquire lock before modifying the queue
    int value = -1; // Default value if queue is empty
    if (!isEmpty(q)) {
        value = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        q->size--;
    }
    pthread_mutex_unlock(&(q->lock)); // Release lock after modification
    return value;
}