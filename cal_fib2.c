#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>

#define MAX_THREAD_DEPTH 2 // 控制最大平行層數，避免 thread 爆炸

typedef struct {
    long n;
    int depth;
    mpz_t result;
} FibTask;

void* fibonacci_parallel(void* arg);

// Helper to call fibonacci recursively (in thread or same thread)
void fibonacci_recursive(long n, int depth, mpz_t result) {
    if (n == 0) {
        mpz_set_ui(result, 0);
        return;
    }
    if (n == 1 || n == 2) {
        mpz_set_ui(result, 1);
        return;
    }

    long k = n / 2;

    mpz_t f_k, f_k1, temp;
    mpz_inits(f_k, f_k1, temp, NULL);

    pthread_t t1, t2;
    FibTask task1 = {.n = k, .depth = depth + 1};
    FibTask task2 = {.n = k + 1, .depth = depth + 1};
    mpz_init(task1.result);
    mpz_init(task2.result);

    int use_threads = (depth < MAX_THREAD_DEPTH);

    if (use_threads) {
        pthread_create(&t1, NULL, fibonacci_parallel, &task1);
        pthread_create(&t2, NULL, fibonacci_parallel, &task2);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    } else {
        fibonacci_recursive(k, depth + 1, task1.result);
        fibonacci_recursive(k + 1, depth + 1, task2.result);
    }

    mpz_set(f_k, task1.result);
    mpz_set(f_k1, task2.result);

    // F(2k) = F(k) * [2F(k+1) − F(k)]
    // F(2k+1) = F(k+1)^2 + F(k)^2
    if (n % 2 == 0) {
        mpz_mul(result, f_k1, f_k);
        mpz_mul_ui(result, result, 2);
        mpz_pow_ui(temp, f_k, 2);
        mpz_sub(result, result, temp);
    } else {
        mpz_pow_ui(result, f_k1, 2); // F(k+1)^2
        mpz_addmul(result, f_k, f_k); // + F(k)^2
    }

    mpz_clears(f_k, f_k1, temp, task1.result, task2.result, NULL);
}

void* fibonacci_parallel(void* arg) {
    FibTask* task = (FibTask*)arg;
    fibonacci_recursive(task->n, task->depth, task->result);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    long n = strtol(argv[1], NULL, 10);
    if (n < 0) {
        fprintf(stderr, "Fibonacci only defined for n >= 0\n");
        return EXIT_FAILURE;
    }

    FibTask task = {.n = n, .depth = 0};
    mpz_init(task.result);

    clock_t start = clock();
    fibonacci_recursive(n, 0, task.result);
    clock_t end = clock();

    printf("Fibonacci(%ld) = ", n);
    mpz_out_str(stdout, 10, task.result);
    printf("\n");

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %.6f seconds\n", time_taken);

    mpz_clear(task.result);
    return EXIT_SUCCESS;
}
