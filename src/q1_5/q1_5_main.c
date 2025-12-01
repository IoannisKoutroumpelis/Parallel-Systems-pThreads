#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define THREADS 4
#define CYCLES 1000000

pthread_barrier_t lib_bar;

typedef struct {
    pthread_mutex_t m;
    pthread_cond_t c;
    int count;
    int target;
    int phase;
} cond_barrier_t;

cond_barrier_t cond_bar;

typedef struct {
    pthread_mutex_t m;
    int count;
    int target;
    int global_sense;
} spin_barrier_t;

spin_barrier_t spin_bar;

void* task_lib(void* arg) {
    for (int i = 0; i < CYCLES; i++) {
        pthread_barrier_wait(&lib_bar);
    }
    return NULL;
}

void cond_barrier_init(cond_barrier_t* b, int t) {
    pthread_mutex_init(&b->m, NULL);
    pthread_cond_init(&b->c, NULL);
    b->count = 0;
    b->target = t;
    b->phase = 0;
}

void cond_barrier_wait(cond_barrier_t* b) {
    pthread_mutex_lock(&b->m);
    b->count++;
    if (b->count == b->target) {
        b->count = 0;
        b->phase++;
        pthread_cond_broadcast(&b->c);
    } else {
        int my_phase = b->phase;
        while (b->phase == my_phase) {
            pthread_cond_wait(&b->c, &b->m);
        }
    }
    pthread_mutex_unlock(&b->m);
}

void* task_cond(void* arg) {
    for (int i = 0; i < CYCLES; i++) {
        cond_barrier_wait(&cond_bar);
    }
    return NULL;
}

void spin_barrier_init(spin_barrier_t* b, int t) {
    pthread_mutex_init(&b->m, NULL);
    b->count = 0;
    b->target = t;
    b->global_sense = 0;
}

void spin_barrier_wait(spin_barrier_t* b, int* my_sense) {
    *my_sense = !(*my_sense);
    pthread_mutex_lock(&b->m);
    b->count++;
    int last = (b->count == b->target);
    if (last) {
        b->count = 0;
        b->global_sense = *my_sense;
    }
    pthread_mutex_unlock(&b->m);

    if (!last) {
        while (b->global_sense != *my_sense);
    }
}

void* task_spin(void* arg) {
    int local_sense = 0;
    for (int i = 0; i < CYCLES; i++) {
        spin_barrier_wait(&spin_bar, &local_sense);
    }
    return NULL;
}

int main() {
    pthread_t t[THREADS];
    struct timeval t1, t2;
    double time_taken;

    pthread_barrier_init(&lib_bar, NULL, THREADS);
    gettimeofday(&t1, NULL);
    for (int i = 0; i < THREADS; i++) pthread_create(&t[i], NULL, task_lib, NULL);
    for (int i = 0; i < THREADS; i++) pthread_join(t[i], NULL);
    gettimeofday(&t2, NULL);
    pthread_barrier_destroy(&lib_bar);
    time_taken = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;
    printf("Pthread Barrier: %f sec\n", time_taken);

    cond_barrier_init(&cond_bar, THREADS);
    gettimeofday(&t1, NULL);
    for (int i = 0; i < THREADS; i++) pthread_create(&t[i], NULL, task_cond, NULL);
    for (int i = 0; i < THREADS; i++) pthread_join(t[i], NULL);
    gettimeofday(&t2, NULL);
    pthread_mutex_destroy(&cond_bar.m);
    pthread_cond_destroy(&cond_bar.c);
    time_taken = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;
    printf("CondVar Barrier: %f sec\n", time_taken);

    spin_barrier_init(&spin_bar, THREADS);
    gettimeofday(&t1, NULL);
    for (int i = 0; i < THREADS; i++) pthread_create(&t[i], NULL, task_spin, NULL);
    for (int i = 0; i < THREADS; i++) pthread_join(t[i], NULL);
    gettimeofday(&t2, NULL);
    pthread_mutex_destroy(&spin_bar.m);
    time_taken = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;
    printf("Spin    Barrier: %f sec\n", time_taken);

    return 0;
}