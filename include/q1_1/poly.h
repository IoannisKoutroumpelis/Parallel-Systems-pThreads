#ifndef POLY_H
#define POLY_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

// Δομή για τα ορίσματα των νημάτων
typedef struct {
    int id;                 // ID νήματος (0, 1, ... num_threads-1)
    int n;                  // Βαθμός πολυωνύμου
    int num_threads;        // Συνολικός αριθμός νημάτων
    int *A;                 // Πολυώνυμο Α (Είσοδος)
    int *B;                 // Πολυώνυμο Β (Είσοδος)
    long long *P;           // Αποτέλεσμα (Κοινόχρηστη μνήμη)
} thread_arg_t;

// Βοηθητική συνάρτηση για τη μέτρηση του χρόνου
double now_sec();

// Δημιουργία τυχαίου μη μηδενικού συντελεστή
int random_nonzero_coeff();

// Εκτύπωση πολυωνύμου
void print_poly(const long long int *p, int degree);

// Συνάρτηση νήματος: Υλοποιεί τον αλγόριθμο Output Partitioning
void *thread_mult(void *arg);

// Σειριακός πολλαπλασιασμός πολυωνύμων (για επαλήθευση)
void poly_mul_serial(const int *a, const int *b, long long int *c, int n);

// Έλεγχος ισότητας δύο πολυωνύμων
int check_equal(long long *C1, long long *C2, int n);

#endif