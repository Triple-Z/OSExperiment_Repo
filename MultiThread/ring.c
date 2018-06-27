/**
 * Author: TripleZ
 * Date: 6/27/2018
 * 
 * A thread ring.
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 100
#define BUFFER_CAPACITY 4
int buffer[BUFFER_CAPACITY] = {0};
int in = 0, out = 0;

struct params {
    int order;
};

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

sema_t thread_mutex[N];
sema_t buffer_mutex;
sema_t buffer_full_sema, buffer_empty_sema;

void sema_init(sema_t *sema, int value) {
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0) 
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

int get_item() {
    int item;
    item = buffer[out];
    out = (out + 1) % BUFFER_CAPACITY;
    return item;
}

void put_item(char item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_CAPACITY;
}

void *instance (void *args) {
    struct params *param = (struct params *)args;
    int item;
    int i = param->order;
    if (i > 0) {
        sema_wait(&thread_mutex[i]);
        sema_wait(&buffer_full_sema);
        sema_wait(&buffer_mutex);

        item = get_item();
        printf("\033[0;32m%d get item: %d\033[0m\n", i+1, item);

        put_item(++item);
        printf("\033[0;31m%d put item: %d\033[0m\n", i+1, item);

        sema_signal(&buffer_mutex);
        sema_signal(&buffer_full_sema);
        sema_signal(&thread_mutex[(i+1) % N]);
    } else {
        // The first thread
        sema_wait(&buffer_empty_sema);
        sema_wait(&buffer_mutex);
        item = i + 1;
        put_item(item);
        printf("\033[0;31m%d put item: %d\033[0m\n", i+1, item);
        sema_signal(&buffer_mutex);
        sema_signal(&buffer_full_sema);
        sema_signal(&thread_mutex[(i+1) % N]);

        sema_wait(&thread_mutex[i]);
        sema_wait(&buffer_full_sema);
        sema_wait(&buffer_mutex);
        item = get_item();
        printf("\033[0;32m%d get item: %d\033[0m\n", i+1, item);
        sema_signal(&buffer_mutex);
        sema_signal(&buffer_empty_sema);
        sema_signal(&thread_mutex[i]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread[N];
    int i;
    struct params params[N];
    struct params *param = params;

    sema_init(&buffer_mutex, 1);
    sema_init(&buffer_full_sema, 0);
    sema_init(&buffer_empty_sema, BUFFER_CAPACITY - 1);
    for (i = 0; i < N; i++) {
        sema_init(&thread_mutex[i], 0);
    }
    

    for (i = 0; i < N; i++, param++) {
        param->order = i;
        pthread_create(&thread[i], NULL, instance, param);
    }

    pthread_join(thread[0], NULL);

    return 0;
}
