/**
 * Author: TripleZ
 * Date: 6/20/2018
 * 
 * Use one assistant thread to calculate pi.
 * 
 **/
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#define N 1000000

double worker_output = 0;
double master_output = 0;

void *worker(void *arg) {
    int i;
    
    for (i = N + 1; i <= 2 * N - 1; i += 2){
        worker_output += pow(-1 , (i-1) / 2) * 1/i;
    }

    return NULL;
}

void master() {
    int i;
    for (i = 1; i <= N - 1; i += 2 ) {
        master_output += pow(-1 , (i-1) / 2) * 1/i;
    }
}


int main (int argc, char *argv[]) {
    pthread_t worker_tid;
    double total, pi;
    
    pthread_create(&worker_tid, NULL, worker, NULL);
    master();
    pthread_join(worker_tid, NULL);
    total = master_output + worker_output;
    pi = total * 4;
    printf("PI = %lf\n", pi);
    return 0;
}
