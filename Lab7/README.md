# Lab 7: Dense Matrix Multiplication and Pseudo-Polynomial Knapsack Using Dynamic Programming with OpenMP

## Problem Overview

This code implements and compares sequential and parallel solutions for two classic computational problems:

- **Dense Matrix Multiplication:** Multiply two dense matrices $A \in \mathbb{R}^{M \times L}$ and $B \in \mathbb{R}^{L \times N}$ to produce matrix $C \in \mathbb{R}^{M \times N}$ such that

$$
C[i,j] = \sum_{k=0}^{L-1} A[i,k] \times B[k,j]
$$

- **Pseudo-Polynomial Knapsack Problem:** Solve the 0/1 knapsack problem for $N$ items and capacity $C$ using dynamic programming. The DP relation is

$$
m[i,j] = 
\begin{cases}
\max(m[i-1,j], m[i-1,j - w_i] + v_i), & \text{if } w_i \leq j \\
m[i-1,j], & \text{otherwise}
\end{cases}
$$

where $m[i,j]$ is the maximum value achievable with the first $i$ items and capacity $j$.

## Parallelization Using OpenMP

### Dense Matrix Multiplication

- **Strategy:** Parallelize the outer loop over rows $i$ of matrix $A$. Each thread computes all columns $j$ for its assigned row independently.
- **Data Race Avoidance:** Each thread writes to distinct parts of $C$ (rows), eliminating data races.
- **Scheduling:** Dynamic scheduling is used to balance load, although workload is uniform in this case.
- **Performance:** The parallel version achieves significant speedup (~3.69x) over sequential, demonstrating effective multi-core utilization and embarrassingly parallel nature.

### Pseudo-Polynomial Knapsack

- **Strategy:** Parallelize the inner loop over capacities $j$ for each fixed item $i$.
- **Dependencies:** Outer loop over $i$ remains sequential due to dependency on previous row $m[i-1,*]$.
- **Scheduling:** Static scheduling applied as workload across capacities is uniform.
- **Performance:** Parallelization results in a slowdown compared to sequential due to:
  - High overhead from thread management in OpenMP relative to small computational work per iteration.
  - Poor memory access locality (non-contiguous memory accesses for DP table), increasing cache misses.
  - Potential false sharing and synchronization overhead.
- **Implication:** The low computational intensity and data dependencies make OpenMP parallelization inefficient for this problem size.

## Summary of Execution Times

| Problem                           | Sequential Time (s) | OpenMP Time (s) |
|-----------------------------------|---------------------|-----------------|
| Dense Matrix Multiplication (256 x 256) | 0.0505        | 0.0137          |
| Pseudo-Polynomial Knapsack (N=1024, C=1024) | 0.0058 | 0.0818          |

## Conclusions

- Dense matrix multiplication benefits strongly from OpenMP parallelization due to independent computations and large computational workload per iteration.
- The pseudo-polynomial knapsack problem exhibits inherent data dependencies and irregular memory access patterns, limiting parallel performance.
- Effective parallelization requires balancing computation granularity, data locality, and synchronization overhead.
- For small to medium problem sizes with dependencies, parallel overheads can outweigh benefits, emphasizing the need for careful analysis before parallelizing.