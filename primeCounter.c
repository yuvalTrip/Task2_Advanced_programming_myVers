#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h> 
#define NUM_THREADS 3

// Struct to hold multiple parameters for thread function
typedef struct {
    int *counter;
    int thread_index;
} ThreadParams;

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

// Thread function to process input
void *processInput(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    int *counter = params->counter;
    int thread_index = params->thread_index;
    int num;
    
    // Read numbers from stdin until end of file
    while (scanf("%d", &num) != EOF) {
        printf("Thread %d, input %d\n", thread_index, num); // Print thread index and input number
        if (isPrime(num)) {
            (*counter)++;
        }
    }
    
    pthread_exit(NULL);
}

int main() {
    int total_counter = 0;
    pthread_t threads[NUM_THREADS];
    ThreadParams thread_params[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_params[i].counter = &total_counter;
        thread_params[i].thread_index = i;
        pthread_create(&threads[i], NULL, processInput, &thread_params[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_counter);

    return 0;
}
