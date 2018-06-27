/**
 * Author: TripleZ
 * Date: 6/27/2018
 * 
 * Use condition variable to solve producer, computer and consumer problem.
 * 
 **/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 4
#define ITEM_COUNT 8

char buffer1[BUFFER_SIZE] = {0};
char buffer2[BUFFER_SIZE] = {0};

int bf1_in, bf1_out;
int bf2_in, bf2_out;

pthread_mutex_t buffer1_mutex, buffer2_mutex;
pthread_cond_t bf1_full_cond, bf1_empty_cond;
pthread_cond_t bf2_full_cond, bf2_empty_cond;

int buffer1_is_empty() {
    return bf1_in == bf1_out;
}

int buffer1_is_full() {
    return (bf1_in + 1) % BUFFER_SIZE == bf1_out;
}

int buffer2_is_empty() {
    return bf2_in == bf2_out;
}

int buffer2_is_full() {
    return (bf2_in + 1) % BUFFER_SIZE == bf2_out;
}

char get_item_from_buf1() {
    char item;

    item = buffer1[bf1_out];
    bf1_out = (bf1_out + 1) % BUFFER_SIZE;
    
    return item;
}

void put_item_to_buf1(char item) {
    buffer1[bf1_in] = item;
    bf1_in = (bf1_in + 1) % BUFFER_SIZE;
}

char get_item_from_buf2() {
    char item;

    item = buffer2[bf2_out];
    bf2_out = (bf2_out + 1) % BUFFER_SIZE;
    
    return item;
}

void put_item_to_buf2(char item) {
    buffer2[bf2_in] = item;
    bf2_in = (bf2_in + 1) % BUFFER_SIZE;
}

void *producer (void *args) {
    int i;
    char item;

    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&buffer1_mutex);
        while(buffer1_is_full())
            pthread_cond_wait(&bf1_empty_cond, &buffer1_mutex);

        item = 'a' + i;
        put_item_to_buf1(item);
        printf("\033[0;32mProduce item: %c\033[0m\n", item);
        pthread_cond_signal(&bf1_full_cond);

        pthread_mutex_unlock(&buffer1_mutex);
    }
    return NULL;
}

void *computer (void *args) {
    int i;
    char item;
    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&buffer1_mutex);
        while(buffer1_is_empty())
            pthread_cond_wait(&bf1_full_cond, &buffer1_mutex);

        item = get_item_from_buf1();
        printf("\033[0;33mComputer get item: %c\033[0m\n", item);
        pthread_cond_signal(&bf1_empty_cond);
        pthread_mutex_unlock(&buffer1_mutex);
        // Compute item
        pthread_mutex_lock(&buffer2_mutex);
        while(buffer2_is_full())
            pthread_cond_wait(&bf2_empty_cond, &buffer2_mutex);
        
        item = item - 32;
        put_item_to_buf2(item);
        printf("\033[0;34mComputer put item: %c\033[0m\n", item);
        pthread_cond_signal(&bf2_full_cond);
        pthread_mutex_unlock(&buffer2_mutex);
    }
    
}

void *consumer (void *args) {
    int i;
    char item;
    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&buffer2_mutex);
        while(buffer2_is_empty())
            pthread_cond_wait(&bf2_full_cond, &buffer2_mutex);
        
        item = get_item_from_buf2();
        printf("\033[0;35mConsumer get item: %c\033[0m\n", item);
        pthread_cond_signal(&bf2_empty_cond);
        pthread_mutex_unlock(&buffer2_mutex);
    }
}

int main (int argc, char *argv[]) {
    pthread_t producer_pid, consumer_pid, computer_pid;

    pthread_mutex_init(&buffer1_mutex, NULL);
    pthread_mutex_init(&buffer2_mutex, NULL);
    pthread_cond_init(&bf1_full_cond, NULL);
    pthread_cond_init(&bf1_empty_cond, NULL);
    pthread_cond_init(&bf2_full_cond, NULL);
    pthread_cond_init(&bf2_empty_cond, NULL);

    pthread_create(&producer_pid, NULL, producer, NULL);
    pthread_create(&computer_pid, NULL, computer, NULL);
    pthread_create(&consumer_pid, NULL, consumer, NULL);

    pthread_join(consumer_pid, NULL);

    pthread_cond_destroy(&bf1_full_cond);
    pthread_cond_destroy(&bf1_empty_cond);
    pthread_cond_destroy(&bf2_full_cond);
    pthread_cond_destroy(&bf2_empty_cond);
    pthread_mutex_destroy(&buffer1_mutex);
    pthread_mutex_destroy(&buffer2_mutex);

    return 0;
}

