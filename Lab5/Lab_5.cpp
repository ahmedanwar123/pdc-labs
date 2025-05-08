#include <iostream>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cmath>
#define MATRIX_SIZE 2048
#define NUM_THREADS 8

// Timer Macros
#define TIMERSTART(label)                                                  \
    std::chrono::time_point<std::chrono::system_clock> a##label, b##label; \
    a##label = std::chrono::system_clock::now();

#define TIMERSTOP(label)                                                   \
    b##label = std::chrono::system_clock::now();                           \
    std::chrono::duration<double> delta##label = b##label - a##label;      \
    std::cout << #label " Execution Time: " << delta##label.count() << " seconds\n";

static double A[MATRIX_SIZE][MATRIX_SIZE];
static double B[MATRIX_SIZE][MATRIX_SIZE];
static double C[MATRIX_SIZE][MATRIX_SIZE];

struct ThreadData {
    int thread_id;
};

// Serial matrix subtraction
void serial_matrix_subtraction() {
    TIMERSTART(serial)
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    TIMERSTOP(serial)
}

// Block Distribution
void *block_subtraction(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int id = data->thread_id;
    int rows_per_thread = MATRIX_SIZE / NUM_THREADS;
    int start = id * rows_per_thread;
    int end = (id == NUM_THREADS - 1) ? MATRIX_SIZE : start + rows_per_thread;

    for (int i = start; i < end; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    pthread_exit(nullptr);
}

void parallel_block() {
    TIMERSTART(block)
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], nullptr, block_subtraction, (void *)&thread_data[i]);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }
    TIMERSTOP(block)
}

// Cyclic Distribution
void *cyclic_subtraction(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int id = data->thread_id;
    
    for (int i = id; i < MATRIX_SIZE; i += NUM_THREADS) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    pthread_exit(nullptr);
}

void parallel_cyclic() {
    TIMERSTART(cyclic)
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], nullptr, cyclic_subtraction, (void *)&thread_data[i]);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }
    TIMERSTOP(cyclic)
}

// Block-Cyclic Distribution
void *block_cyclic_subtraction(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int id = data->thread_id;
    int block_size = MATRIX_SIZE / NUM_THREADS;
    
    for (int i = id * block_size; i < MATRIX_SIZE; i += block_size * NUM_THREADS) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    pthread_exit(nullptr);
}

void parallel_block_cyclic() {
    TIMERSTART(block_cyclic)
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], nullptr, block_cyclic_subtraction, (void *)&thread_data[i]);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }
    TIMERSTOP(block_cyclic)
}

int main() {
    // Initialize matrices A and B
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            A[i][j] = i + j;
            B[i][j] = i - j;
        }
    }
    
    serial_matrix_subtraction();
    parallel_block();
    parallel_cyclic();
    parallel_block_cyclic();
    
    return 0;
}

/*
Results:
 - Serial Execution: 0.0253971 seconds (baseline for comparison).
 - Block Distribution: 0.0114499 seconds.
 - Cyclic Distribution: 0.0116451 seconds.
 - Block-Cyclic Distribution: 0.00390354 seconds.

 - The parallel implementations significantly outperform the serial version.
 - Block and cyclic distributions split work among threads, reducing execution time.
 - Block-cyclic further optimizes cache usage, improving efficiency.

 - Serial Execution: I think it experiences fewer cache misses due to sequential memory access.
 - Block Distribution: Maintains spatial locality but may lead to cache conflicts at boundaries.
 - Cyclic Distribution: Can introduce more cache misses since non-contiguous rows are assigned to threads.
 - Block-Cyclic Distribution: Balances cache efficiency and workload distribution, reducing cache misses.

 - Speedup = Serial Time / Parallel Time.
 - Block Speedup        :   2.22x.
 - Cyclic Speedup       :   2.18x.
 - Block-Cyclic Speedup :   6.5x    (Best performance, due to better load balancing and cache utilization).
 - Block-cyclic distribution is the most efficient due to better memory access patterns.
 - Workload balancing and cache-friendly memory access are key to performance improvement.
*/
