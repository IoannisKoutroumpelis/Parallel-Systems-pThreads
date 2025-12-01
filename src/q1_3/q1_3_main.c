#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h> // Added for time()

struct array_stats_s {
    long long int info_array_0;
    long long int info_array_1;
    long long int info_array_2;
    long long int info_array_3;
};

struct array_stats_s *array_stats_ptr;

int *arrays[4];
int N;

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

void *thread_func(void *arg) {
    long id = (long)arg;
    int *my_array = arrays[id];

    for (int i = 0; i < N; i++) {
        if (my_array[i] != 0) {
            switch (id) {
                case 0: array_stats_ptr->info_array_0++; break;
                case 1: array_stats_ptr->info_array_1++; break;
                case 2: array_stats_ptr->info_array_2++; break;
                case 3: array_stats_ptr->info_array_3++; break;
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    printf("Array size per thread: %d\n", N);

    // Initialize random seed
    srand(time(NULL));

    double t_start = get_time();

    array_stats_ptr = (struct array_stats_s *)malloc(sizeof(struct array_stats_s));
    if (array_stats_ptr == NULL) {
        perror("Failed to allocate memory for stats struct");
        return 1;
    }

    array_stats_ptr->info_array_0 = 0;
    array_stats_ptr->info_array_1 = 0;
    array_stats_ptr->info_array_2 = 0;
    array_stats_ptr->info_array_3 = 0;

    for (int i = 0; i < 4; i++) {
        arrays[i] = (int *)malloc(N * sizeof(int));
        if (arrays[i] == NULL) {
            perror("Malloc failed for data array");
            return 1;
        }
        for (int j = 0; j < N; j++) {
            arrays[i][j] = rand() % 10;
        }
    }
    
    double t_init = get_time() - t_start;
    printf("Initialization Time: %.6f sec\n", t_init);

    long long s_res[4] = {0};
    t_start = get_time();
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < N; i++) {
            if (arrays[k][i] != 0) {
                s_res[k]++;
            }
        }
    }
    printf("Serial Execution Time: %.6f sec\n", get_time() - t_start);

    pthread_t threads[4];
    t_start = get_time();
    
    for (long i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, (void *)i) != 0) {
            perror("Thread create failed");
            return 1;
        }
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    double t_parallel = get_time() - t_start;
    printf("Parallel Execution Time: %.6f sec\n", t_parallel);

    int correct = 1;
    if (s_res[0] != array_stats_ptr->info_array_0) correct = 0;
    if (s_res[1] != array_stats_ptr->info_array_1) correct = 0;
    if (s_res[2] != array_stats_ptr->info_array_2) correct = 0;
    if (s_res[3] != array_stats_ptr->info_array_3) correct = 0;

    if (correct) {
        printf("Validation: SUCCESS\n");
    } else {
        printf("Validation: FAILURE\n");
        printf("Expected: %lld %lld %lld %lld\n", s_res[0], s_res[1], s_res[2], s_res[3]);
        printf("Got:      %lld %lld %lld %lld\n", 
               array_stats_ptr->info_array_0, 
               array_stats_ptr->info_array_1, 
               array_stats_ptr->info_array_2, 
               array_stats_ptr->info_array_3);
    }


    for (int i = 0; i < 4; i++) {
        free(arrays[i]);
    }
    free(array_stats_ptr);

    return 0;
}