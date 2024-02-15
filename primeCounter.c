#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "queue.h"
#define NUM_THREADS 2 // Adjust this according to your system and workload
bool finishReading=false;
Queue queues [NUM_THREADS];// define array of queues
int counters [NUM_THREADS]; //Array of prime counters from each thread (according to queues[i])

//Func read input from generator
void * readData (void * args)
{
    printf("start readData\n ");

    int num;
    int index=0;
    // Read the input
    while (scanf("%d", &num) != EOF) {
        printf("num: %d\n", num);

        // Decide which queue to insert the number int
        while (!enqueue(&queues[index] , num))//if the queue is full
        {

            index = (index+1) % NUM_THREADS;// try to push the next queue
        }
        printf("queues[%d] : %d",index, queues[index].size );
        index = (index+1) % NUM_THREADS;

    }
    finishReading=true; //threads finished the reading of all the numbers
    printf("finish reading!");

    pthread_exit(NULL);
}
// Function to check if a number is prime
bool isPrime(int n) {
    //printf("got number %d!", n);

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

void * find_primes(void * args)
{
    printf("start find primmes");
    int ind = *((int *) args);
    printf("\n\n\nind is %d\n", ind);
    int count = 0;
    // while the queue is not empty,or the reader thread did not finish to read, we will read the numbers and check which are primes
    while (!isEmpty(&queues[ind]) || (!finishReading))
    {
        //printf("isEmpty? %d\n", isEmpty(&queues[ind]));

        //if the queue is not empty we will dequeue and check the number
        //printf("size %d!!", queues[ind].size);

        if (!isEmpty(&queues[ind]) && isPrime(dequeue (&queues[ind]))) {
            count++;

        }
    }
    printf("counters[**ind**]\n", ind);
    counters[ind]=count; //put how many primes this thread found
    printf("thread num %d find %d primes\n", ind, count);
    pthread_exit(NULL);
}

int main() {

    for (int i=0 ; i<NUM_THREADS ;i++)
    {
        // Initialize queues
        initializeQueue(&queues[i]);
    }

//main create the thread that insert numbers into queues
    int total_count = 0;
    pthread_t t_readData; //create thread to read the data
    pthread_create(&t_readData, NULL, (void *(*)(void *))readData, NULL);//send the thread to readData
    pthread_t t_queueWorkers[NUM_THREADS];

    // We will iterate over all numbers in all queues and find prime numbers. create NUM_THREADS threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int cur_index = i;
        printf("\n\n\n\n########i is: %d\n\n\n\n", i);
        pthread_create(&t_queueWorkers[i], NULL, (void *(*)(void *))find_primes, (void *)&cur_index);
    }

    //wait until the reader thread finished and if finished, continue
    pthread_join(t_readData, NULL);//the main thread sleep until t_readData thread is finished, and then wake up

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t_queueWorkers[i], NULL);//the main thread sleep until t_queueWorkers[i] thread is finished, and then wake up
        printf("counters[%d] %d\n", i, counters[i]);
        total_count += counters[i];
    }

    printf("Total prime numbers: %d\n", total_count);


    return 0;
}
