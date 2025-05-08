import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from typing import Optional


# Speedup function
def speedup(p: np.ndarray, f: float, delta: np.ndarray) -> np.ndarray:
    return (f + (1 - f) * p**delta) / (f + (1 - f) * p ** (delta - 1))


p_values: np.ndarray = np.linspace(1, 20, 25)
delta_values: np.ndarray = np.linspace(0, 1, 25)

P: np.ndarray
Delta: np.ndarray
P, Delta = np.meshgrid(p_values, delta_values)

# Compute speedup for f = 0.1 and f = 0.5
S_01: np.ndarray = speedup(P, f=0.1, delta=Delta)
S_05: np.ndarray = speedup(P, f=0.5, delta=Delta)

# Plotting the graphs
fig: plt.Figure = plt.figure(figsize=(12, 5))

# First subplot for f = 0.1
ax1: Axes3D = fig.add_subplot(121, projection="3d")
ax1.plot_surface(P, Delta, S_01, cmap="Blues", edgecolor="blue", rcount=20, ccount=20)
ax1.set_xlabel(r"$p$")
ax1.set_ylabel(r"$\delta$")
ax1.set_zlabel(r"$S_{\gamma = p^\delta}(p)$")
ax1.set_title(r"Speedup $S_{\gamma=p^\delta}(p)$ for $f=0.1$")

# Second subplot for f = 0.5
ax2: Axes3D = fig.add_subplot(122, projection="3d")
ax2.plot_surface(P, Delta, S_05, cmap="Blues", edgecolor="blue", rcount=20, ccount=20)
ax2.set_xlabel(r"$p$")
ax2.set_ylabel(r"$\delta$")
ax2.set_zlabel(r"$S_{\gamma = p^\delta}(p)$")
ax2.set_title(r"Speedup $S_{\gamma=p^\delta}(p)$ for $f=0.5$")

# Show plots
plt.tight_layout()
plt.show()
