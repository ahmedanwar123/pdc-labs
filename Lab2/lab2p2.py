import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


def compute_efficiency(
    p_values: np.ndarray, delta_values: np.ndarray, f_1: float, f_2: float
) -> np.ndarray:
    P, Delta = np.meshgrid(p_values, delta_values)
    S_1 = (f_1 + (1 - f_1) * P**Delta) / (f_1 + (1 - f_1) * P ** (Delta - 1))
    E = S_1 / P
    return P, Delta, E


def plot_efficiency(
    p_values: np.ndarray, delta_values: np.ndarray, efficiency: np.ndarray
) -> None:
    fig: plt.Figure = plt.figure()
    ax1: Axes3D = fig.add_subplot(projection="3d")
    ax1.plot_surface(
        p_values,
        delta_values,
        efficiency,
        cmap="Blues",
        edgecolor="blue",
        rcount=20,
        ccount=15,
    )
    ax1.set_xlabel(r"$p$")
    ax1.set_ylabel(r"$\delta$")
    ax1.set_zlabel(r"$E_{\gamma = p^\delta}(p)$")
    ax1.set_zlim(0, 1)
    ax1.set_title(r"Efficiency $E_{\gamma = p^\delta}(p)$ for $f=0.1$")
    plt.show()


p_values: np.ndarray = np.linspace(1, 20, 25)
delta_values: np.ndarray = np.linspace(0, 1, 25)
f_1: float = 0.1
f_2: float = 0.5

P, Delta, E = compute_efficiency(p_values, delta_values, f_1, f_2)
plot_efficiency(P, Delta, E)
