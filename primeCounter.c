#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include "queue.h"

#define NUM_THREADS 4
#define MAX_NUMBERS 1000

bool finishReading = false;
Queue queues[NUM_THREADS];
int counters[NUM_THREADS];
pthread_mutex_t mutexes[NUM_THREADS];

void *readData(void *args) {
    int num = 0;
    int index = 0;
    while (scanf("%d", &num) != EOF) {
        pthread_mutex_lock(&mutexes[index]);
        while (isFull(&queues[index])) {
            pthread_mutex_unlock(&mutexes[index]);
            index = (index + 1) % NUM_THREADS;
            pthread_mutex_lock(&mutexes[index]);
        }
        enqueue(&queues[index], num);
        pthread_mutex_unlock(&mutexes[index]);
        index = (index + 1) % NUM_THREADS;
    }
    finishReading = true;
    pthread_exit(NULL);
}

//bool isPrime(int n) {
//    if (n <= 1) return false;
//    if (n <= 3) return true;
//    if (n % 2 == 0 || n % 3 == 0) return false;
//    int i = 5;
//    while (i * i <= n) {
//        if (n % i == 0 || n % (i + 2) == 0) return false;
//        i += 6;
//    }
//    return true;
//}
// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void *find_primes(void *args) {
    int ind = *((int *) args);
    int count = 0;
    int batch_size = 1000; // Batch size for dequeuing
    int nums[batch_size];
    while (!finishReading || !isEmpty(&queues[ind])) {
        pthread_mutex_lock(&mutexes[ind]);
        int dequeued = dequeueMany(&queues[ind], nums, batch_size);
        pthread_mutex_unlock(&mutexes[ind]);
        for (int i = 0; i < dequeued; ++i) {
            if (isPrime(nums[i])) {
                count++;
            }
        }
    }
    counters[ind] = count;
    pthread_exit(NULL);
}

int main() {
    pthread_t t_queueWorkers[NUM_THREADS];
    pthread_t t_readData;
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        initializeQueue(&queues[i]);
        pthread_mutex_init(&mutexes[i], NULL);
        thread_ids[i] = i;  // Store thread ID explicitly
    }

    pthread_create(&t_readData, NULL, readData, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&t_queueWorkers[i], NULL, find_primes, (void *) &thread_ids[i]);
    }

    pthread_join(t_readData, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t_queueWorkers[i], NULL);
    }

    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("counters[%d] %d\n", i, counters[i]);
        total_count += counters[i];
    }

    printf("Total prime numbers: %d\n", total_count);

    return 0;
}
