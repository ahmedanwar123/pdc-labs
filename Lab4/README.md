# Lab 4: Matrix Multiplication with Transpose and AVX

Perform matrix multiplication (dot product) for two matrices $A$ and $B$, both of size $2048 \times 2048$:
- With transposing $B$
- With transposing $B$ and AVX intrinsics

To improve cache efficiency during matrix multiplication, the matrix $B$ is first transposed to obtain $B^T$, which enables row-wise access in both $A$ and $B^T$. The multiplication is then performed as:

$$
C[i][j] = \sum_{k=0}^{N-1} A[i][k] \cdot B^T[j][k]
$$

This improves spatial locality, reducing cache misses compared to accessing $B$ column-wise. The implemented function performs the standard nested loop multiplication with $B^T$.

- Size of matrices: $2048 \times 2048$
- Execution time observed: 22.50 seconds

## Matrix Multiplication with Transposed Matrix and AVX Optimization

To further accelerate the multiplication, SIMD instructions (AVX) are employed. The main idea is to exploit data-level parallelism by processing 8 floating-point elements simultaneously using 256-bit wide registers. The multiplication uses the fused multiply-add (FMA) instruction `_mm256_fmadd_ps` which computes:

$$
\text{sum_vec} = \text{sum_vec} + (a_vec \times b_vec)
$$

in a single instruction, reducing latency.

Key details:
- Load 8 contiguous elements of $A[i, k:k+7]$ and $B^T[j, k:k+7]$ with `_mm256_loadu_ps`
- Perform FMA (Fused Multiply Additions) operations on vectors (Perform multiply and addition in single step "single CPU Instruction")
- Accumulate partial sums horizontally after the loop to compute the final scalar product

Performance results:
- Execution time: 6.39 seconds
- Speedup over standard transpose multiplication: $\approx 3.52 \times$

## Summary

- Transposing matrix $B$ optimizes memory access patterns and cache utilization.
- AVX SIMD instructions leverage parallelism at the data level, processing 8 floats simultaneously and using FMA to reduce instruction count.
- Combining these optimizations achieves significant performance gains in large-scale matrix multiplication.