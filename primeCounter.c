#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4  // Adjust this according to your system and workload

int is_prime(int num) {
    if (num <= 1) return 0;  // Not a prime number
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return 0;  // Not a prime number
    }
    return 1;  // Prime number
}

typedef struct {
    int start;
    int end;
    int count;
} ThreadData;

void *find_primes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;
    for (int i = data->start; i <= data->end; ++i) {
        if (is_prime(i)) {
            count++;
        }
    }
    data->count = count;
    pthread_exit(NULL);
}

int main() {
    int total_count = 0;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int range = 1000000;  // Adjust the range as needed

    int chunk_size = range / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].start = i * chunk_size + 1;
        thread_data[i].end = (i == NUM_THREADS - 1) ? range : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, find_primes, (void *)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        total_count += thread_data[i].count;
    }

    printf("Total prime numbers: %d\n", total_count);

    return 0;
}
