#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_NUMBERS 1000 // Maximum number of integers to process
#define NUM_THREADS 4     // Number of threads to use

// Global variables
int numbers[MAX_NUMBERS];
int total_primes = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

// Worker function for each thread
void* threadWorker(void* arg) {
    int start = *((int*)arg); // Starting index for this thread
    int end = start + (MAX_NUMBERS / NUM_THREADS); // Ending index

    int local_count = 0; // Local count of prime numbers

    // Iterate over assigned range of numbers and count primes
    for (int i = start; i < end; i++) {
        if (isPrime(numbers[i])) {
            local_count++;
        }
    }

    // Update global count of prime numbers using mutex
    pthread_mutex_lock(&mutex);
    total_primes += local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    // Read numbers from stdin
    int num, num_count = 0;
    while (scanf("%d", &num) != EOF && num_count < MAX_NUMBERS) {
        numbers[num_count++] = num;
    }

    // Create threads
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    // Start threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i * (MAX_NUMBERS / NUM_THREADS); // Assigning starting index for each thread
        pthread_create(&threads[i], NULL, threadWorker, &thread_args[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_primes);

    return 0;
}
