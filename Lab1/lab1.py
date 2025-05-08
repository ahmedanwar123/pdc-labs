import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from typing import Optional


def speedup(gamma: np.ndarray, q: np.ndarray, k: int) -> np.ndarray:
    return (gamma * (2**k - 1)) / (2 * q + gamma * (2 ** (k - q) - 1 + q))


k: int = 10
n: int = 2**k

q_values: np.ndarray = np.linspace(0.1, 10, 100)
gamma_values: np.ndarray = np.linspace(0.01, 0.35, 100)
Q, G = np.meshgrid(q_values, gamma_values)
S: np.ndarray = speedup(G, Q, k)

fig: plt.Figure = plt.figure()
ax: Axes3D = fig.add_subplot(111, projection="3d")
surface = ax.plot_surface(Q, G, S, cmap="Blues", edgecolor="none", alpha=0.7)

optimal_gamma: np.ndarray = np.linspace(0.01, 0.35, 100)
optimal_q: np.ndarray = np.log2((optimal_gamma * np.log(2) * n) / (2 + optimal_gamma))
optimal_speedup: np.ndarray = speedup(optimal_gamma, optimal_q, k)

ax.plot(
    optimal_q,
    optimal_gamma,
    optimal_speedup,
    color="red",
    label="Optimal Speedup",
    linewidth=2,
)

ax.set_xlabel("q")
ax.set_ylabel("γ")
ax.set_zlabel("F(γ, q)")
ax.set_title("The General Case and the Compute-to-Communication Ratio")
ax.view_init(elev=30, azim=225)  # Adjust angles as needed
ax.set_zlim(0, 20)
plt.legend()
plt.savefig("lab1.png")
