#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>

constexpr int N = 2048;

// Function to transpose a matrix
void transposeMatrix(const float *B, float *B_T, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            B_T[j * n + i] = B[i * n + j];
        }
    }
}

// Matrix multiplication with transposed B
void matMulTransposed(const float *A, const float *B_T, float *C, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            float sum = 0;
            for (int k = 0; k < n; k++)
            {
                sum += A[i * n + k] * B_T[j * n + k];
            }
            C[i * n + j] = sum;
        }
    }
}
// Time teaken by Matrix multiplication: 22.5025 Sec

// Matrix multiplication using AVX with transposed B
void matMulTransposedAVX(const float *A, const float *B_T, float *C, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            __m256 sum_vec = _mm256_setzero_ps();
            for (int k = 0; k < n; k += 8)
            {
                __m256 a_vec = _mm256_loadu_ps(&A[i * n + k]);
                __m256 b_vec = _mm256_loadu_ps(&B_T[j * n + k]);
                sum_vec = _mm256_fmadd_ps(a_vec, b_vec, sum_vec);
            }
            float sum[8];
            _mm256_storeu_ps(sum, sum_vec);
            C[i * n + j] = sum[0] + sum[1] + sum[2] + sum[3] +
                           sum[4] + sum[5] + sum[6] + sum[7];
        }
    }
}
// Time taken by AVX multiplication: 6.38833 Sec

int main()
{
    std::ios::sync_with_stdio(false); // Disable I/O synchronization for potential speedup
    std::vector<float> A(N * N), B(N * N), B_T(N * N), C1(N * N, 0), C2(N * N, 0);

    // Initialize A and B with random values
    for (int i = 0; i < N * N; i++)
    {
        A[i] = static_cast<float>(rand()) / RAND_MAX;
        B[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    // Transpose matrix B
    transposeMatrix(B.data(), B_T.data(), N);

    // Measure execution time for standard multiplication
    auto start1 = std::chrono::high_resolution_clock::now();
    matMulTransposed(A.data(), B_T.data(), C1.data(), N);
    auto end1 = std::chrono::high_resolution_clock::now();
    double time1 = std::chrono::duration<double>(end1 - start1).count();

    // Measure execution time for AVX-optimized multiplication
    auto start2 = std::chrono::high_resolution_clock::now();
    matMulTransposedAVX(A.data(), B_T.data(), C2.data(), N);
    auto end2 = std::chrono::high_resolution_clock::now();
    double time2 = std::chrono::duration<double>(end2 - start2).count();

    std::cout << "Matrix Multiplication with B Transposed (Standard): " << time1 << " seconds\n"
              << std::flush;
    std::cout << "Matrix Multiplication with B Transposed & AVX: " << time2 << " seconds\n"
              << std::flush;

    return 0;
}

/*
Speedup Calculation:
Speedup = Time for Standard / Time for AVX
        = 22.5025 Sec / 6.38833 Sec
        ≈ 3.52
- Better cache utilization from transposed matrix reduces cache misses.
- The AVX version is ~3.52× faster due to SIMD parallelism and FMA instructions,
While without AVX and FMA Computes one element    at a time; AVX Processes 8 elements in parallel & reduces memory latency using '_mm256_loadu_ps' and fuses operations with '_mm256_fmadd_ps'.

*/