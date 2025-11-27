#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
    int thread_id;
    int num_threads;
    int n;          // Βαθμός πολυωνύμων
    int *A;         // Πολυώνυμο Α (είσοδος)
    int *B;         // Πολυώνυμο Β (είσοδος)
    long long *C;   // Πολυώνυμο C (αποτέλεσμα)
} thread_args_t;

// Συνάρτηση για μέτρηση χρόνου (wall clock time)
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

// Σειριακός Αλγόριθμος Πολλαπλασιασμού Πολυωνύμων
// Πολυπλοκότητα: O(n^2)
void serial_multiplication(int n, int *A, int *B, long long *C) {
    // Αρχικοποίηση αποτελέσματος σε 0
    for (int k = 0; k <= 2 * n; k++) {
        C[k] = 0;
    }

    // Υπολογισμός
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            C[i + j] += (long long)A[i] * B[j];
        }
    }
}

// Συνάρτηση που εκτελεί κάθε νήμα
// Υπολογίζει συγκεκριμένους συντελεστές του αποτελέσματος (Output Partitioning)
void *parallel_worker(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int n = args->n;
    
    // Κυκλική κατανομή (Cyclic Distribution) για εξισορρόπηση φορτίου
    // Το νήμα υπολογίζει τους συντελεστές k, k + num_threads, k + 2*num_threads, ...
    for (int k = args->thread_id; k <= 2 * n; k += args->num_threads) {
        long long sum = 0;
        
        // Όρια του αθροίσματος για τον υπολογισμό του C[k]
        // Πρέπει i + j = k => j = k - i
        // Περιορισμοί: 0 <= i <= n  ΚΑΙ  0 <= k - i <= n
        // Από το δεύτερο προκύπτει: i <= k  ΚΑΙ  i >= k - n
        
        int start_i = (k - n > 0) ? k - n : 0;
        int end_i = (k < n) ? k : n;

        for (int i = start_i; i <= end_i; i++) {
            sum += (long long)args->A[i] * args->B[k - i];
        }
        
        args->C[k] = sum;
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Χρήση: %s <βαθμός πολυωνύμου n> <αριθμός νημάτων>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (n <= 0 || num_threads <= 0) {
        printf("Ο βαθμός n και ο αριθμός νημάτων πρέπει να είναι θετικοί.\n");
        return 1;
    }

    printf("Βαθμός n: %d (Μέγεθος πίνακα: %d)\n", n, n+1);
    printf("Νήματα: %d\n", num_threads);
    printf("--------------------------------------------------\n");

    // --- 1. Αρχικοποίηση ---
    double start_time = get_time();

    // Δέσμευση μνήμης
    // Τα πολυώνυμα βαθμού n έχουν n+1 συντελεστές
    int *A = (int *)malloc((n + 1) * sizeof(int));
    int *B = (int *)malloc((n + 1) * sizeof(int));
    // Το γινόμενο έχει βαθμό 2n, άρα 2n+1 συντελεστές
    long long *C_serial = (long long *)malloc((2 * n + 1) * sizeof(long long));
    long long *C_parallel = (long long *)malloc((2 * n + 1) * sizeof(long long));

    if (!A || !B || !C_serial || !C_parallel) {
        printf("Σφάλμα δέσμευσης μνήμης.\n");
        return 1;
    }

    // Γέμισμα με τυχαίους μη-μηδενικούς ακέραιους (π.χ. 1 έως 10)
    srand(time(NULL));
    for (int i = 0; i <= n; i++) {
        A[i] = (rand() % 10) + 1;
        B[i] = (rand() % 10) + 1;
    }

    double init_time = get_time() - start_time;
    printf("Χρόνος Αρχικοποίησης: %.6f sec\n", init_time);


    // --- 2. Σειριακή Εκτέλεση ---
    start_time = get_time();
    serial_multiplication(n, A, B, C_serial);
    double serial_time = get_time() - start_time;
    printf("Χρόνος Σειριακής Εκτέλεσης: %.6f sec\n", serial_time);


    // --- 3. Παράλληλη Εκτέλεση ---
    start_time = get_time();

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    thread_args_t *args = malloc(num_threads * sizeof(thread_args_t));

    // Δημιουργία νημάτων
    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        args[i].num_threads = num_threads;
        args[i].n = n;
        args[i].A = A;
        args[i].B = B;
        args[i].C = C_parallel; // Γράφουν στον πίνακα του παράλληλου
        
        pthread_create(&threads[i], NULL, parallel_worker, (void *)&args[i]);
    }

    // Αναμονή τερματισμού νημάτων (Synchronization Point)
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double parallel_time = get_time() - start_time;
    printf("Χρόνος Παράλληλης Εκτέλεσης: %.6f sec\n", parallel_time);


    // --- 4. Επιβεβαίωση Αποτελεσμάτων ---
    int correct = 1;
    for (int k = 0; k <= 2 * n; k++) {
        if (C_serial[k] != C_parallel[k]) {
            correct = 0;
            printf("Σφάλμα στη θέση %d: Serial=%lld, Parallel=%lld\n", k, C_serial[k], C_parallel[k]);
            break;
        }
    }

    if (correct) {
        printf("Επιβεβαίωση: Ο παράλληλος αλγόριθμος παρήγαγε σωστά αποτελέσματα.\n");
        printf("Speedup: %.2fx\n", serial_time / parallel_time);
    } else {
        printf("Επιβεβαίωση: ΑΠΟΤΥΧΙΑ.\n");
    }

    // Αποδέσμευση μνήμης
    free(A); free(B); free(C_serial); free(C_parallel);
    free(threads); free(args);

    return 0;
}