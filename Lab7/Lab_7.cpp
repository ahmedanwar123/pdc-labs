/*
 * Name  : Ahmed Anwar Mazhar
 * ID    : 120210007
 * Lab 7
 */
#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <random>

// Dense Matrix Multiplication (a)
void matrix_multiply_sequential(double *A, double *B, double *C, int M, int L, int N)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double sum = 0.0;
            for (int k = 0; k < L; k++)
            {
                sum += A[i * L + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Parallel Matrix Multiplication using OpenMP
void matrix_multiply_openmp(double *A, double *B, double *C, int M, int L, int N)
{
// Parallelization strategy:
// - Parallelize the outer loop over i (rows of A) to distribute rows across threads.
// - Each thread computes C[i*N+j] for all j, ensuring no data races since each C[i*N+j] is independent.
// - Use dynamic scheduling to handle potential load imbalances, though workload is uniform for square matrices.
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double sum = 0.0;
            for (int k = 0; k < L; k++)
            {
                sum += A[i * L + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Pseudo-Polynomial Knapsack (b)
#define AT(i, j, C) ((i) * (C + 1) + (j))
#define MAX(x, y) ((x) < (y) ? (y) : (x))
void knapsack_sequential(const int *w, const int *v, int *m, int N, int C)
{
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 0; j < C + 1; j++)
        {
            if (w[i - 1] <= j)
            {
                m[AT(i, j, C)] = MAX(m[AT(i - 1, j, C)], m[AT(i - 1, j - w[i - 1], C)] + v[i - 1]);
            }
            else
            {
                m[AT(i, j, C)] = m[AT(i - 1, j, C)];
            }
        }
    }
}

// Parallel Knapsack using OpenMP
void knapsack_openmp(const int *w, const int *v, int *m, int N, int C)
{
    // Parallelization strategy:
    // - Parallelize the inner loop over j (capacity) for each item i.
    // - Computations for different j are independent within the same i, allowing parallel execution.
    // - Use static scheduling since workload is uniform across j.
    // - Outer loop (i) cannot be parallelized due to dependencies between consecutive i iterations.
    for (int i = 1; i < N + 1; i++)
    {
#pragma omp parallel for schedule(static)
        for (int j = 0; j < C + 1; j++)
        {
            if (w[i - 1] <= j)
            {
                m[AT(i, j, C)] = MAX(m[AT(i - 1, j, C)], m[AT(i - 1, j - w[i - 1], C)] + v[i - 1]);
            }
            else
            {
                m[AT(i, j, C)] = m[AT(i - 1, j, C)];
            }
        }
    }
}

int main()
{
    // Initialize random numbers for matrix and knapsack inputs
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);
    std::uniform_int_distribution<> int_dis(1, 100);

    // (a) Matrix Multiplication: N = M = L = 256
    const int M = 256, L = 256, N = 256;
    std::vector<double> A(M * L), B(L * N), C_seq(M * N), C_omp(M * N);

    // Initialize matrices A and B with random values
    for (int i = 0; i < M * L; ++i)
        A[i] = dis(gen);
    for (int i = 0; i < L * N; ++i)
        B[i] = dis(gen);

    // Sequential Matrix Multiplication
    auto start_seq = std::chrono::high_resolution_clock::now();
    matrix_multiply_sequential(A.data(), B.data(), C_seq.data(), M, L, N);
    auto end_seq = std::chrono::high_resolution_clock::now();
    double time_seq = std::chrono::duration<double>(end_seq - start_seq).count();
    std::cout << "Sequential Matrix Multiplication Time: " << time_seq << " seconds\n";

    // OpenMP Matrix Multiplication
    auto start_omp = std::chrono::high_resolution_clock::now();
    matrix_multiply_openmp(A.data(), B.data(), C_omp.data(), M, L, N);
    auto end_omp = std::chrono::high_resolution_clock::now();
    double time_omp = std::chrono::duration<double>(end_omp - start_omp).count();
    std::cout << "OpenMP Matrix Multiplication Time: " << time_omp << " seconds\n";

    // (b) Knapsack: N = C = 1024
    const int K_N = 1024, K_C = 1024;
    std::vector<int> w(K_N), v(K_N), m_seq((K_N + 1) * (K_C + 1), 0), m_omp((K_N + 1) * (K_C + 1), 0);
    for (int i = 0; i < K_N; ++i)
    {
        w[i] = int_dis(gen);
        v[i] = int_dis(gen);
    }

    // Sequential Knapsack
    start_seq = std::chrono::high_resolution_clock::now();
    knapsack_sequential(w.data(), v.data(), m_seq.data(), K_N, K_C);
    end_seq = std::chrono::high_resolution_clock::now();
    time_seq = std::chrono::duration<double>(end_seq - start_seq).count();
    std::cout << "Sequential Knapsack Result: " << m_seq[AT(K_N, K_C, K_C)] << "\n";
    std::cout << "Sequential Knapsack Time: " << time_seq << " seconds\n";

    // OpenMP Knapsack
    start_omp = std::chrono::high_resolution_clock::now();
    knapsack_openmp(w.data(), v.data(), m_omp.data(), K_N, K_C);
    end_omp = std::chrono::high_resolution_clock::now();
    time_omp = std::chrono::duration<double>(end_omp - start_omp).count();
    std::cout << "OpenMP Knapsack Result: " << m_omp[AT(K_N, K_C, K_C)] << "\n";
    std::cout << "OpenMP Knapsack Time: " << time_omp << " seconds\n";

    return 0;
}

/*
Execution Time Results and Parallelization Strategy Discussion:

Dense Matrix Multiplication (N = M = L = 256):
- Sequential Time: 0.0504608 seconds
- OpenMP Time: 0.0136851 seconds
- Speedup: ~3.69x (0.0504608 / 0.0136851)
- Parallelization Strategy: The outer loop over rows (i) is parallelized, distributing rows across threads.
  Each thread computes C[i*N+j] for all j, avoiding data races as each element is independent.
  Dynamic scheduling balances workload, though the uniform workload (square matrices) minimizes imbalances.
  The ~3.69x speedup indicates effective parallelization, leveraging multiple cores (likely 4–8).
  The algorithm is embarrassingly parallel, with minimal overhead, explaining the strong performance.

Pseudo-Polynomial Knapsack (N = C = 1024):
- Sequential Time: 0.00579259 seconds
- OpenMP Time: 0.0817717 seconds
- Speedup: ~0.071x (0.00579259 / 0.0817717, indicating a slowdown)
- Parallelization Strategy: The inner loop over capacity (j) is parallelized for each item i, as computations
  for different j are independent within the same i. Static scheduling is used due to uniform workload.
  The outer loop (i) cannot be parallelized due to dependencies between consecutive i iterations.
  The significant slowdown (~14x) is likely due to high OpenMP overhead (thread creation and synchronization)
  and non-contiguous memory accesses (m[AT(i-1,j-w[i-1])]), causing cache misses or false sharing.
  The small problem size (N = C = 1024) and low computational intensity make parallelization inefficient.
  Potential improvements include increasing problem size or optimizing memory access patterns.
  */