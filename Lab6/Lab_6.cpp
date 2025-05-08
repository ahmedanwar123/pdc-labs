#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <atomic>
#include <iomanip>
#include <functional>

// Function to calculate Riemann Zeta function for a specific k
double Riemann_Zeta(double s, uint64_t k) {
    double result = 0.0;
    for (uint64_t i = 1; i < k; i++)
        for (uint64_t j = 1; j < k; j++)
            result += (2*(i&1)-1)/pow(i+j, s);
    return result*pow(2, s);
}

// Static block-cyclic calculation
void static_block_cyclic(std::vector<double>& X, double s, uint64_t n, uint64_t thread_id, 
                         uint64_t num_threads, uint64_t chunk_size) {
    for (uint64_t k = thread_id * chunk_size; k < n; k += num_threads * chunk_size) {
        uint64_t end = std::min(k + chunk_size, n);
        for (uint64_t i = k; i < end; i++) {
            X[i] = Riemann_Zeta(s, i);
        }
    }
}

// Dynamic block-cyclic calculation using atomic counter
void dynamic_block_cyclic(std::vector<double>& X, double s, uint64_t n, 
                         std::atomic<uint64_t>& counter, uint64_t chunk_size) {
    uint64_t k;
    while ((k = counter.fetch_add(chunk_size)) < n) {
        uint64_t end = std::min(k + chunk_size, n);
        for (uint64_t i = k; i < end; i++) {
            X[i] = Riemann_Zeta(s, i);
        }
    }
}

// Function to run tests with different chunk sizes and methods
void run_test(uint64_t n, uint64_t num_threads, const std::vector<uint64_t>& chunk_sizes) {
    const double s = 2.0; // Using s=2 as in the problem (pi^2/6)
    
    std::cout << "Running tests with n = " << n << " and " << num_threads << " threads" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Chunk Size | Static Block-Cyclic | Dynamic Block-Cyclic |" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    
    for (uint64_t chunk_size : chunk_sizes) {
        // Test with static block-cyclic
        std::vector<double> X_static(n, 0.0);
        std::vector<std::thread> threads_static;
        
        auto start_static = std::chrono::high_resolution_clock::now();
        
        for (uint64_t i = 0; i < num_threads; i++) {
            threads_static.emplace_back(static_block_cyclic, std::ref(X_static), s, n, i, num_threads, chunk_size);
        }
        
        for (auto& thread : threads_static) {
            thread.join();
        }
        
        auto end_static = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_static = end_static - start_static;
        
        // Test with dynamic block-cyclic
        std::vector<double> X_dynamic(n, 0.0);
        std::vector<std::thread> threads_dynamic;
        std::atomic<uint64_t> counter(0);
        
        auto start_dynamic = std::chrono::high_resolution_clock::now();
        
        for (uint64_t i = 0; i < num_threads; i++) {
            threads_dynamic.emplace_back(dynamic_block_cyclic, std::ref(X_dynamic), s, n, std::ref(counter), chunk_size);
        }
        
        for (auto& thread : threads_dynamic) {
            thread.join();
        }
        
        auto end_dynamic = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_dynamic = end_dynamic - start_dynamic;
        
        // Print results
        std::cout << "| " << std::setw(10) << chunk_size << " | " 
                  << std::setw(19) << elapsed_static.count() << "s | " 
                  << std::setw(19) << elapsed_dynamic.count() << "s |" << std::endl;
        
        // Verify that both methods produce the same results
        bool results_match = true;
        for (uint64_t i = 0; i < n; i++) {
            if (std::abs(X_static[i] - X_dynamic[i]) > 1e-10) {
                results_match = false;
                break;
            }
        }
        
        if (!results_match) {
            std::cout << "WARNING: Results do not match for chunk_size = " << chunk_size << std::endl;
        }
    }
    
    std::cout << "-----------------------------------------------------------" << std::endl;
}

int main() {
    const uint64_t n = 2048;
    const uint64_t num_threads = 8;
    std::vector<uint64_t> chunk_sizes = {1, 2, 4, 8};
    
    std::cout << "Solving Riemann Zeta Problem (n = " << n << ")" << std::endl;
    std::cout << "Number of threads: " << num_threads << std::endl;
    std::cout << std::endl;
    
    run_test(n, num_threads, chunk_sizes);
    return 0;
}

/*
  Time of Execution:
  Static Block-Cyclic: The execution times range from 5.84017s (chunk size 1) to 6.04175s (chunk size 8).
  Dynamic Block-Cyclic: The execution times range from 5.83903s (chunk size 1) to 5.93133s (chunk size 8).
 
  Method Analysis:
 
  Static Block-Cyclic:
  - In this approach, each thread gets a fixed chunk of work.
  - The time remains relatively stable across different chunk sizes, with only a slight increase as the chunk size grows (from 5.84017s to 6.04175s).
  - This method is efficient when the workload is uniform and the computational work can be evenly distributed among threads without much variation.
 
  Dynamic Block-Cyclic:
  - Here, chunks are dynamically assigned to threads. Threads that finish their work early can pick up additional chunks.
  - This method tends to perform better when there are significant variations in computational work between chunks (e.g., some chunks might take more time to compute).
  - The times are very similar to Static Block-Cyclic, with only slight variations (from 5.83903s to 5.93133s).
 
  Conclusion:
  - The Dynamic Block-Cyclic method shows a slight edge in performance, especially with smaller chunk sizes (1 and 2).
    The performance is consistent and tends to slightly outperform the Static Block-Cyclic method in most cases.
  - The Static Block-Cyclic method would be preferable when the workload is predictable and balanced, as it avoids the overhead of dynamic scheduling.
  - However, given the minimal difference in execution times, Dynamic Block-Cyclic may be slightly better for this problem due to its ability to adapt to variations in workload, especially for smaller chunks.
  - The difference in performance may not be significant enough to choose one method universally, but if maximizing efficiency is key, Dynamic Block-Cyclic is the preferable choice here.
 */
