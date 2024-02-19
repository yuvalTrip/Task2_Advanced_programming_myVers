#include "allPrimes.c"
#include <stdbool.h>

int isqrt(int param_1) {
    int low = 0;
    int high = sizeof(all_primes) / sizeof(unsigned short) - 1;
    //printf("high: %d\n", high);
    int result = 0;
    int mid;

    //int index = 1;
    // Binary search to find the largest prime whose square is less than or equal to param_1
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (all_primes[mid] * all_primes[mid] <= param_1) {
            result = all_primes[mid];
            low = mid + 1;
            //printf("mid: %d, going higher\n", mid);
        } else {
            high = mid - 1;
            //printf("mid: %d, going lower\n", mid);
        }
        //index++;
    }
    //printf("%d iterations\n", index);

    return result;
}



// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    int square = isqrt(n);
    for(int i = 0; i <= 4747;i++){
        if(all_primes[i] > square){
            return true;
        }
        if (n % all_primes[i] == 0){
            return false;
        }
    }
    return true;
}