#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep函数声明在该头文件中

pthread_cond_t relation1_cond;
pthread_cond_t relation2_cond;
pthread_mutex_t relation1_mutex;
pthread_mutex_t relation2_mutex;
int t1 = 0, t2 = 0, t3 = 0;


void *T1_entry(void *arg)
{
    int status;
    status = pthread_mutex_lock(&relation1_mutex);
    // printf("status T1 lock = %d\n", status);
    sleep(2);  // 睡眠2秒，不准删除此条语句，否则答题无效
    puts("T1");
    t1 = 1;
    pthread_cond_broadcast(&relation1_cond);
    pthread_mutex_unlock(&relation1_mutex);

    return NULL;
}

void *T2_entry(void *arg)
{
    int status;
    status = pthread_mutex_lock(&relation1_mutex);
    // printf("status = %d\n", status);
    while (!t1) 
        pthread_cond_wait(&relation1_cond, &relation1_mutex);
    pthread_mutex_unlock(&relation1_mutex);
    // printf("relation1_mutex unlocked\n");

    sleep(1);  // 睡眠1秒，不准删除此条语句，否则答题无效
    puts("T2");

    pthread_mutex_lock(&relation2_mutex);
    t2 = 1;
    pthread_cond_broadcast(&relation2_cond);
    pthread_mutex_unlock(&relation2_mutex);
    return NULL;
}

void *T3_entry(void *arg)
{
    pthread_mutex_lock(&relation1_mutex);
    while (!t1)
        pthread_cond_wait(&relation1_cond, &relation1_mutex);
    pthread_mutex_unlock(&relation1_mutex);

    sleep(1);  // 睡眠1秒，不准删除此条语句，否则答题无效
    puts("T3");

    pthread_mutex_lock(&relation2_mutex);
    t3 = 1;
    pthread_cond_broadcast(&relation2_cond);
    pthread_mutex_unlock(&relation2_mutex);
    return NULL;
}

void *T4_entry(void *arg)
{
    pthread_mutex_lock(&relation2_mutex);
    while (!t2 || !t3) 
        pthread_cond_wait(&relation2_cond, &relation2_mutex);

    puts("T4");
    pthread_mutex_unlock(&relation2_mutex);
    return NULL;
}

int main (int argc, char *argv[]) {
    pthread_t thread[4];

    pthread_mutex_init(&relation1_mutex, NULL);
    pthread_mutex_init(&relation2_mutex, NULL);

    pthread_create(&thread[1], NULL, T2_entry, NULL);
    pthread_create(&thread[0], NULL, T1_entry, NULL);
    pthread_create(&thread[2], NULL, T3_entry, NULL);
    pthread_create(&thread[3], NULL, T4_entry, NULL);    

    pthread_join(thread[3], NULL);
    return 0;
}
