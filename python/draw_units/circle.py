from Bezier import Bezier
import matplotlib.pyplot as plt
import numpy as np
import seaborn

num_subdivisions = 80
points = []
alpha = 2 * np.pi / num_subdivisions
t_values = np.linspace(0, 1, num_subdivisions * 10)

for i in np.linspace(0, 2 * np.pi, num_subdivisions):
    points.append((np.cos(i), np.sin(i)))
points.append(points[0])

nodes = np.asarray(points)
curve = Bezier.Curve(t_values, nodes)

plt.figure(figsize=(8, 8))
plt.plot(
    curve[:, 0],  # x-coordinates.
    curve[:, 1]  # y-coordinates.
)
plt.plot(
    nodes[:, 0],  # x-coordinates.
    nodes[:, 1],  # y-coordinates.
    'ro:'  # Styling (red, circles, dotted).
)
plt.grid()
plt.tight_layout()
plt.savefig('circle.png', dpi=300)
plt.show()
