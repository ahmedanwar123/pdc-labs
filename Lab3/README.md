# Lab 3: Matrix Multiplication

Perform matrix multiplication (dot product) for two matrices $A$ and $B$, both of size $2048 \times 2048$:
- Without transposing $B$
- With transposing $B$

This experiment demonstrates the impact of **cache-aware memory access patterns** on matrix multiplication performance. It compares a naive implementation with an optimized version that transposes one of the matrices to improve cache locality.

## Matrix Setup

Four matrices of size $2048 \times 2048$ are used:
- $A$: Input matrix, initialized with 1.0
- $B$: Input matrix, initialized with 1.0
- $B^T$: Transposed version of $B$
- $C$: Output matrix

All matrices are stored in a 1D `std::vector<float>` using **row-major order**.

## Naive Matrix Multiplication

The naive approach performs multiplication as:

$$
C[i][j] = \sum_{k=0}^{n-1} A[i][k] \cdot B[k][j]
$$

In C++ (row-major layout), this results in:
- `A[i * size + k]` --- accessed row-wise (cache-friendly)
- `B[k * size + j]` --- accessed column-wise (cache-unfriendly)

This causes **frequent cache misses** since elements of $B$ are accessed in a non-contiguous manner.

## Execution Time

> `naive_mult` took 155911 ms

## Matrix Transposition

To improve performance, we transpose $B$ to get $B^T$ such that:

$$
B^T[j][k] = B[k][j]
$$

This stores the original columns of $B$ as rows in $B^T$, which improves spatial locality.

## Execution Time

> `transpose` took 107 ms

## Optimized Matrix Multiplication

After transposing $B$, multiplication becomes:

$$
C[i][j] = \sum_{k=0}^{n-1} A[i][k] \cdot B^T[j][k]
$$

Access patterns:
- `A[i * size + k]` --- row-wise
- `Bt[j * size + k]` --- also row-wise

Now, both operands are accessed sequentially, significantly reducing cache misses.

## Speedup Analysis

$$
\text{Speedup} = \frac{T_{\text{naive}}}{T_{\text{optimized}}} = \frac{155911}{32641} \approx 4.78\times
$$

Even including the transposition time, this represents a significant performance gain.