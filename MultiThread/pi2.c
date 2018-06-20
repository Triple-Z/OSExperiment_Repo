/**
 * Author: TripleZ
 * Date: 6/20/2018
 * 
 * Use multi-thread to calculate pi.
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#define N 1000000
#define N_CPU_CORE 8

struct param {
    int start;
    int end;
};

struct result {
    double sum;
};

void *compute(void *arg) {
    struct param *param;
    struct result *result;
    double sum = 0;
    int i;
    
    param = (struct param*) arg;

    for (i = param->start; i <= param->end; i += 2){
        sum += pow(-1 , (i-1) / 2) * 1/i;
    }
    
    result = malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}


int main (int argc, char *argv[]) {
    pthread_t workers[N_CPU_CORE];
    struct param params[N_CPU_CORE];
    struct param *param;
    struct result *result;
    int i;
    double total = 0, pi = 0;
    
    for (i = 0; i < N_CPU_CORE; i++) {
        // struct param *param;
        param = &params[i];
        param->start = 2 * (i * (N / N_CPU_CORE)) + 1;
        param->end = 2 * ((i+1) * (N / N_CPU_CORE)) + 1;
        pthread_create(&workers[i], NULL, compute, param);
    }

    for (i = 0; i < N_CPU_CORE; i++) {
        // struct result *result;
        pthread_join(workers[i], (void **)&result);
        total += result->sum;
        free(result);
    }

    pi = total * 4;
    printf("PI = %lf\n", pi);
    return 0;
}
