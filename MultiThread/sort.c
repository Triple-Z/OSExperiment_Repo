/**
 * Author: TripleZ
 * Date: 6/20/2018
 * 
 * Use assistant thread to sort array.
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

pthread_mutex_t lock;
int array[] = {8,2,6,53,4,2,3,5,7,32,3,4};

struct params {
    int start;
    int end;
};

void *sortEnd(void *args) {
    struct params *param;
    param = (struct params *)args;

    int i, j;
    int min, minIndex;
    for (i = param->start; i <= param->end; i++) {
        min = array[i];
        minIndex = i;

        for (j = i; j <= param->end; j++) {
            if (array[j] < min) {
                min = array[j];
                minIndex = j;
            }
        }

        if (minIndex != i) {
            pthread_mutex_lock(&lock);
            // printf("Exchange %d with %d ...\n", array[i], array[minIndex]);
            Exchange(i, minIndex);
            pthread_mutex_unlock(&lock);
            // int tem = array[i];
            // array[i] = array[minIndex];
            // array[minIndex] = tem;
        }
    }
}

void sortFront(int start, int end) {
    int i, j;
    int min, minIndex;
    for (i = start; i <= end; i++) {
        min = array[i];
        minIndex = i;

        for (j = i; j <= end; j++) {
            if (array[j] < min) {
                min = array[j];
                minIndex = j;
            }
        }

        if (minIndex != i) {
            pthread_mutex_lock(&lock);
            Exchange(i, minIndex);
            pthread_mutex_unlock(&lock);
        }
    }
}

void Exchange(int i, int j) {
    int tem = array[i];
    array[i] = array[j];
    array[j] = tem;
}

void mergeArray(int totalElement) {
    int *a = array;
    int *b = (int*) malloc (totalElement*sizeof(int));
    int *tmp = b;
    int i;
    int start1 = 0, end1 = totalElement / 2 - 1;
    int start2 = totalElement / 2, end2 = totalElement - 1;
    while (start1 <= end1 && start2 <= end2) {
        *tmp++ = array[start1] < array[start2] ? array[start1++] : array[start2++];
    }
    while (start1 <= end1){
        *tmp++ = array[start1++];
    }
    while (start2 <= end2) {
        *tmp++ = array[start2++];
    }

    for (i = 0; i < totalElement; i++) {
        array[i] = b[i];
    }

    free(b);
}

void printArray(int totalElement) {
    int i;
    for (i = 0; i < totalElement; i++) {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

int main (int argc, char *argv[]) {
    pthread_t worker_tid;
    struct params params;
    int totalElement = sizeof(array) / sizeof(array[0]);
    printf("Original: ");
    printArray(totalElement);
    params.start = totalElement / 2;
    params.end = totalElement - 1;

    pthread_mutex_init(&lock, NULL);
    pthread_create(&worker_tid, NULL, sortEnd, &params);
    sortFront(0, totalElement / 2 - 1);
    pthread_join(worker_tid, NULL);
    pthread_mutex_destroy(&lock);

    printf("Sorted: ");
    printArray(totalElement);
    mergeArray(totalElement);
    printf("Merged: ");
    printArray(totalElement);
    return 0;
}

