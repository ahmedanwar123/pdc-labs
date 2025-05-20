/*
 * Name: Ahmed Anwar Mazhar
 * ID: 120210007
 */

#include <iostream>
#include <vector>
#include <chrono>

#define TIMERSTART(label) auto start_##label = std::chrono::high_resolution_clock::now()
#define TIMERSTOP(label)                                                                                     \
    auto stop_##label = std::chrono::high_resolution_clock::now();                                           \
    std::cout << #label " took "                                                                             \
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop_##label - start_##label).count() \
              << " ms" << std::endl;

int main()
{
    const uint64_t size = 2048;
    std::vector<float> A(size * size, 1.0f);
    std::vector<float> B(size * size, 1.0f);
    std::vector<float> Bt(size * size, 0.0f);
    std::vector<float> C(size * size, 0.0f);

    // Without transposing B
    TIMERSTART(naive_mult);
    for (uint64_t i = 0; i < size; i++)
    {
        for (uint64_t j = 0; j < size; j++)
        {
            float accum = 0;
            for (uint64_t k = 0; k < size; k++)
            {
                // A[i * size + k] accesses row-wise (good for cache locality)
                // B[k * size + j] accesses column-wise (causing cache misses)
                accum += A[i * size + k] * B[k * size + j];
            }
            C[i * size + j] = accum;
        }
    }
    TIMERSTOP(naive_mult);
    // naive_mult took 155911 ms

    // Cache inefficiency: B[k * size + j] results in frequent cache misses since
    // elements are not accessed sequentially in memory

    // Transposing B
    TIMERSTART(transpose);
    for (uint64_t k = 0; k < size; k++)
    {
        for (uint64_t j = 0; j < size; j++)
        {
            // Store in row-major order to help in increaseing hit ratio
            Bt[j * size + k] = B[k * size + j];
        }
    }
    TIMERSTOP(transpose);
    // transpose took 107 ms
    // Transposing improves spatial locality by storing B in row-major order for efficient access.

    // Multiplication using Bt
    TIMERSTART(transpose_mult);
    for (uint64_t i = 0; i < size; i++)
    {
        for (uint64_t j = 0; j < size; j++)
        {
            float accum = 0;
            for (uint64_t k = 0; k < size; k++)
            {
                // Bt[j * size + k] ensures row-wise access, improving cache efficiency.
                accum += A[i * size + k] * Bt[j * size + k];
            }
            C[i * size + j] = accum;
        }
    }
    TIMERSTOP(transpose_mult);
    // transpose_mult took 32641 ms
    // Bt improves cache locality, reducing cache misses and improving performance.

    /* Speedup Calculation
     * naive_mult took 155911 ms
     * transpose took 107 ms
     * transpose_mult took 32641 ms
     * Speedup = naive_mult / transpose_mult = 155911 / 32641 ≈ 4.78x
     */

    return 0;
}
