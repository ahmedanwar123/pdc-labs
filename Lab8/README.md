# Lab 8: Vector Operations

1. Create two vectors each of size $2^{24}$ with random values, then add them.
2. Create a vector of 4-dimensional vectors with $2^{22}$ elements and normalize each 4-dimensional vector.

## Sequential Vector Addition and 4D Vector Normalization

This subsection presents two fundamental vector operations implemented sequentially in C++ and measured for CPU execution time.

### Problem 1: Vector Addition

We add two vectors $\mathbf{a}$ and $\mathbf{b}$ each of size $2^{24}$. The elements of both vectors are initialized with random floating-point values uniformly distributed in $[-1, 1]$. The result vector $\mathbf{c}$ stores the element-wise sum:

$$
c_i = a_i + b_i, \quad \text{for } i=0, \ldots, 2^{24}-1.
$$

This addition is performed sequentially, processing one element at a time on the CPU. The computational complexity is $O(N)$, where $N = 2^{24}$.

### Problem 2: 4D Vector Normalization

Given $2^{22}$ four-dimensional vectors $\mathbf{v}_i = (x_i, y_i, z_i, w_i)$, each vector is normalized by its Euclidean norm:

$$
\mathbf{v}_i \leftarrow \frac{\mathbf{v}_i}{|\mathbf{v}_i|}, \quad \text{where} \quad |\mathbf{v}_i| = \sqrt{x_i^2 + y_i^2 + z_i^2 + w_i^2}.
$$

Vectors are initialized with random values in $[-1, 1]$, and normalization is applied sequentially. The algorithm involves a square root and four divisions per vector, making it computationally intensive for the large dataset.

### Performance Measurements

Execution times were recorded on a single CPU core:

- Problem 1 (Vector Addition): Approximately 4891.6 ms
- Problem 2 (4D Vector Normalization): Approximately 2475.75 ms

These times highlight the cost of sequential processing on large data sizes. Both problems are embarrassingly parallel, suggesting significant performance improvements through parallelization or vectorization.

### Code Implementation Highlights

- Random number generation uses the Mersenne Twister engine `std::mt19937` with uniform distribution.
- Sequential loops iterate over large vectors to perform addition or normalization.
- For normalization, a conditional check avoids division by zero.
- CPU execution time is measured with `std::chrono::high_resolution_clock`.

## GPU-Accelerated Vector Addition and 4D Vector Normalization

This subsection demonstrates parallel implementations of the vector operations from the previous subsection using CUDA, highlighting the performance gains from GPU acceleration.

### Problem 1: Parallel Vector Addition on GPU

Two vectors $\mathbf{a}$ and $\mathbf{b}$ of size $2^{24}$ are initialized on the GPU with random values scaled to the range $[-1, 1]$. Using the CUDA kernel `addVectors`, each thread performs one element-wise addition:

$$
c_i = a_i + b_i, \quad i=0, \ldots, 2^{24}-1,
$$

where thousands of threads run concurrently, processing elements in parallel.

Random number generation uses the CUDA CURAND library, which generates uniform floats in $[0,1)$. A dedicated kernel `scaleKernel` then scales these to $[-1,1]$.

### Problem 2: Parallel Normalization of 4D Vectors on GPU

A large array of $2^{22}$ four-dimensional vectors $\mathbf{v}_i$ is initialized similarly on the GPU. The kernel `normalizeVectors` launches one thread per vector, computing the Euclidean norm and normalizing each vector in parallel:

$$
\mathbf{v}_i \leftarrow \frac{\mathbf{v}_i}{|\mathbf{v}_i|}.
$$

Data transfer and memory allocation are handled explicitly with `cudaMalloc`, `cudaMemcpy`, and CURAND calls for random number generation.

### Performance Results

GPU execution times are drastically reduced compared to the CPU baseline:

- Problem 1 (Vector Addition): $\approx 3.91$ ms
- Problem 2 (4D Vector Normalization): $\approx 3.16$ ms

This speed-up, from thousands of milliseconds on the CPU to single-digit milliseconds on the GPU, is due to massive parallelism—thousands of CUDA threads perform computations concurrently.

### Implementation Details

- CUDA kernels map each element or vector to a unique thread using `blockIdx`, `blockDim`, and `threadIdx`.
- CURAND is used for efficient parallel random number generation directly on the GPU.
- Data scaling and normalization are performed with simple CUDA kernels to exploit data parallelism.
- Timing is measured with CUDA events (`cudaEventRecord`) for precise GPU timing.

### Summary

This subsection illustrates how data-parallel tasks such as vector addition and normalization benefit from GPU acceleration by dividing work across thousands of threads. The explicit memory management and kernel design ensure efficient utilization of GPU resources, achieving a significant performance gain over the sequential CPU approach.

## Requirements

- NVIDIA GPU with CUDA support
- CUDA Toolkit installed (`nvcc`, `curand` libraries)

## Compilation

To compile the CUDA program:

```bash
nvcc -o vector_ops_cuda vector_ops_cuda.cu -lcurand
```
