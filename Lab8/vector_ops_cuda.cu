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
    std::cout << "Problem 1 (CUDA): First element of c: " << h_c << std::endl;

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
    blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    normalizeVectors<<<blocksPerGrid, threadsPerBlock>>>(d_vec, size);

    // Copy the first normalized vector back to host
    float4 h_vec;
    cudaMemcpy(&h_vec, d_vec, sizeof(float4), cudaMemcpyDeviceToHost);
    std::cout << "Problem 2 (CUDA): First normalized vector: " << h_vec.x << ", " << h_vec.y << ", " << h_vec.z << ", " << h_vec.w << std::endl;

    // Clean up
    cudaFree(d_vec);
    curandDestroyGenerator(gen);
}

int main()
{
    problem1_cuda();
    problem2_cuda();
    return 0;
}