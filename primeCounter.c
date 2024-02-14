#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#define NUM_THREADS 1 // Adjust this according to your system and workload

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
//bool isPrime(int n) {
//    if (n <= 1) {
//        return false;
//    }
//    for (int i = 2; i * i <= n; i++) {
//        if (n % i == 0) {
//            return false;
//        }
//    }
//    return true;
//}
typedef struct {
    int start;
    int end;
    int count;
} ThreadData;

void *find_primes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;
    for (int i = data->start; i <= data->end; ++i) {
        if (isPrime(i)) {
            count++;
        }
    }
    data->count = count;
    printf("data->count %d\n", data->count);

    pthread_exit(NULL);
}

int main() {
    int total_count = 0;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int range = 10000000;  // Adjust the range as needed

    int chunk_size = range / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].start = i * chunk_size + 1;
        thread_data[i].end = (i == NUM_THREADS - 1) ? range : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, find_primes, (void *)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        printf("thread_data[%d].count %d\n", i, thread_data[i].count);
        total_count += thread_data[i].count;
    }

    printf("Total prime numbers: %d\n", total_count);

    return 0;
}
