# Lab 2: Dependency of Scaled Efficiency $\gamma = p^{\delta}$

Let the speedup of a parallel system be denoted as $S_\gamma(p)$, where $\gamma$ is the compute-to-communication ratio and $p$ is the number of processors. The scaled efficiency is defined as:

$$
E_\gamma(p) = \frac{S_\gamma(p)}{p}
$$

Now assume that the compute-to-communication ratio $\gamma$ scales with the number of processors as:

$$
\gamma = p^\delta
$$

Substituting this into the efficiency definition yields:

$$
E_\delta(p) = \frac{S_\gamma(p)}{p} = \frac{S_{p^\delta}(p)}{p}
$$

## Example Speedup Model

Consider a simplified speedup model:

$$
S_\gamma(p) = \frac{\gamma (p - 1)}{1 + \gamma}
$$

Substitute into the efficiency formula:

$$
E_\gamma(p) = \frac{S_\gamma(p)}{p} = \frac{\gamma (p - 1)}{p(1 + \gamma)}
$$

With $\gamma = p^\delta$, we get:

$$
E_\delta(p) = \frac{p^\delta (p - 1)}{p (1 + p^\delta)} = \frac{p^\delta \left(1 - \frac{1}{p} \right)}{1 + p^\delta}
$$

For large $p$, the approximation becomes:

$$
E_\delta(p) \approx \frac{p^\delta}{1 + p^\delta} = \frac{1}{1 + p^{-\delta}}
$$

## Behavior Analysis

- If $\delta > 0$: $p^{-\delta} \to 0$, so $E_\delta(p) \to 1$.
- If $\delta = 0$: $\gamma = 1$, so $E_\delta(p) \to 0$ as $p \to \infty$.
- If $\delta < 0$: $\gamma \to 0$, so $E_\delta(p) \to 0$.

**Conclusion:** To preserve efficiency as the number of processors increases, $\delta$ must be positive. That is, the problem size (parallelizable work) must grow with $p$.

## Iso-Efficiency Lines

Iso-efficiency functions describe how the problem size must scale with $p$ to maintain constant efficiency $E$.

From:

$$
E_\delta(p) = \frac{1}{1 + p^{-\delta}} = E \Rightarrow p^{-\delta} = \frac{1 - E}{E}
$$

Taking logarithms:

$$
-\delta \log p = \log\left(\frac{1 - E}{E}\right) \Rightarrow \delta = \frac{\log\left(\frac{E}{1 - E}\right)}{\log p}
$$

## Interpretation

- This expression tells us how $\delta$ (i.e., the rate of growth of the parallel portion of the problem) must increase to maintain a fixed efficiency $E$ as $p$ grows.
- Equivalently, the parallel workload $\gamma = p^\delta$ must grow fast enough to compensate for increasing communication and synchronization overhead.

## Summary

- Scaled efficiency drops if the compute-to-communication ratio $\gamma$ does not grow with $p$.
- Iso-efficiency analysis helps determine the required growth rate of problem size (parameterized by $\delta$) to sustain efficiency.
- The higher the number of processors, the faster the parallel workload must scale to avoid diminishing returns.