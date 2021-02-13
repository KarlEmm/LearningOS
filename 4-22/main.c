#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#define NBOFTHREADS 3

typedef struct args {
    int argc;
    char **argv;
} ARGS;

/* Global variables shared by the threads */
double average;
double maximum;
double minimum;

/* Thread workers declaration */
void *workerAvr(void *param);
void *workerMax(void *param);
void *workerMin(void *param);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./main x y ... z\n");
        return 1;
    }
    /* struct fields placed on the stack of main */
    ARGS args;
    args.argc = argc;
    args.argv = argv;

    pthread_t tid[NBOFTHREADS];

    pthread_attr_t attr[NBOFTHREADS];

    for (int i = 0; i < NBOFTHREADS; i++) {
        pthread_attr_init(&attr[i]);
    }

    for (int i = 0; i < NBOFTHREADS; i++) {
        void *worker;
        switch (i) {
            case 0:
                worker = workerAvr;
                break;
            case 1:
                worker = workerMax;
                break;
            case 2:
                worker = workerMin;
                break;
        }

        pthread_create(&tid[i], &attr[i], worker, &args);
    }

    for (int i = 0; i < NBOFTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("The average value is: %.2f\n", average);
    printf("The minimum value is: %.2f\n", minimum);
    printf("The maximum value is: %.2f\n", maximum);

    return 0;
}

void *workerAvr(void *param) {
    ARGS *args = (ARGS *) param;
    double sum = 0;
    for (int i = 1; i < args->argc; i++) {
        /* No out of range is checked */
        sum += atof(args->argv[i]);
    }
    average = sum / (args->argc - 1);
    pthread_exit(0);
}

void *workerMax(void *param) {
    ARGS *args = (ARGS *) param;
    double max = 0;
    for (int i = 1; i < args->argc; i++) {
        if (atof(args->argv[i]) > max)
            max = atof(args->argv[i]);
    }
    maximum = max;
    pthread_exit(0);
}

void *workerMin(void *param) {
    ARGS *args = (ARGS *) param;
    double min = INFINITY;
    for (int i = 1; i < args->argc; i++) {
        if (atof(args->argv[i]) < min)
            min = atof(args->argv[i]);
    }
    minimum = min;
    pthread_exit(0);
}

