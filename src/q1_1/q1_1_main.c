#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../include/q1_1/poly.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <degree n> <num_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (n <= 0 || num_threads <= 0) {
        fprintf(stderr, "Degree and number of threads must be positive integers.\n");
        return 1;
    }

    printf("Polynomial Degree: %d\nNumber of Threads: %d\n", n, num_threads);

    double t_start = now_sec();
    
    int *A = malloc((n + 1) * sizeof(int));
    int *B = malloc((n + 1) * sizeof(int));
    long long int *mult_serial   = malloc((2 * n + 1) * sizeof(long long int));
    long long int *mult_parallel = malloc((2 * n + 1) * sizeof(long long int)); 

    if (!A || !B || !mult_serial || !mult_parallel) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    for(int i = 0; i <= n; i++) {
        A[i] = random_nonzero_coeff();
        B[i] = random_nonzero_coeff();
    }
    printf("Initialization time: %.6f sec\n", now_sec() - t_start);

    t_start = now_sec();
    poly_mul_serial(A, B, mult_serial, n);
    printf("Serial time:         %.6f sec\n", now_sec() - t_start);


    t_start = now_sec();

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    thread_arg_t *args = malloc(num_threads * sizeof(thread_arg_t));

    for (int i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].n = n;
        args[i].num_threads = num_threads;
        args[i].A = A;
        args[i].B = B;
        args[i].P = mult_parallel; 

        if (pthread_create(&threads[i], NULL, thread_mult, &args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    

    double t_end = now_sec();
    printf("Parallel time:       %.6f sec\n", t_end - t_start);

    if (check_equal(mult_serial, mult_parallel, n)) {
        printf("Result: OK (Matches Serial)\n");
    } else {
        printf("Result: ERROR (Mismatch)\n");
    }

    free(A);
    free(B);
    free(mult_serial);
    free(mult_parallel);
    free(threads);
    free(args);

    return 0;
}