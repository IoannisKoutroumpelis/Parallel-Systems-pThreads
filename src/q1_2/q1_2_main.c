#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <bits/pthreadtypes.h>

pthread_mutex_t mlock;
pthread_rwlock_t rwlock;

int var;

double now_sec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

void *thread_loop(void *arg){
    int flag = *(int*)arg;
    int m = 100000;

    if(flag == 2) {
        for(int i = 0; i < m; i++){
            pthread_mutex_lock(&mlock);
            var++;
            pthread_mutex_unlock(&mlock);
        }
    } else if(flag == 3) {
        for(int i = 0; i < m; i++){
            pthread_rwlock_wrlock(&rwlock);
            var++;
            pthread_rwlock_unlock(&rwlock);
        }
    } else {
        for(int i = 0; i < m; i++){
            __atomic_fetch_add(&var, 1, __ATOMIC_SEQ_CST);
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 3){
        printf("Run the execθtable like this: ./main n t\nn = 1 for mutex lock, n = 2 for read-write lock, n = 3 for atomic operations (__atomic builtins)\n");
        printf("and t the number of threads\n");
        return -1;
    }
    int impl = atoi(argv[1]);

    int thr_num = atoi(argv[2]);
    if(thr_num <= 0) {
        printf("please enter a positive integer as number of threads\n");
        return -1;
    }

    var = 0;
    pthread_t threads[thr_num];
    //Δημιουργία και αρχικοποίηση πολυωνύμων
    double t_start, t_end; //Μεταβλητές για τη μέτρηση χρόνου

    if(impl == 2) {
        t_start = now_sec();
        //Υλοποίηση με mutex lock
        pthread_mutex_init(&mlock, NULL);

        for(int i = 0; i < thr_num; i++)
            pthread_create(&threads[i], NULL, thread_loop, &impl);
        
        for(int i = 0; i < thr_num; i++)
            pthread_join(threads[i], NULL);

        pthread_mutex_destroy(&mlock);
        t_end = now_sec();
        printf("Mutex lock implementation time: (%d threads): %.6f sec\n", thr_num, t_end-t_start);
    } else if(impl == 3) {
        t_start = now_sec();
        //Υλοποίηση με read-write lock
        pthread_rwlock_init(&rwlock, NULL);

        for(int i = 0; i < thr_num; i++)
            pthread_create(&threads[i], NULL, thread_loop, &impl);

        for(int i = 0; i < thr_num; i++)        
            pthread_join(threads[i], NULL);

        pthread_rwlock_destroy(&rwlock);
        t_end = now_sec();
        printf("Read-write lock implementation time: (%d threads): %.6f sec\n", thr_num, t_end-t_start);
    } else {
        t_start = now_sec();
        for(int i = 0; i < thr_num; i++)
            pthread_create(&threads[i], NULL, thread_loop, &impl);

        for(int i = 0; i < thr_num; i++)        
            pthread_join(threads[i], NULL);

        t_end = now_sec();
        printf("atomic operations implementation time: (%d threads): %.6f sec\n", thr_num, t_end-t_start);
    }

    printf("Final value: %d\n", var);

    return 0;
}
