#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include "queue.h"
#include "advanced_prime.c"

#define NUM_THREADS 4

bool finishReading = false;
Queue queues[NUM_THREADS];
int counters[NUM_THREADS];
//pthread_mutex_t mutexes[NUM_THREADS];

void *readData(void *args) {
    int num = 0;
    int index = 0;
    int batch_size = 1000; // Adjust the batch size as needed
    int numbers[batch_size];
    int first_index_to_override = 0;

    while (scanf("%d", &num) != EOF) {
        //printf("first_index_to_override:%d\n",first_index_to_override);
        numbers[first_index_to_override++] = num;
        while (first_index_to_override == batch_size)
        {
            first_index_to_override = enqueueMany(&queues[index], numbers, first_index_to_override); ///////////////
            //printf("first_index_to_override loop:%d\n",first_index_to_override);
            //printf("(first_index_to_override == batch_size):%d\n",(first_index_to_override == batch_size));
            index = (index + 1) % NUM_THREADS; ///////////////////////////////
        }
    }
    //printf("AFTERLOOP\n");

    // Enqueue remaining numbers
    while (first_index_to_override > 0) {
        first_index_to_override=enqueueMany(&queues[index], numbers, first_index_to_override); // Think about it
        index = (index + 1) % NUM_THREADS; ///////////////////////////////
    }

    finishReading = true;
    pthread_exit(NULL);
}

bool isPrime2(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    //printf("prime num: %d\n",n);
    return true;
}

void *find_primes(void *args) {
    int ind = *((int *) args);
    int count = 0;
    int batch_size = 1000; // Batch size for dequeuing
    int nums[batch_size];

    while (!finishReading || !isEmpty(&queues[ind])) {
        //pthread_mutex_lock(&mutexes[ind]);
        int dequeued = dequeueMany(&queues[ind], nums, batch_size);
      //  pthread_mutex_unlock(&mutexes[ind]);

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
    //printf ("this is the start");
    pthread_t t_queueWorkers[NUM_THREADS];
    pthread_t t_readData;
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        initializeQueue(&queues[i]);
       // pthread_mutex_init(&mutexes[i], NULL);
        thread_ids[i] = i;  // Store thread ID explicitly
    }

    pthread_create(&t_readData, NULL, readData, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&t_queueWorkers[i], NULL, find_primes, (void *) &thread_ids[i]);
        //printf("got heere1\n");

    }
    //printf("got heere2\n");

    pthread_join(t_readData, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t_queueWorkers[i], NULL);
        //printf("got heere3\n");
    }

    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        //printf("counters[%d] %d\n", i, counters[i]);
        total_count += counters[i];
    }

    printf("Total prime numbers: %d\n", total_count);

    return 0;
}
