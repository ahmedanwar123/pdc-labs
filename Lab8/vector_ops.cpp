#include <vector>
#include <random>
#include <iostream>
#include <chrono>

// Problem 1: Add two vectors of size 2^24
void problem1_cpp()
{
    const size_t size = 1 << 24; // 2^24
    std::vector<float> a(size);
    std::vector<float> b(size);
    std::vector<float> c(size);

    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Fill vectors a and b with random values
    for (size_t i = 0; i < size; ++i)
    {
        a[i] = dis(gen);
        b[i] = dis(gen);
    }

    // Add vectors a and b element-wise into c, performed sequentially in a loop.
    // On a single-core CPU, this operation will take time proportional to the problem size.
    // The CPU will process one element at a time.

    // Add vectors a and b element-wise into c
    for (size_t i = 0; i < size; ++i)
    {
        c[i] = a[i] + b[i];
    }

    // Print the first element of c
    std::cout << "Problem 1: First element of c: " << c[0] << std::endl;
}

// Problem 2: Normalize 2^22 4D vectors
struct Vec4
{
    float x, y, z, w;
};

void problem2_cpp()
{
    const size_t size = 1 << 22; // 2^22
    std::vector<Vec4> vec(size);

    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    // Fill the vector with random 4D vectors and normalize each
    for (size_t i = 0; i < size; ++i)
    {
        vec[i].x = dis(gen);
        vec[i].y = dis(gen);
        vec[i].z = dis(gen);
        vec[i].w = dis(gen);

        // Compute the Euclidean norm and normalize each vector, computation for each vector is done sequentially in a loop.
        // This is a computationally expensive on the CPU because the task is done element by element.

        // Compute the Euclidean norm
        float norm = std::sqrt(vec[i].x * vec[i].x + vec[i].y * vec[i].y + vec[i].z * vec[i].z + vec[i].w * vec[i].w);

        // Normalize if the norm is non-zero
        if (norm > 0.0f)
        {
            vec[i].x /= norm;
            vec[i].y /= norm;
            vec[i].z /= norm;
            vec[i].w /= norm;
        }
    }

    // Print the first normalized vector
    std::cout << "Problem 2: First normalized vector: " << vec[0].x << ", " << vec[0].y << ", " << vec[0].z << ", " << vec[0].w << std::endl;
}

// Measure CPU time for a function
void measureCPU(void (*func)(), const std::string &label)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << label << " execution time: " << duration.count() << " ms\n";
}
int main()
{
    problem1_cpp();
    problem2_cpp();
    measureCPU(problem1_cpp, "Problem 1");
    measureCPU(problem2_cpp, "Problem 2");
    return 0;
}

/*
 * Problem 1 execution time: 4891.6 ms
 * Problem 2 execution time: 2475.75 ms
 */