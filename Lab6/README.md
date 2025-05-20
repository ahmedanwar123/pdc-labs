# Lab 6: Riemann Zeta Problem using Static and Dynamic Block-Cyclic

The **Riemann Zeta function** $\zeta(s)$ for a complex number $s$ with $\operatorname{Re}(s) > 1$ is defined as the infinite series:

$$
\zeta(s) = \sum_{n=1}^\infty \frac{1}{n^s}.
$$

This function has significant applications in number theory, physics, and applied mathematics. For computational purposes, particularly in parallel and distributed computing (PDC), the infinite sum is truncated to a finite number of terms $k$, and the function is approximated as:

$$
\zeta_k(s) = \sum_{n=1}^k \frac{1}{n^s}.
$$

In this implementation, a variant of the Riemann Zeta function is considered, involving double sums of the form:

$$
Z_k(s) = 2^s \sum_{i=1}^k \sum_{j=1}^k \frac{(-1)^{i+1}}{(i+j)^s},
$$

which is computationally intensive due to its quadratic complexity in $k$.

The problem is naturally parallelizable since the computation of $Z_k(s)$ for different values of $k$ (from 1 to $n$) can be done independently. Parallelizing this allows us to accelerate the computation by distributing chunks of work across multiple threads or processing units.

## Parallelization Strategies: Static and Dynamic Block-Cyclic Scheduling

To effectively distribute work among multiple threads, two block-cyclic scheduling strategies are employed:

### 1. Static Block-Cyclic Scheduling

In static scheduling, the iteration space of size $n$ is divided into chunks of size $c$. Each thread is assigned fixed chunks in a cyclic manner before execution starts. Formally, thread $t$ computes:

$$
\{X_i \mid i = t \cdot c, t \cdot c + 1, \ldots, \min((t+1) \cdot c - 1, n-1)\} \cup
\{X_i \mid i = t \cdot c + m \cdot c \cdot T, \ldots\},
$$

where $T$ is the total number of threads, $m \in \mathbb{N}$.

This method has minimal scheduling overhead and works well when workloads are uniform, but may suffer from load imbalance if some chunks require significantly more computation.

### 2. Dynamic Block-Cyclic Scheduling

Dynamic scheduling assigns chunks of size $c$ to threads at runtime, using an atomic counter that all threads increment to fetch their next chunk. This approach allows threads that finish early to pick up additional work, improving load balancing when workloads are irregular.

Let $\text{counter}$ be an atomic variable initially set to 0. Each thread atomically fetches and increments the counter by $c$, processes the assigned chunk, and repeats until all work is done.

## Experimental Results

The following table summarizes the execution times (in seconds) for computing the Riemann Zeta values for $n=2048$ elements, using $T=8$ threads and varying chunk sizes ($c \in \{1, 2, 4, 8\}$):

| Chunk Size | Static Block-Cyclic | Dynamic Block-Cyclic |
|------------|---------------------|----------------------|
| 1          | 5.84017 s           | 5.83903 s            |
| 2          | 5.94072 s           | 5.84897 s            |
| 4          | 6.00824 s           | 5.87931 s            |
| 8          | 6.04175 s           | 5.93133 s            |

## Analysis

- **Static block-cyclic scheduling** demonstrates relatively stable execution times across chunk sizes with a slight increase as chunk size grows. This behavior is expected because larger chunk sizes reduce scheduling overhead but can increase load imbalance if chunks have uneven workloads.
- **Dynamic block-cyclic scheduling** achieves slightly better and more consistent performance by dynamically distributing chunks based on thread availability. This reduces idle time caused by load imbalance.
- Both methods produce equivalent computational results, confirming correctness.

## Conclusion

For this computationally intensive problem with potentially uneven workload distribution, dynamic block-cyclic scheduling provides a small but consistent performance advantage due to its adaptive load balancing capability. Static block-cyclic scheduling remains simpler and can be preferable when workloads are uniform and predictable, as it incurs less overhead.

Choosing between these strategies depends on the specific workload characteristics and system constraints, but dynamic scheduling is generally more robust to variations in computation times across chunks.