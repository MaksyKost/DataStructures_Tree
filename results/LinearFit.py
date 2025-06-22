import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

# Rozmiary drzew
sizes = np.array([10000, 20000, 30000, 40000, 50000,
                  60000, 70000, 80000, 90000, 100000]).reshape(-1, 1)

# Dane czasowe (Insert, Remove, Search) [s]
trees = {
    'AVL Tree': {
        'Insert':  [2.792e-07, 3.23e-07, 3.502e-07, 3.618e-07, 3.586e-07,
                    3.888e-07, 4.164e-07, 4.14e-07, 4.852e-07, 4.808e-07],
        'Remove':  [2.254e-07, 2.388e-07, 2.472e-07, 2.532e-07, 2.394e-07,
                    2.6e-07, 2.692e-07, 2.712e-07, 2.778e-07, 2.768e-07],
        'Search':  [4.18e-08, 4.76e-08, 4.3e-08, 4.68e-08, 4.92e-08,
                    5.16e-08, 4.52e-08, 5.22e-08, 5.14e-08, 5.58e-08]
    },
    'TTree': {
        'Insert':  [6.564e-07, 8.802e-07, 7.92e-07, 8.326e-07, 8.618e-07,
                    8.968e-07, 9.22e-07, 1.003e-06, 1.0888e-06, 1.068e-06],
        'Remove':  [2.768e-07, 3.188e-07, 2.978e-07, 3.068e-07, 3.132e-07,
                    3.138e-07, 3.248e-07, 3.238e-07, 3.406e-07, 3.374e-07],
        'Search':  [2.418e-07, 2.718e-07, 2.594e-07, 2.668e-07, 2.634e-07,
                    2.672e-07, 2.686e-07, 2.75e-07, 2.67e-07, 2.744e-07]
    },
    'Splay Tree': {
        'Insert':  [9.5e-08, 1.194e-07, 1.032e-07, 1.302e-07, 1.218e-07,
                    1.428e-07, 1.394e-07, 1.51e-07, 1.448e-07, 1.572e-07],
        'Remove':  [5.06e-08, 6.54e-08, 5.48e-08, 7.32e-08, 6.76e-08,
                    7.68e-08, 7.66e-08, 7.44e-08, 7.64e-08, 8.86e-08],
        'Search':  [3.36e-08, 3.48e-08, 3.18e-08, 3.58e-08, 3.32e-08,
                    3.64e-08, 3.5e-08, 3.1e-08, 3.3e-08, 3.26e-08]
    },
    'Dancing Tree': {
        'Insert':  [7.62e-08, 9.58e-08, 9.84e-08, 1.012e-07, 1.49e-07,
                    1.224e-07, 1.298e-07, 1.244e-07, 1.308e-07, 1.376e-07],
        'Remove':  [5.16e-08, 6.38e-08, 6.58e-08, 7.06e-08, 8.4e-08,
                    7.4e-08, 8.06e-08, 8.18e-08, 8.34e-08, 8.38e-08],
        'Search':  [3.44e-08, 3.94e-08, 4.1e-08, 4.42e-08, 4.46e-08,
                    4.06e-08, 5.02e-08, 5.08e-08, 4.92e-08, 5.08e-08]
    }
}

# Funkcja pomocnicza do regresji i wykresu
def plot_tree_operation(tree_name, operation, color):
    y = np.array(trees[tree_name][operation])
    model = LinearRegression().fit(sizes, y)
    y_pred = model.predict(sizes)

    plt.figure(figsize=(7, 4))
    plt.scatter(sizes, y, label='Pomiary', color=color)
    plt.plot(sizes, y_pred, 'k--', label='Dopasowanie liniowe')
    plt.title(f'{tree_name} - {operation}')
    plt.xlabel('Liczba elementów')
    plt.ylabel('Czas [s]')
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.show()

# Generowanie wykresów
colors = ['blue', 'green', 'red']
for tree_name in trees:
    for idx, operation in enumerate(['Insert', 'Remove', 'Search']):
        plot_tree_operation(tree_name, operation, colors[idx])