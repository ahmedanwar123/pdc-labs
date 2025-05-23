#include <cuda_runtime.h>
#include <curand.h>
#include <vector_types.h>
#include <iostream>

// Problem 1: Kernel to add two vectors
__global__ void addVectors(float *a, float *b, float *c, size_t size)
{
    size_t id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id < size)
    {
        c[id] = a[id] + b[id];
    }
}

// Kernel to scale random numbers to [-1, 1]
__global__ void scaleKernel(float *data, size_t size, float scale, float shift)
{
    size_t id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id < size)
    {
        data[id] = data[id] * scale + shift;
    }
}

void problem1_cuda()
{
    const size_t size = 1 << 24; // 2^24
    float *d_a, *d_b, *d_c;

    // Allocate device memory
    cudaMalloc(&d_a, size * sizeof(float));
    cudaMalloc(&d_b, size * sizeof(float));
    cudaMalloc(&d_c, size * sizeof(float));

    // Initialize CURAND generator
    curandGenerator_t gen;
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(gen, 1234ULL);

    // Generate random numbers in [0, 1) and scale to [-1, 1]
    // CURAND generates random floats between 0 and 1. We then scale the values to be between -1 and 1.
    curandGenerateUniform(gen, d_a, size);
    curandGenerateUniform(gen, d_b, size);

    // Scale to [-1, 1]
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    scaleKernel<<<blocksPerGrid, threadsPerBlock>>>(d_a, size, 2.0f, -1.0f);
    scaleKernel<<<blocksPerGrid, threadsPerBlock>>>(d_b, size, 2.0f, -1.0f);

    // Add vectors
    addVectors<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, size);

    // Copy the first element of c back to host
    float h_c;
    cudaMemcpy(&h_c, d_c, sizeof(float), cudaMemcpyDeviceToHost);
    std::cout << "Problem 1: First element of c: " << h_c << std::endl;

    // Clean up
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    curandDestroyGenerator(gen);
}

// Problem 2: Kernel to normalize 4D vectors
__global__ void normalizeVectors(float4 *vec, size_t size)
{
    size_t id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id < size)
    {
        float4 v = vec[id];
        float norm = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
        if (norm > 0.0f)
        {
            vec[id].x /= norm;
            vec[id].y /= norm;
            vec[id].z /= norm;
            vec[id].w /= norm;
        }
    }
}

void problem2_cuda()
{
    const size_t size = 1 << 22; // 2^22
    float4 *d_vec;

    // Allocate device memory
    cudaMalloc(&d_vec, size * sizeof(float4));

    // Initialize CURAND generator
    curandGenerator_t gen;
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(gen, 1234ULL);

    // Generate random numbers into a flat float array
    float *d_data;
    cudaMalloc(&d_data, 4 * size * sizeof(float));
    curandGenerateUniform(gen, d_data, 4 * size);

    // Scale to [-1, 1]
    int threadsPerBlock = 256;
    int blocksPerGrid = (4 * size + threadsPerBlock - 1) / threadsPerBlock;
    scaleKernel<<<blocksPerGrid, threadsPerBlock>>>(d_data, 4 * size, 2.0f, -1.0f);

    // Copy to float4 array
    cudaMemcpy(d_vec, d_data, size * sizeof(float4), cudaMemcpyDeviceToDevice);
    cudaFree(d_data);

    // Normalize the vectors
    // Each thread computes the norm of a 4D vector and normalizes it. This is a parallel operation that benefits from the GPU’s many cores.
    blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    normalizeVectors<<<blocksPerGrid, threadsPerBlock>>>(d_vec, size);

    // Copy the first normalized vector back to host
    float4 h_vec;
    cudaMemcpy(&h_vec, d_vec, sizeof(float4), cudaMemcpyDeviceToHost);
    std::cout << "Problem 2: First normalized vector: " << h_vec.x << ", " << h_vec.y << ", " << h_vec.z << ", " << h_vec.w << std::endl;

    // Clean up
    cudaFree(d_vec);
    curandDestroyGenerator(gen);
}
void measureCuda(void (*func)(), const std::string &label)
{
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
    func();
    cudaEventRecord(stop);

    cudaEventSynchronize(stop);
    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);

    std::cout << label << " execution time: " << ms << " ms\n";

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
}

int main()
{
    problem1_cuda();
    problem2_cuda();
    measureCuda(problem1_cuda, "Problem 1");
    measureCuda(problem2_cuda, "Problem 2");
    return 0;
}
/*
 * Problem 1 execution time: 3.9095 ms
 * Problem 2 execution time: 3.15635 ms
 *  The execution time on the GPU is in the millisecond range for both problems, showcasing the speed-up when using a GPU compared to sequential CPU execution.
 * The GPU handles the problem efficiently by utilizing thousands of threads to compute the vector operations concurrently.

 */