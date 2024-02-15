#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <malloc.h>
#include "queue.h"

#define NUM_THREADS 4
#define MAX_NUMBERS 1000

bool finishReading = false;
Queue queues[NUM_THREADS];
int counters[NUM_THREADS];

void *readData(void *args) {
    int num;
    int index = 0;
    int tempind=0;
    while (scanf("%d", &num) != EOF) {
      //  printf(" tempind : %d, value: %d \n", tempind,num);
        while (!enqueue(&queues[index], num,tempind)) {
            index = (index + 1) % NUM_THREADS;
        }
        tempind++;
        index = (index + 1) % NUM_THREADS;
    }
    finishReading = true;
    pthread_exit(NULL);
}

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    printf("num: %d\n", n);
    return true;
}

void *find_primes(void *args) {
    int ind = *((int *) args);
    int count = 0;
    while (!isEmpty(&queues[ind]) || (!finishReading)) {
        if (!isEmpty(&queues[ind]) && isPrime(dequeue(&queues[ind]))) {
            count++;
        }
    }
    counters[ind] = count;
    pthread_exit(NULL);
}

int main() {
    pthread_t t_readData;
    pthread_create(&t_readData, NULL, readData, NULL);

    pthread_t t_queueWorkers[NUM_THREADS];
    int *thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = malloc(sizeof(int));
        *thread_ids[i] = i;
        initializeQueue(&queues[i]);
        pthread_create(&t_queueWorkers[i], NULL, find_primes, (void *) thread_ids[i]);
    }

    pthread_join(t_readData, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t_queueWorkers[i], NULL);
        free(thread_ids[i]);
    }

    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("counters[%d] %d\n", i, counters[i]);
        total_count += counters[i];
    }

    printf("Total prime numbers: %d\n", total_count);

    return 0;
}
