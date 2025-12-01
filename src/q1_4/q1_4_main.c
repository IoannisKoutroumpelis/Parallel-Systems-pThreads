#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define HEAVY_READ_WORK 0 
#define WORK_DELAY_US 10 

int num_accounts;
int num_transactions;
int read_percentage;
int lock_strategy;
int num_threads;

int *accounts;

pthread_mutex_t global_mutex;
pthread_rwlock_t global_rwlock;
pthread_mutex_t *account_mutexes;
pthread_rwlock_t *account_rwlocks;

enum Strategy {
    COARSE_MUTEX = 1,
    COARSE_RWLOCK = 2,
    FINE_MUTEX = 3,
    FINE_RWLOCK = 4
};

void do_read_work() {
    if (HEAVY_READ_WORK) {
        usleep(WORK_DELAY_US); 
    }
}

void *worker(void *arg) {
    long tid = (long)arg;
    unsigned int seed = (unsigned int)tid + time(NULL); 
    long long local_read_sum = 0;

    for (int k = 0; k < num_transactions; k++) {
        int r = rand_r(&seed) % 100;
        
        if (r < read_percentage) {
            int idx = rand_r(&seed) % num_accounts;
            int balance = 0;

            if (lock_strategy == COARSE_MUTEX) {
                pthread_mutex_lock(&global_mutex);
                balance = accounts[idx];
                do_read_work();
                pthread_mutex_unlock(&global_mutex);
            } 
            else if (lock_strategy == COARSE_RWLOCK) {
                pthread_rwlock_rdlock(&global_rwlock);
                balance = accounts[idx];
                do_read_work();
                pthread_rwlock_unlock(&global_rwlock);
            } 
            else if (lock_strategy == FINE_MUTEX) {
                pthread_mutex_lock(&account_mutexes[idx]);
                balance = accounts[idx];
                do_read_work();
                pthread_mutex_unlock(&account_mutexes[idx]);
            } 
            else if (lock_strategy == FINE_RWLOCK) {
                pthread_rwlock_rdlock(&account_rwlocks[idx]);
                balance = accounts[idx];
                do_read_work();
                pthread_rwlock_unlock(&account_rwlocks[idx]);
            }
            
            local_read_sum += balance;
        } 
        else {
            int src = rand_r(&seed) % num_accounts;
            int dst = rand_r(&seed) % num_accounts;
            
            while (src == dst) {
                dst = rand_r(&seed) % num_accounts;
            }

            int amount = (rand_r(&seed) % 100) + 1;

            if (lock_strategy == COARSE_MUTEX) {
                pthread_mutex_lock(&global_mutex);
                if (accounts[src] >= amount) {
                    accounts[src] -= amount;
                    accounts[dst] += amount;
                }
                pthread_mutex_unlock(&global_mutex);
            } 
            else if (lock_strategy == COARSE_RWLOCK) {
                pthread_rwlock_wrlock(&global_rwlock);
                if (accounts[src] >= amount) {
                    accounts[src] -= amount;
                    accounts[dst] += amount;
                }
                pthread_rwlock_unlock(&global_rwlock);
            } 
            else if (lock_strategy == FINE_MUTEX) {
                int first = (src < dst) ? src : dst;
                int second = (src < dst) ? dst : src;

                pthread_mutex_lock(&account_mutexes[first]);
                pthread_mutex_lock(&account_mutexes[second]);

                if (accounts[src] >= amount) {
                    accounts[src] -= amount;
                    accounts[dst] += amount;
                }

                pthread_mutex_unlock(&account_mutexes[second]);
                pthread_mutex_unlock(&account_mutexes[first]);
            } 
            else if (lock_strategy == FINE_RWLOCK) {
                int first = (src < dst) ? src : dst;
                int second = (src < dst) ? dst : src;

                pthread_rwlock_wrlock(&account_rwlocks[first]);
                pthread_rwlock_wrlock(&account_rwlocks[second]);

                if (accounts[src] >= amount) {
                    accounts[src] -= amount;
                    accounts[dst] += amount;
                }

                pthread_rwlock_unlock(&account_rwlocks[second]);
                pthread_rwlock_unlock(&account_rwlocks[first]);
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Usage: %s <num_accounts> <num_transactions> <read_percentage> <strategy 1-4> <num_threads>\n", argv[0]);
        return 1;
    }

    num_accounts = atoi(argv[1]);
    num_transactions = atoi(argv[2]);
    read_percentage = atoi(argv[3]);
    lock_strategy = atoi(argv[4]);
    num_threads = atoi(argv[5]);

    accounts = malloc(sizeof(int) * num_accounts);
    long long initial_total_balance = 0;
    srand(time(NULL));

    for (int i = 0; i < num_accounts; i++) {
        accounts[i] = (rand() % 1000) + 100; 
        initial_total_balance += accounts[i];
    }

    if (lock_strategy == COARSE_MUTEX) {
        pthread_mutex_init(&global_mutex, NULL);
    } else if (lock_strategy == COARSE_RWLOCK) {
        pthread_rwlock_init(&global_rwlock, NULL);
    } else if (lock_strategy == FINE_MUTEX) {
        account_mutexes = malloc(sizeof(pthread_mutex_t) * num_accounts);
        for (int i = 0; i < num_accounts; i++) pthread_mutex_init(&account_mutexes[i], NULL);
    } else if (lock_strategy == FINE_RWLOCK) {
        account_rwlocks = malloc(sizeof(pthread_rwlock_t) * num_accounts);
        for (int i = 0; i < num_accounts; i++) pthread_rwlock_init(&account_rwlocks[i], NULL);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    long long final_total_balance = 0;
    for (int i = 0; i < num_accounts; i++) {
        final_total_balance += accounts[i];
    }

    printf("%.4f\n", elapsed_time); 

    if (initial_total_balance == final_total_balance) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
    }

    free(accounts);
    free(threads);
    if (lock_strategy == FINE_MUTEX) {
        for(int i=0; i<num_accounts; i++) pthread_mutex_destroy(&account_mutexes[i]);
        free(account_mutexes);
    }
    if (lock_strategy == FINE_RWLOCK) {
        for(int i=0; i<num_accounts; i++) pthread_rwlock_destroy(&account_rwlocks[i]);
        free(account_rwlocks);
    }

    return 0;
}