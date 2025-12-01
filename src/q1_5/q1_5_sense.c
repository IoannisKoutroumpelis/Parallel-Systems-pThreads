#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct {
    int count;
    int total;
    volatile bool sense; // Βάλαμε volatile για να καταλαβαίνει ο compiler ότι μπορεί να αλλάξει τιμή απρόβλεπτα
    pthread_mutex_t mtx;
    int global_sense;
} barrier_t;

barrier_t barrier;
int N;

double now_sec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

void barrier_init(barrier_t *b, int total) {
    b->count = 0;
    b->total = total;
    b->sense = false;
    pthread_mutex_init(&b->mtx, NULL);
}

void barrier_wait(barrier_t *b) {
    bool local_sense = b->sense;
    pthread_mutex_lock(&b->mtx);
    b->count++;
    local_sense = !local_sense;
    if (b->count == b->total) {
        b->count = 0;
        b->sense = local_sense;
        pthread_mutex_unlock(&b->mtx);
    } else {
        pthread_mutex_unlock(&b->mtx);
        while (b->sense != local_sense) {
            // Busy wait
        }
    }
}

void barrier_destroy(barrier_t *b) {
    pthread_mutex_destroy(&b->mtx);
}

void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < N; i++) {
        barrier_wait(&barrier);
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

    barrier_init(&barrier, num_threads);
    pthread_t *threads = malloc((size_t)num_threads * sizeof(pthread_t));

    double start = now_sec();

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double end = now_sec();
    printf("Sense Reversal Barrier Time: %.6f sec\n", end - start);

    barrier_destroy(&barrier);
    free(threads);
    return 0;
}