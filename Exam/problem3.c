#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

struct params {
    int start;
    int end;
};

struct result {
    int sum;
};

void *compute (void *args) {
    struct params *param = (struct params *)args;

    int sum = 0, i;
    for (i = param->start; i <= param->end; i++) {
        sum += i;
    }
    // printf("sum = %d, start = %d, end = %d\n", sum, param->start, param->end);
    struct result *result = malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}

int main(int argc, char *argv[]) {
    pthread_t worker[10];
    struct params param[10];
    struct params *p_param = param;
    struct result *result;
    int sum = 0;

    int i;
    for (i = 0; i < 10; i++, p_param++) {
        p_param->start = i * 10 + 1;
        p_param->end = (i+1) * 10;
        pthread_create(&worker[i], NULL, compute, p_param);
    }

    for (i = 0; i < 10; i++) {
        // struct result *result;
        pthread_join(worker[i], (void**)&result);
        sum += result->sum;
        free(result);
    }

    printf("Total result = %d\n", sum);

    return 0;
}
