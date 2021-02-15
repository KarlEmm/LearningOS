#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_INPUT_SIZE 10 /* maximum number of digits in a int */

void *workerPrime(void *param);

int readNumber(int *number) {
    char *inputString = (char *) malloc(sizeof (char) * MAX_INPUT_SIZE + 1);
    int c;
    char *inputStringIdx = inputString;
    while ((c = getc(stdin)) != '\n') {
        *inputStringIdx++ = (char) c;
        if (inputStringIdx == inputString + MAX_INPUT_SIZE)
            break;
    }
    *inputStringIdx = '\0';
    *number = atoi(inputString);
    return 0;
}

int main(int argc, char *argv[]) {
    int num;
    readNumber(&num);

    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_create(&tid, &attr, workerPrime, &num);
    pthread_join(tid, NULL);

    return 0;
}

void *workerPrime(void *param) {
    int *num = (int *) param;
    /* Array of flags about primes */
    int isPrime[*num];

    /* Set all flags to true to start at 2 being a prime */
    for (int i = 2; i <= *num; ++i) {
        isPrime[i] = 1;
    }

    /* Array to hold the prime */
    int prime[*num];
    int primeIdx = 0;

    for (int k = 2; k <= *num; ++k) {
        if (isPrime[k]) {
            prime[primeIdx++] = k;
            printf("%i\n", k);
            /* Square k, because all the previous factor to k will have been ruled
             * out by the previous iterations */
            int composite = k * k;
            while (composite <= *num) {
                isPrime[composite] = 0;
                composite += k;
            }
        }
    }
}


