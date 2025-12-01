#include "../../include/q1_1/poly.h"


double now_sec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}


int random_nonzero_coeff() {
    int x = 0;
    while (x == 0) {
        x = (rand() % 21) - 10;
    }
    return x;
}

void print_poly(const long long int *p, int degree) {
    for (int i = degree; i >= 0; i--) {
        long long int c = p[i];
        if (c == 0) continue;

        if (i == degree) printf("%lld", c);
        else {
            if (c > 0) printf(" + %lld", c);
            else       printf(" - %lld", -c);
        }

        if (i > 0) printf("x");
        if (i > 1) printf("^%d", i);
    }
    printf("\n");
}

void *thread_mult(void *arg) {
    thread_arg_t *t = (thread_arg_t*)arg;
    int id = t->id;
    int n = t->n;
    int num_threads = t->num_threads;
    int *A = t->A;
    int *B = t->B;
    long long *P = t->P;

    
    for (int k = id; k <= 2 * n; k += num_threads) {
        long long sum = 0;

        // Υπολογισμός ορίων για το loop convolution
        // Θέλουμε: A[i] * B[j] όπου i + j = k => j = k - i
        // Πρέπει: 0 <= i <= n  ΚΑΙ  0 <= k-i <= n
        
        int start_i = (k - n > 0) ? k - n : 0;
        int end_i   = (k < n) ? k : n;

        for (int i = start_i; i <= end_i; i++) {
            sum += (long long)A[i] * (long long)B[k - i];
        }
        
        P[k] = sum;
    }

    return NULL;
}

void poly_mul_serial(const int *a, const int *b, long long int *c, int n) {
    for (int i = 0; i <= 2 * n; i++) c[i] = 0;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            c[i + j] += (long long)a[i] * (long long)b[j];
        }
    }
}

int check_equal(long long *C1, long long *C2, int n) {
    for (int i = 0; i <= 2*n; i++) {
        if (C1[i] != C2[i])
            return 0;
    }
    return 1;
}