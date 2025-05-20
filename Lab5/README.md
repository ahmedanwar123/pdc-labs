# Lab 5: Matrix Subtraction

Perform matrix subtraction $C = A - B$ for two matrices $A$ and $B$ with sizes $2048 \times 2048$ each. Compare the run times of:
- Serial matrix subtraction
- Parallel matrix subtraction using Pthreads with:
  1. Block distribution
  2. Cyclic distribution
  3. Block-cyclic distribution using 8 threads

## Overview

This code performs matrix subtraction $C = A - B$ for two $2048 \times 2048$ matrices using both serial and parallel methods. The parallel implementations leverage POSIX threads (Pthreads) with different work distribution strategies:
- Block Distribution
- Cyclic Distribution
- Block-Cyclic Distribution

All parallel versions use 8 threads.

## Serial Matrix Subtraction

The serial method iterates through each element $(i,j)$ and performs:

$$
C[i][j] = A[i][j] - B[i][j]
$$

This serves as the baseline for performance comparison.

## Parallel Matrix Subtraction with Pthreads

Each parallel method divides the workload of matrix rows among 8 threads differently:

### 1. Block Distribution

Each thread is assigned a contiguous block of rows:

$$
\text{Thread } t: \quad \text{rows } t \times \frac{N}{T} \text{ to } (t+1) \times \frac{N}{T} - 1
$$

where $N=2048$ is matrix size and $T=8$ is the number of threads. This maintains spatial locality but can cause cache conflicts at block boundaries.

### 2. Cyclic Distribution

Rows are distributed in a round-robin fashion among threads:

$$
\text{Thread } t: \quad \{ i \mid i \equiv t \pmod{T} \}
$$

This may reduce load imbalance but can increase cache misses due to non-contiguous access.

### 3. Block-Cyclic Distribution

Each thread processes blocks of rows cyclically:

$$
\text{Thread } t: \quad \text{rows } t \times b \text{ to } t \times b + b - 1, \quad t \times b + k \times T \times b
$$

with block size $b = \frac{N}{T}$. This balances workload and improves cache efficiency by combining block and cyclic approaches.

## Performance Results

| Method                     | Execution Time (seconds) |
|----------------------------|--------------------------|
| Serial                     | 0.0254                   |
| Block Distribution         | 0.0114                   |
| Cyclic Distribution        | 0.0116                   |
| Block-Cyclic Distribution  | 0.0039                   |

## Speedup Analysis

Speedup is computed as:

$$
\text{Speedup} = \frac{\text{Serial Time}}{\text{Parallel Time}}
$$

- Block Distribution: $\approx 2.22 \times$
- Cyclic Distribution: $\approx 2.18 \times$
- Block-Cyclic Distribution: $\approx 6.5 \times$ (best)

## Discussion

- **Serial execution** benefits from sequential memory access, resulting in fewer cache misses.
- **Block distribution** preserves spatial locality but may suffer from cache conflicts at block boundaries.
- **Cyclic distribution** distributes workload evenly but increases cache misses due to scattered memory access.
- **Block-cyclic distribution** strikes a balance between load balancing and cache efficiency, resulting in the best performance.

Effective parallelization depends on balancing workload across threads and optimizing memory access patterns to reduce cache misses. The block-cyclic approach achieves this balance, leading to significantly improved execution times.