#include "queue.h"
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

void printQueue(Queue *q) {
    pthread_mutex_lock(&(q->lock));
    int i = q->front;
    int count = 0;
    while (count < q->size) {
        printf("%d ", q->data[i]);
        i = (i + 1) % MAX_QUEUE_SIZE;
        count++;
    }
    printf("\n");
    pthread_mutex_unlock(&(q->lock));
}

void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    pthread_mutex_init(&(q->lock), NULL);
}

int isFull(Queue *q) {
    return (q->size == MAX_QUEUE_SIZE);
}

int isEmpty(Queue *q) {
    return (q->size == 0);
}

bool enqueue(Queue *q, int value) {
    pthread_mutex_lock(&(q->lock));
    if (!isFull(q)) {
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        q->data[q->rear] = value;
        q->size++;
        pthread_mutex_unlock(&(q->lock));
        return true;
    }
    pthread_mutex_unlock(&(q->lock));
    return false;
}

int dequeue(Queue *q) {
    pthread_mutex_lock(&(q->lock));
    int value = -1;
    if (!isEmpty(q)) {
        value = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        q->size--;
    }
    pthread_mutex_unlock(&(q->lock));
    return value;
}

int dequeueMany(Queue *q, int *values, int count) {
    pthread_mutex_lock(&(q->lock));
    int dequeued = 0;
    while (!isEmpty(q) && dequeued < count) {
        values[dequeued++] = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        q->size--;
    }
    pthread_mutex_unlock(&(q->lock));
    return dequeued;
}

int enqueueMany(Queue *q, int *values, int count) {
    pthread_mutex_lock(&(q->lock));
    int updated_inserted_index = count - 1;
    while (!isFull(q) && updated_inserted_index >= 0) {
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        q->data[q->rear] = values[updated_inserted_index--];
        q->size++;
    }
    pthread_mutex_unlock(&(q->lock));
    return updated_inserted_index + 1;
}
