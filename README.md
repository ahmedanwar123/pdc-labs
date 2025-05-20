
# CSE 425 Parallel & Distributed Computing Labs

This repository contains the lab assignments for the **CSE 425 Parallel & Distributed Computing** course. Each lab explores key concepts in parallel and distributed computing, focusing on performance optimization, parallelization strategies, and efficient memory access patterns. Below is an overview of each lab and its objectives.

## Lab Overviews

### Lab 1: Compute-to-Communication Ratio

* **Purpose** : Introduces the  **Compute-to-Communication Ratio (CCR)** , a metric for evaluating the balance between computation and communication in parallel systems.
* **What It Does** : Models speedup using Python, analyzing how CCR (via parameter $\gamma$) and parallel overhead ($q$) affect performance. Visualizes optimal speedup configurations with a 3D surface plot.
* **Key Concepts** : CCR, speedup modeling, parallel efficiency, visualization of performance metrics.

### Lab 2: Dependency of Scaled Efficiency

* **Purpose** : Explores how scaled efficiency varies with the number of processors and compute-to-communication ratio.
* **What It Does** : Analyzes efficiency ($E_\gamma(p)$) when $\gamma = p^\delta$, deriving iso-efficiency lines to understand how problem size must scale to maintain efficiency.
* **Key Concepts** : Scaled efficiency, iso-efficiency analysis, impact of processor scaling on performance.

### Lab 3: Matrix Multiplication

* **Purpose** : Demonstrates the impact of cache-aware memory access on matrix multiplication performance.
* **What It Does** : Compares naive matrix multiplication (2048x2048 matrices) with an optimized version that transposes matrix $B$ to improve cache locality, achieving ~4.78x speedup.
* **Key Concepts** : Cache efficiency, matrix transposition, performance optimization.

### Lab 4: Matrix Multiplication with Transpose and AVX

* **Purpose** : Enhances matrix multiplication performance using transposition and SIMD instructions.
* **What It Does** : Builds on Lab 3 by adding AVX intrinsics to process 8 floats simultaneously with fused multiply-add operations, reducing execution time to 6.39 seconds (~3.52x speedup over transpose-only).
* **Key Concepts** : SIMD, AVX, fused multiply-add, cache optimization.

### Lab 5: Matrix Subtraction

* **Purpose** : Compares serial and parallel matrix subtraction using different work distribution strategies.
* **What It Does** : Implements matrix subtraction (2048x2048) serially and in parallel using Pthreads with block, cyclic, and block-cyclic distributions. Block-cyclic distribution achieves the best speedup (~6.5x).
* **Key Concepts** : Pthreads, work distribution, load balancing, cache efficiency.

### Lab 6: Riemann Zeta Problem

* **Purpose** : Explores parallelization of a computationally intensive problem using static and dynamic scheduling.
* **What It Does** : Computes a variant of the Riemann Zeta function with double sums using static and dynamic block-cyclic scheduling. Dynamic scheduling slightly outperforms static due to better load balancing.
* **Key Concepts** : Riemann Zeta function, static vs. dynamic scheduling, load balancing.

### Lab 7: Dense Matrix Multiplication and Pseudo-Polynomial Knapsack

* **Purpose** : Compares sequential and parallel implementations of two problems using OpenMP.
* **What It Does** : Implements dense matrix multiplication (256x256) and the 0/1 knapsack problem. Matrix multiplication achieves ~3.69x speedup with OpenMP, while knapsack suffers from parallelization overhead due to data dependencies.
* **Key Concepts** : OpenMP, data dependencies, parallel overhead, dynamic programming.

### Lab 8: Vector Operations

* **Purpose** : Demonstrates sequential and GPU-accelerated vector operations.
* **What It Does** : Performs vector addition ($2^{24}$ elements) and 4D vector normalization ($2^{22}$ vectors) sequentially (CPU) and in parallel (GPU with CUDA). GPU implementations reduce execution times to ~3-4 ms from thousands of ms.
* **Key Concepts** : CUDA, GPU parallelization, vector operations, random number generation.

## Getting Started

To explore the labs:

1. Clone this repository: `git clone <repository-url>`
2. Navigate to each lab's Markdown file (e.g., `Lab1-CCR.md`) for detailed explanations and results.
3. Use a Markdown viewer with MathJax support to render equations properly.

Each lab includes performance results, mathematical formulations, and insights into parallel computing techniques, making this repository a valuable resource for understanding parallel and distributed systems.
