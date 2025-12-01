#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

pthread_barrier_t barrier;
int N;

double now_sec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < N; i++) {
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_threads> <num_iterations>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    N = atoi(argv[2]);

    pthread_barrier_init(&barrier, NULL, (unsigned)num_threads);
    pthread_t *threads = malloc((size_t)num_threads * sizeof(pthread_t));

    double start = now_sec();

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = now_sec();
    printf("Pthread Barrier Time: %.6f sec\n", end - start);

    pthread_barrier_destroy(&barrier);
    free(threads);
    return 0;
}